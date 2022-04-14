/** \file main.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <QtWidgets/QApplication>

#include <ddcutil_c_api.h>

extern "C" {
#include "c_util/debug_util.h"
#include "c_util/ddcutil_config_file.h"
#include "c_util/simple_ini_file.h"
#include "c_util/string_util.h"
#include "c_util/xdg_util.h"

#include "cmdline/ddcui_parsed_cmd.h"
#include "cmdline/ddcui_cmd_parser.h"
}

#include "base/ddcui_parms.h"
#include "base/global_state.h"
#include "base/core.h"
#include "main/msgbox_thread.h"

#include "main/mainwindow.h"


/* See: https://www.qt.io/blog/2016/01/26/high-dpi-support-in-qt-5-6
 */


void dbgrpt_hidpi_environment_vars() {
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   QStringList varnames = {
              "QT_AUTO_SCREEN_SCALE_FACTOR",
              "QT_SCALE_FACTOR",
              "QT_SCREEN_SCALE_FACTORS",
              "QT_ENABLE_HIGHDPI_SCALING",
              "QT_DEVICE_PIXEL_RATIO"                    // introduced 5.4, deprecated 5.6
   };
   for (int ndx = 0; ndx < varnames.size(); ndx++) {
      QString curvar= varnames.at(ndx);
      QString curval = env.value(curvar, "not set");
      printf("%s = %s\n", QS2S(curvar), QS2S(curval));
   }
}


const int TitleColumnSize = 25;

void qrpt_Qstring(QString name, QString val) {
   printf("    %-*s: %s\n", TitleColumnSize, QS2S(name), QS2S(val));
}

void qrpt_QSizef(QString name, QSizeF val) {
   qreal h = val.height();
   qreal w = val.width();
   printf("    %-*s: w = %f, h=%f\n", TitleColumnSize, QS2S(name), w, h);
}

void qrpt_qreal(QString name, qreal val) {
   printf("    %-*s: %f\n", TitleColumnSize, QS2S(name), val);
}

void qrpt_int(QString name, int val) {
   printf("    %-*s: %d\n", TitleColumnSize, QS2S(name), val);
}

void qrpt_QRect(QString name, QRect val) {
   int h = val.height();
   int w = val.width();
   int l = val.left();
   int r = val.right();
   printf("    %-*s: w = %d, h=%d, l=%d, r=%d\n", TitleColumnSize, QS2S(name), w, h, l, r);
}

void qrpt_QSize(QString name, QSize val) {
   int h = val.height();
   int w = val.width();
   printf("    %-*s: w = %d, h=%d\n", TitleColumnSize, QS2S(name), w, h);
}

void dbgrptQScreen(QScreen * screen) {
   QString s = screen->name();     // on X11, XRandr screen names
   qrpt_Qstring("name", s);

#if QT_VERSION >= QT_VERSION_CHECK(5,9,0)
   s = screen->manufacturer(); // 5.9
   qrpt_Qstring("manufacturer", s);
   s = screen->model();  // 5.9
   qrpt_Qstring("model", s);
   s = screen->serialNumber();   // 5.9
   qrpt_Qstring("serialNumber", s);
#endif

   QSizeF sizef = screen->physicalSize();
   qrpt_QSizef("physicalSize", sizef);

   QRect rect = screen->geometry();
   rect = screen->availableGeometry();
   qrpt_QRect("geometry", rect);
   rect = screen->availableVirtualGeometry();
   qrpt_QRect("availableVirtualGeometry", rect);
   rect = screen->virtualGeometry();
   qrpt_QRect("virtualGeometry", rect);

   QSize size = screen->size();
   size = screen->availableSize();
   size = screen->availableVirtualSize();
   size = screen->virtualSize();


   qreal r = screen->devicePixelRatio();
   qrpt_qreal("devicePixelRatio", r);

/* From https://github.com/owncloud/client/issues/5000:
    You can't rely on screen DPI there are a huge bunch of samsung monitors that
    get mm and inches swapped so the auto DPI is huge. I do the Plasma High DPI
    and we don't use this because the feedback we got is that it simply doesn't
    work. Also it messes with Plasma which manually sets the scale factor based
    on the user's settings.

    There is an alternate method to acheive the same result for you.
    qApp->setAttribute(Qt::AA_EnableHighDpiScaling, true);

    This will act the same as:
        qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    if and only if no other env vars are set.
*/
   r = screen->logicalDotsPerInch();   // average of logicalDotsPerInchX, logicalDotsPerInchY
   qrpt_qreal("logicalDotsPerInch", r);
   r = screen->logicalDotsPerInchX();
   qrpt_qreal("logicalDotsPerInchX", r);
   r = screen->logicalDotsPerInchY();
   qrpt_qreal("logicalDotsPerInchY", r);

   r = screen->physicalDotsPerInch();   //average of X, Y
   qrpt_qreal("physicalDotsPerInch", r);
   r = screen->physicalDotsPerInchX();
   qrpt_qreal("physicalDotsPerInchX", r);
   r = screen->physicalDotsPerInchY();
   qrpt_qreal("physicalDotsPerInchY", r);

   int i = screen->depth();   // color depth
   qrpt_int("depth", i);
}


