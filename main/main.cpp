/* main.cpp */

#include <QtWidgets/QApplication>

// #include <ui_main.h>
#include "main/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
