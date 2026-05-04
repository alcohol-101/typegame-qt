#include <QtTest>
#include <QApplication>
#include "exitconfirmdialog.h"

class TestExitConfirmDialog : public QObject
{
    Q_OBJECT

private slots:
    void testTwoButtonConstructor();
    void testThreeButtonConstructor();
    void testDefaultConfirmed();
};

void TestExitConfirmDialog::testTwoButtonConstructor()
{
    QWidget parent;
    QPixmap bg(100, 100);
    bg.fill(Qt::white);
    bg.save("test_bg.png");
    QPixmap left(100, 50);
    left.fill(Qt::red);
    left.save("test_left.png");
    QPixmap right(100, 50);
    right.fill(Qt::green);
    right.save("test_right.png");

    ExitConfirmDialog dlg(&parent,
        "test_bg.png", "test_left.png", "test_right.png",
        tr("确认退出？"));
    QVERIFY(!dlg.isConfirmed());
    QCOMPARE(dlg.isConfirmed_3(), 0);

    QFile::remove("test_bg.png");
    QFile::remove("test_left.png");
    QFile::remove("test_right.png");
}

void TestExitConfirmDialog::testThreeButtonConstructor()
{
    QWidget parent;
    QPixmap bg(100, 100);
    bg.fill(Qt::white);
    bg.save("test_bg2.png");
    QPixmap left(100, 50);
    left.fill(Qt::red);
    left.save("test_left2.png");
    QPixmap mid(100, 50);
    mid.fill(Qt::blue);
    mid.save("test_mid.png");
    QPixmap right(100, 50);
    right.fill(Qt::green);
    right.save("test_right2.png");

    ExitConfirmDialog dlg(&parent,
        "test_bg2.png", "test_left2.png", "test_mid.png", "test_right2.png",
        tr("设置改变"));
    QVERIFY(!dlg.isConfirmed());
    QCOMPARE(dlg.isConfirmed_3(), 0);

    QFile::remove("test_bg2.png");
    QFile::remove("test_left2.png");
    QFile::remove("test_mid.png");
    QFile::remove("test_right2.png");
}

void TestExitConfirmDialog::testDefaultConfirmed()
{
    QWidget parent;
    ExitConfirmDialog dlg(&parent,
        ":/res/image/Common/Images/MAIN_DLG_BG.png",
        ":/res/image/Common/Images/MAIN_DLG_EXIT.png",
        ":/res/image/Common/Images/MAIN_DLG_REPLAY.png",
        tr("测试"));
    QVERIFY(!dlg.isConfirmed());
    QCOMPARE(dlg.isConfirmed_3(), 0);
}

QTEST_MAIN(TestExitConfirmDialog)
#include "tst_exitconfirmdialog.moc"
