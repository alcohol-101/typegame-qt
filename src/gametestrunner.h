#ifndef GAMETESTRUNNER_H
#define GAMETESTRUNNER_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

struct RoundResult {
    int round;
    QString mode;
    bool passed;
    QJsonObject stats;
    int expectedCorrect;
    int expectedWrong;
    int expectedScore;
};

class GameTestRunner
{
public:
    GameTestRunner(int argc, char* argv[]);
    int run();

private:
    QString m_gameName;
    QString m_inputPath;
    QString m_outputPath;

    QJsonObject m_config;
    QString m_letters;

    bool parseArgs(int argc, char* argv[]);
    bool loadConfig();
    RoundResult runRound(const QString& mode, const QString& letters, int errorFrequency);
    QVector<bool> generateCorrectMask(int total, const QString& mode, int errorFrequency);
    void saveResults(const QJsonArray& rounds);
};

#endif
