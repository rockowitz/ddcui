/* main.cpp */

/* Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

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
