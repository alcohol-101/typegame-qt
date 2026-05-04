#include <QtTest>

class TestTrivial : public QObject
{
    Q_OBJECT
private slots:
    void testPass() { QVERIFY(true); }
};

QTEST_MAIN(TestTrivial)
#include "tst_highscoredialog.moc"