void dbgrpt_hidpiQApplication(QApplication& coreapp) {
   bool b = coreapp.testAttribute(Qt::AA_Use96Dpi);
   printf("AA_Use96Dpi:    %s\n", SBOOL(b));
   b = coreapp.testAttribute(Qt::AA_EnableHighDpiScaling);
   printf("AA_EnableHighDPIscaling:    %s\n", SBOOL(b));
   b = coreapp.testAttribute(Qt::AA_DisableHighDpiScaling);
   printf("AA_DisableHighDPIscaling:    %s\n", SBOOL(b));

   QScreen * primaryScreen = coreapp.primaryScreen();
   // n. will want to use slot primaryScreenChanged(QScreen * screen);
   QString name = primaryScreen->name();
   printf("primaryScreen: %s\n", QS2S(name));

   const auto screens = coreapp.screens();

   for (int ndx = 0; ndx < screens.count(); ndx++) {
      printf("Screen %d:\n", ndx);
      dbgrptQScreen(screens[ndx]);
   }
}


static bool init_ddcutil_library(Parsed_Ddcui_Cmd * parsed_cmd) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting. parsed_cmd=%p\n", __func__, (void*)parsed_cmd);

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

   uint16_t work = 0;
   if (parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE) work |= DDCA_TRCOPT_TIMESTAMP;
   if (parsed_cmd->flags & CMD_FLAG_THREAD_ID_TRACE) work |= DDCA_TRCOPT_THREAD_ID;

   ddca_set_trace_options((DDCA_Trace_Options) work);

      // flunks --permissive
      // ddca_set_trace_options(
      //       ( (parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE) ?  DDCA_TRCOPT_TIMESTAMP : 0) |
      //       ( (parsed_cmd->flags & CMD_FLAG_THREAD_ID_TRACE) ?  DDCA_TRCOPT_THREAD_ID : 0)
      //      );

   ddca_add_trace_groups(parsed_cmd->traced_groups);

   // Must be called before any API call that triggers display identification
   // DDCA_Status rc =  // unused, comment out for now, need to properly set
//    ddca_enable_usb_display_detection(parsed_cmd->flags & CMD_FLAG_NOUSB);

// ddca_enable_udf(              parsed_cmd->flags & CMD_FLAG_ENABLE_UDF);

   if (parsed_cmd->flags & CMD_FLAG_DDCDATA)
      ddca_enable_report_ddc_errors(true);
   if (parsed_cmd->flags & CMD_FLAG_REPORT_FREED_EXCP)
      ddca_enable_error_info(true);

   if (parsed_cmd->enable_force_slave_address == TRIVAL_TRUE)
      ddca_enable_force_slave_address(true);
   else if (parsed_cmd->enable_force_slave_address == TRIVAL_FALSE)
      ddca_enable_force_slave_address(false);

#ifdef USE_CONFIG_FILE
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
      ddca_set_default_sleep_multiplier(parsed_cmd->sleep_multiplier);
   }

   if (parsed_cmd->enable_sleep_suppression != TRIVAL_UNSET) {
      bool val = (parsed_cmd->enable_sleep_suppression == TRIVAL_TRUE) ? true : false;
      ddca_enable_sleep_suppression(val);
   }
#endif

   if (debug)
      printf("(%s) Done.  Returning %s\n", __func__, SBOOL(ok));
   return ok;
}


