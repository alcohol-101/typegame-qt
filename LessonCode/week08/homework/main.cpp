#include <QApplication>
#include <QFont>
#include<QDebug>
#include "mainwindow.h"

#include "ButtonWithText.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MainWindow window;


    window.show();

    return app.exec();
}
