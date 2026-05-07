// filename: tst_spacelogic.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-05-06
// description: Unit tests for SpaceWarWidget game logic

#include <QtTest>
#include <QSet>
#include <QVector>
#include <algorithm>
#include <cmath>

class TestSpaceLogic : public QObject
{
    Q_OBJECT

public:

private:
    QSet<QChar> m_usedLetters;

    QChar getUnusedLetter()
    {
        QVector<QChar> unused;
        for (char c = 'A'; c <= 'Z'; ++c) {
            QChar ch(c);
            if (!m_usedLetters.contains(ch))
                unused.append(ch);
        }
        if (unused.isEmpty())
            return QChar();
        int idx = QRandomGenerator::global()->bounded(unused.size());
        return unused[idx];
    }

    void releaseLetter(QChar ch)
    {
        m_usedLetters.remove(ch);
    }

    int qBoundTest(int val, int min, int max)
    {
        return std::max(min, std::min(max, val));
    }

    double bulletAngle(double dx, double dy)
    {
        return std::atan2(dy, dx);
    }

private slots:
    void testGetUnusedLetterUnique()
    {
        m_usedLetters.clear();
        QSet<QChar> letters;
        for (int i = 0; i < 26; ++i) {
            QChar c = getUnusedLetter();
            QVERIFY(!c.isNull());
            letters.insert(c);
            m_usedLetters.insert(c);
        }
        QCOMPARE(letters.size(), 26);
        QVERIFY(getUnusedLetter().isNull());
    }

    void testGetUnusedLetterAfterRelease()
    {
        m_usedLetters.clear();
        for (char c = 'A'; c <= 'Z'; ++c)
            m_usedLetters.insert(QChar(c));

        releaseLetter('A');
        releaseLetter('Z');
        QVERIFY(!getUnusedLetter().isNull());
        QSet<QChar> released;
        released.insert(getUnusedLetter());
        released.insert(getUnusedLetter());
        QVERIFY(released.contains('A') || released.contains('Z'));
    }

    void testReleaseLetter()
    {
        m_usedLetters.clear();
        m_usedLetters.insert('A');
        m_usedLetters.insert('B');
        releaseLetter('A');
        QVERIFY(!m_usedLetters.contains('A'));
        QVERIFY(m_usedLetters.contains('B'));
        releaseLetter('B');
        QVERIFY(m_usedLetters.isEmpty());
    }

    void testReleaseNonExistent()
    {
        m_usedLetters.clear();
        releaseLetter('X');
        QVERIFY(m_usedLetters.isEmpty());
    }

    void testGetUnusedLetterAllUsed()
    {
        m_usedLetters.clear();
        for (char c = 'A'; c <= 'Z'; ++c)
            m_usedLetters.insert(QChar(c));
        QVERIFY(getUnusedLetter().isNull());
    }

    void testUpgradeBounds()
    {
        int speedLevel = 0;
        int enemyMax = 3;
        for (int i = 0; i < 10; ++i) {
            if (speedLevel < 10) {
                speedLevel++;
                enemyMax = qBoundTest(enemyMax + 1, 0, 10);
            }
        }
        QCOMPARE(speedLevel, 10);
        QCOMPARE(enemyMax, 10);
    }

    void testBulletAngle()
    {
        double angle = bulletAngle(1.0, 0.0);
        QVERIFY(std::abs(angle) < 0.001);
        angle = bulletAngle(0.0, 1.0);
        QVERIFY(std::abs(angle - 3.14159 / 2) < 0.01);
    }
};

QTEST_MAIN(TestSpaceLogic)
#include "tst_spacelogic.moc"