int main(int argc, char *argv[])
{
    bool debug = false;
    if (debug) {
       printf("(%s) Starting\n", __func__);
       printf("(%s) prgname = %s, application_name = %s\n",
             __func__, g_get_prgname(), g_get_application_name() );
    }
    // must be called before parsed_ddcui_command(), o.w. --help reports libddcutil as name
    // n. also sets application_name
    g_set_prgname("ddcui");

    // will remove any arguments that it recognizes, e.g. --widgetcount
    QApplication application(argc, argv);
    application.setWindowIcon(QIcon(":/icons/ddcui_multires.ico"));

    GPtrArray * errmsgs = g_ptr_array_new_with_free_func(free);
    char ** new_argv = NULL;
    int new_argc = 0;
    char *  combined_config_file_options = NULL;
    char *  config_fn;
    if (debug)
       printf("(%s) Calling apply_config_file()\n", __func__);
    int apply_config_rc = apply_config_file(
                       "ddcui",
                       argc,
                       argv,
                       &new_argc,
                       &new_argv,
                       &combined_config_file_options,
                       &config_fn,
                       errmsgs);
    if (debug) {
       printf("(%s) apply_config_file() returned %d\n", __func__, new_argc);
       printf("(%s) combined_config_file_options=%s, config_fn=%s\n", __func__,
              combined_config_file_options, config_fn);
       printf("(%s) new_argc=%d, new_argv:\n", __func__, new_argc);
       ntsa_show(new_argv);
    }

    if (combined_config_file_options && strlen(combined_config_file_options) > 0)
       printf("Applying ddcui      options from %s: %s\n",
                    config_fn, combined_config_file_options);
    free(combined_config_file_options);

    if (errmsgs->len > 0) {
       printf("Errors reading ddcui configuration file %s:\n", config_fn);
       for (guint ndx = 0; ndx < errmsgs->len; ndx++)
          printf("   %s\n", (char*) g_ptr_array_index(errmsgs, ndx));
    }
    g_ptr_array_free(errmsgs, true);
    free(config_fn);
    if (apply_config_rc < 0)
       return 1;

    Parsed_Ddcui_Cmd * parsed_cmd = parse_ddcui_command(new_argc, new_argv);
    if (!parsed_cmd)
       return 1;

    if (parsed_cmd->flags & CMD_FLAG_SHOW_STYLES) {
       QStringList styles = QStyleFactory::keys();
       printf("Known styles:\n");
       for (int ndx = 0;  ndx < styles.size(); ndx++)
          printf("  %s\n", styles.at(ndx).toLocal8Bit().constData());
       return 0;
    }

    if (parsed_cmd->flags & CMD_FLAG_SHOW_ACTIVE_STYLE) {
       const char * styleClass = QApplication::style()->metaObject()->className();
       printf("Active style: %s\n", styleClass);
    }

    // local, not in ddcutil library
     enable_trace_show_time(     parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE);
     enable_trace_show_thread_id(parsed_cmd->flags & CMD_FLAG_THREAD_ID_TRACE);

    if (!init_ddcutil_library(parsed_cmd))
       return 1;

    if (parsed_cmd->flags & CMD_FLAG_F1) {
       dbgrpt_hidpi_environment_vars();
       dbgrpt_hidpiQApplication(application);
    }

    GlobalState & globalState = GlobalState::instance();
    init_core();

    if (debug)
       printf("(%s) Calling MainWindow constructor\n", __func__);
    MainWindow w(parsed_cmd);
    if (debug)
       printf("(%s) MainWindow constructor completed\n", __func__);
    globalState._mainWindow = &w;
    globalState._application = &application;

    // without w.show(), initial serial message box does not appear over MainWindow
    w.show();

#ifdef PERSISTENT_SERIAL_MSG_BOX
    w.initSerialMsgbox();
    // how to defer until after main event loop started, i.e. a.exec() called
    w.start_msgBoxThread();
#endif

    if (debug)
       printf("(%s) Calling Application::exec()\n", __func__);
    int mainStatus = application.exec();
    if (debug)
       printf("(%s) Application::exec() returned %d\n", __func__, mainStatus);
    ddca_show_stats(parsed_cmd->stats_types,
                    false,              // include_per_thread_data
                    0);                 // depth
    // return mainStatus;
    exit(mainStatus);
}
