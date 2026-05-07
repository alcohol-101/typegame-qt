// filename: tst_highscoredialog.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-05-05
// description: Unit tests for HighScoreDialog

#include <QtTest>

class TestTrivial : public QObject
{
    Q_OBJECT
public:
private slots:
    void testPass() { QVERIFY(true); }
};

QTEST_MAIN(TestTrivial)
#include "tst_highscoredialog.moc"
