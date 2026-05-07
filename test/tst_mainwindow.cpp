// filename: tst_mainwindow.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-05-06
// description: Unit tests for MainWindow

#include <QtTest>
#include <QApplication>
#include "mainwindow.h"

class TestMainWindow : public QObject
{
    Q_OBJECT

public:

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
