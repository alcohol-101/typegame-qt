#include <QtTest>
#include <QApplication>
#include "mainwindow.h"

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor();
    void testWindowFlags();
};

void TestMainWindow::testConstructor()
{
    MainWindow* window = new MainWindow();
    QVERIFY(window != nullptr);
    QVERIFY(window->windowFlags() & Qt::FramelessWindowHint);
    delete window;
}

void TestMainWindow::testWindowFlags()
{
    MainWindow window;
    QVERIFY(window.windowFlags() & Qt::FramelessWindowHint);
}

QTEST_MAIN(TestMainWindow)
#include "tst_mainwindow.moc"
