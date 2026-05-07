// filename: gametestrunner.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-05
// description: Implementation of GameTestRunner

#include "gametestrunner.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QKeyEvent>
#include <QTest>
#include <QDebug>
#include <cmath>
#include <memory>

#include "applegamewidget.h"
#include "spacewarwidget.h"

GameTestRunner::GameTestRunner(int argc, char* argv[])
{
    parseArgs(argc, argv);
}

bool GameTestRunner::parseArgs(int argc, char* argv[])
{
    if (argc < 7) return false;
    m_gameName = QString(argv[1]).toLower();
    if (m_gameName != "apple" && m_gameName != "space") return false;
    if (QString(argv[2]) != "--test") return false;

    for (int i = 3; i < argc; ++i) {
        if (QString(argv[i]) == "--input" && i + 1 < argc) m_inputPath = argv[++i];
        else if (QString(argv[i]) == "--output" && i + 1 < argc) m_outputPath = argv[++i];
    }
    return !m_inputPath.isEmpty() && !m_outputPath.isEmpty();
}

bool GameTestRunner::loadConfig()
{
    QFile file(m_inputPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Cannot open config:" << m_inputPath;
        return false;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        qCritical() << "JSON error:" << err.errorString();
        return false;
    }
    m_config = doc.object();
    m_letters = m_config["letters"].toString();
    return !m_letters.isEmpty();
}

static QChar MakeWrong(QChar c)
{
    QChar u = c.toUpper();
    return (u == 'Z') ? 'A' : QChar(u.unicode() + 1);
}

QVector<bool> GameTestRunner::generateCorrectMask(int total, const QString& mode, int errorFrequency)
{
    QVector<bool> mask(total, false);
    if (mode == "AllCorrect") {
        mask.fill(true);
    } else if (mode == "AllWrong") {
        mask.fill(false);
    } else {
        for (int i = 0; i < total; ++i)
            mask[i] = ((i + 1) % errorFrequency != 0);
    }
    return mask;
}

RoundResult GameTestRunner::runRound(const QString& mode, const QString& letters, int errorFrequency)
{
    RoundResult result;
    result.mode = mode;
    result.passed = false;

    QVector<bool> correctMask = generateCorrectMask(letters.size(), mode, errorFrequency);
    int expectedCorrect = 0, expectedWrong = 0;
    for (bool c : correctMask) { if (c) expectedCorrect++; else expectedWrong++; }
    result.expectedCorrect = expectedCorrect;
    result.expectedWrong = expectedWrong;

    if (m_gameName == "apple") {
        auto widget = std::make_unique<AppleGameWidget>();
        widget->resize(800, 600);
        widget->setTestMode(true);
        widget->setTestLetters(letters);
        widget->setGameParameters(m_config);
        widget->startGame();
        widget->showMinimized();
        QApplication::processEvents();
        QTest::qWait(150);

        for (int i = 0; i < letters.size(); ++i) {
            if (!widget->isGameActive()) break;
            QChar c = letters.at(i).toUpper();
            if (!c.isLetter()) continue;

            QChar typeChar = correctMask[i] ? c : MakeWrong(c);
            QKeyEvent ke(QEvent::KeyPress, 0, Qt::NoModifier, QString(typeChar));
            QApplication::sendEvent(widget.get(), &ke);
            QApplication::processEvents();
            QTest::qWait(150);
        }

        result.expectedScore = 0;

        QJsonObject stats = widget->getGameStats();
        stats["health"] = widget->getGameStats()["health"];
        stats["prizeText"] = QJsonObject{{"chars", 0}, {"words", 0}};

        int actualCorrect = widget->getSuccessCount();
        if (mode == "AllCorrect") result.passed = (actualCorrect == expectedCorrect);
        else if (mode == "AllWrong") result.passed = (actualCorrect == 0);
        else {
            double r = (expectedCorrect > 0) ? qAbs(actualCorrect - expectedCorrect) / static_cast<double>(expectedCorrect) : 1.0;
            result.passed = (r <= 0.05);
        }

        QJsonObject s;
        s["health"] = QString("%1/%2").arg(widget->getCurrentHealth()).arg(widget->getMaxHealth());
        s["missed"] = widget->getBadCount();
        s["correctInputs"] = actualCorrect;
        s["wrongInputs"] = widget->getWrongInputCount();
        s["score"] = 0;
        s["prizeText"] = QJsonObject{{"chars", 0}, {"words", 0}};
        result.stats = s;

        widget->stopGame();
        widget->close();
    } else {
        auto widget = std::make_unique<SpaceWarWidget>();
        widget->resize(800, 600);
        widget->setTestMode(true);
        widget->skipSounds();

        QJsonObject s = m_config["settings"].toObject();
        if (!s.contains("enemysNum")) s["enemysNum"] = 1;
        if (!s.contains("upgradeInterval")) s["upgradeInterval"] = 120;
        QJsonObject cfg;
        cfg["settings"] = s;
        widget->setGameParameters(cfg);
        widget->setTestLetters(letters);

        int totalLetters = letters.size();

        widget->showMinimized();
        QApplication::processEvents();
        QTest::qWait(200);

        QMetaObject::invokeMethod(widget.get(), "onStartClicked", Qt::DirectConnection);
        QApplication::processEvents();
        QTest::qWait(300);

        for (int i = 0; i < totalLetters; ++i) {
            if (!widget->isGameActive()) break;
            QChar c = letters.at(i).toUpper();
            if (!c.isLetter()) continue;

            QChar typeChar = correctMask[i] ? c : MakeWrong(c);
            QKeyEvent ke(QEvent::KeyPress, 0, Qt::NoModifier, QString(typeChar));
            QApplication::sendEvent(widget.get(), &ke);
            QApplication::processEvents();
            QTest::qWait(60 + (correctMask[i] ? 120 : 0));
        }

        int actualCorrect = widget->getCorrectInputCount();
        int actualWrong = widget->getWrongInputCount();
        result.expectedScore = expectedCorrect * 1500 + expectedWrong * (-400);

        QJsonObject stats;
        stats["health"] = QString("%1/%2").arg(widget->getLives()).arg(widget->getMaxLives());
        stats["missed"] = widget->getMissedCount();
        stats["correctInputs"] = actualCorrect;
        stats["wrongInputs"] = actualWrong;
        stats["score"] = widget->getScore();

        int pw = 0, pc = 0;
        stats["prizeText"] = QJsonObject{{"chars", pc}, {"words", pw}};
        result.stats = stats;

        if (mode == "AllCorrect") result.passed = (actualCorrect == expectedCorrect);
        else if (mode == "AllWrong") result.passed = (actualCorrect == 0);
        else {
            double r = (expectedCorrect > 0) ? qAbs(actualCorrect - expectedCorrect) / static_cast<double>(expectedCorrect) : 1.0;
            result.passed = (r <= 0.05);
        }

        QMetaObject::invokeMethod(widget.get(), "stopGame", Qt::DirectConnection);
        widget->close();
    }

    return result;
}

