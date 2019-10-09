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

static bool init_ddcutil_library(Parsed_Cmd * parsed_cmd) {

   bool ok = true;

   if (parsed_cmd->traced_functions) {
      char * funcname = NULL;
      int ndx = 0;
      for (funcname = parsed_cmd->traced_functions[0];
           funcname;
           funcname = parsed_cmd->traced_functions[++ndx])
      {
         printf("(%s) adding traced function: %s\n",  __func__, funcname);
         ddca_add_traced_function(funcname);
      }
   }

   if (parsed_cmd->traced_files) {
      char * filename = NULL;
      int ndx = 0;
      for (filename = parsed_cmd->traced_files[0];
           filename;
           filename = parsed_cmd->traced_files[++ndx]) {
         printf("(%s) adding traced file: %s\n", __func__, filename);
         ddca_add_traced_file(filename);
      }
   }

   ddca_set_trace_groups(parsed_cmd->traced_groups);

   /*
   CMD_FLAG_TIMESTAMP_TRACE     = 0x0008,  // prepend trace and debug msgs with elapsed time
   CMD_FLAG_REPORT_FREED_EXCP   = 0x0200,
   CMD_FLAG_ENABLE_UDF        = 0x100000,
   CMD_FLAG_NOUSB             = 0x200000,
   */

   /*
   *  Must be called before any API call that triggers display identification
   *
   *  @param onoff
   *  @retval  DDCRC_OK                success
   *  @retval  DDCRC_INVALID_OPERATION display identification has already occurred
   *  @retval  DDCRC_UNIMPLEMENTED     ddcutil not built with USB monitor support
   */
   DDCA_Status rc =
   ddca_enable_usb_display_detection(parsed_cmd->flags & CMD_FLAG_NOUSB);

   ddca_enable_udf(              parsed_cmd->flags & CMD_FLAG_ENABLE_UDF);
   ddca_enable_report_ddc_errors(parsed_cmd->flags & CMD_FLAG_DDCDATA);
   ddca_enable_error_info(       parsed_cmd->flags & CMD_FLAG_REPORT_FREED_EXCP);

   if (parsed_cmd->max_tries[0] > 0) {
      ddca_set_max_tries(        DDCA_WRITE_ONLY_TRIES, parsed_cmd->max_tries[0]);
   }
   if (parsed_cmd->max_tries[1] > 0) {
      ddca_set_max_tries(       DDCA_WRITE_READ_TRIES, parsed_cmd->max_tries[1]);
   }
   if (parsed_cmd->max_tries[2] > 0) {
      ddca_set_max_tries(       DDCA_MULTI_PART_TRIES, parsed_cmd->max_tries[2]);
   }
   if (parsed_cmd->sleep_multiplier != 1.0f) {
      ddca_set_global_sleep_multiplier(sleep_multiplier);
   }


   return ok;
}


int main(int argc, char *argv[])
{
    // will remove any arguments that it recognizes, e.g. --widgetcount
    QApplication a(argc, argv);

    Parsed_Cmd * parsed_cmd = parse_command(argc, argv);
    if (!parsed_cmd)
       return 1;

    if (!init_ddcutil_library(parsed_cmd))
       return 1;

    QStringList styles = QStyleFactory::keys();
    printf("Known styles:\n");
    for (int ndx = 0;  ndx < styles.size(); ndx++)
    {
       printf("  %s\n", styles.at(ndx).toLocal8Bit().constData());
    }



    MainWindow w;
    w.show();

    return a.exec();
}
