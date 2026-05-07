// filename: main.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04-05
// description: Entry point for the typing game application

#include <QApplication>
#include <QFont>
#include <QDebug>
#include <QTimer>
#include <cstring>
#include "mainwindow.h"
#include "gametestrunner.h"
#include "buttonwithtext.h"

int main(int argc, char *argv[])
{
    if (argc >= 3 && strcmp(argv[2], "--test") == 0) {
        QApplication app(argc, argv);
        GameTestRunner runner(argc, argv);
        int ret = runner.run();
        QTimer::singleShot(100, &app, &QApplication::quit);
        return ret;
    }
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