void GameTestRunner::saveResults(const QJsonArray& rounds)
{
    bool allPassed = true;
    int totalRounds = rounds.size();
    int passedCount = 0;
    int acPassed = 0, acTotal = 0, wePassed = 0, weTotal = 0, awPassed = 0, awTotal = 0;

    QJsonArray formattedRounds;

    for (int i = 0; i < rounds.size(); ++i) {
        QJsonObject rd = rounds[i].toObject();
        QString mode = rd["mode"].toString();
        bool passed = rd["passed"].toBool();

        if (mode == "AllCorrect") { acTotal++; if (passed) acPassed++; }
        else if (mode == "WithErrors") { weTotal++; if (passed) wePassed++; }
        else { awTotal++; if (passed) awPassed++; }

        if (passed) passedCount++; else allPassed = false;

        QJsonObject formatted;
        formatted["round"] = i + 1;
        formatted["mode"] = mode;
        formatted["passed"] = passed;

        QJsonObject stats = rd["stats"].toObject();
        int actualCorrect = stats["correctInputs"].toInt();
        int actualWrong = stats["wrongInputs"].toInt();
        int actualScore = stats["score"].toInt();
        int expectedCorrect = rd["expectedCorrect"].toInt();
        int expectedWrong = rd["expectedWrong"].toInt();
        int expectedScore = rd["expectedScore"].toInt();

        QJsonObject comp;
        comp["correctInputs"] = QString("%1/%2").arg(actualCorrect).arg(expectedCorrect);
        comp["wrongInputs"] = QString("%1/%2").arg(actualWrong).arg(expectedWrong);
        comp["score"] = QString("%1/%2").arg(actualScore).arg(expectedScore);
        formatted["comparison"] = comp;
        formatted["health"] = stats["health"];
        formatted["missed"] = stats["missed"].toInt();
        formatted["prizeText"] = stats["prizeText"];

        formattedRounds.append(formatted);
    }

    QJsonObject output;
    output["allPassed"] = allPassed;
    output["gameName"] = m_gameName == "apple" ? "Apple" : "Space";

    QJsonObject ms;
    ms["AllCorrect"] = QString("%1/%2").arg(acPassed).arg(acTotal);
    ms["AllWrong"] = QString("%1/%2").arg(awPassed).arg(awTotal);
    ms["WithErrors"] = QString("%1/%2").arg(wePassed).arg(weTotal);
    output["modeStats"] = ms;

    output["result"] = allPassed ? QString("%1/%1 passed").arg(passedCount) :
                                    QString("%1/%2 passed").arg(passedCount).arg(totalRounds);
    output["rounds"] = formattedRounds;

    QJsonDocument doc(output);
    QFile outFile(m_outputPath);
    if (outFile.open(QIODevice::WriteOnly)) {
        outFile.write(doc.toJson(QJsonDocument::Indented));
        outFile.close();
        qDebug() << "Results saved to" << m_outputPath;
    } else {
        qCritical() << "Cannot write" << m_outputPath;
    }
}

int GameTestRunner::run()
{
    if (!loadConfig()) return 2;

    QJsonObject tc = m_config["testConfig"].toObject();
    int cRounds = tc["correctRounds"].toInt(1);
    int eRounds = tc["errorRounds"].toInt(1);
    int wRounds = tc["allWrongRounds"].toInt(1);
    int errFreq = tc["errorFrequency"].toInt(5);

    QJsonArray rounds;

    auto appendRound = [&](const RoundResult& r) {
        QJsonObject rd;
        rd["mode"] = r.mode;
        rd["passed"] = r.passed;
        rd["stats"] = r.stats;
        rd["expectedCorrect"] = r.expectedCorrect;
        rd["expectedWrong"] = r.expectedWrong;
        rd["expectedScore"] = r.expectedScore;
        rounds.append(rd);
    };

    for (int i = 0; i < cRounds; ++i) appendRound(runRound("AllCorrect", m_letters, errFreq));
    for (int i = 0; i < eRounds; ++i) appendRound(runRound("WithErrors", m_letters, errFreq));
    for (int i = 0; i < wRounds; ++i) appendRound(runRound("AllWrong", m_letters, errFreq));

    saveResults(rounds);
    return 0;
}
