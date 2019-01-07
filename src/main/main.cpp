/* main.cpp */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>
#include <QtWidgets/QApplication>

#include <ddcutil_c_api.h>

extern "C" {
#include "cmdline/parsed_cmd.h"
#include "cmdline/cmd_parser.h"
}

// #include <ui_main.h>
#include "main/mainwindow.h"

int main(int argc, char *argv[])
{
    // will remove any arguments that it recognizes, e.g. --widgetcount
    QApplication a(argc, argv);

    Parsed_Cmd * parsed_cmd = parse_command(argc, argv);
    ddca_enable_udf(parsed_cmd->flags & CMD_FLAG_ENABLE_UDF);

    MainWindow w;
    w.show();

    return a.exec();
}
