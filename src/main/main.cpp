/** \file main.cpp */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <glib-2.0/glib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>

#include <QtCore/QtCore>
#include <QtWidgets/QApplication>

#include <ddcutil_c_api.h>
#include <ddcutil_macros.h>
#include <ddcutil_types.h>

#include "c_util/ddcutil_config_file.h"
#include "c_util/debug_util.h"
#include "c_util/simple_ini_file.h"
#include "c_util/string_util.h"
#include "c_util/xdg_util.h"

#include "cmdline/ddcui_cmd_parser.h"
#include "cmdline/ddcui_parsed_cmd.h"

#include "base/ddcui_core.h"
#include "base/ddcui_parms.h"
#include "base/global_state.h"

#include "main/mainwindow.h"
#include "main/msgbox_thread.h"


// See: https://www.qt.io/blog/2016/01/26/high-dpi-support-in-qt-5-6
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


intmax_t get_thread_id() {
   pid_t tid = syscall(SYS_gettid);
   return tid;
}


void create_timestamp(char* buf, int bufsz) {
   assert(bufsz >= 40);
   time_t epoch_seconds = time(NULL);
   struct tm broken_down_time;
   localtime_r(&epoch_seconds, &broken_down_time);
   strftime(buf, 40, "%b %d %T", &broken_down_time);
}


void display_status_event_callback(DDCA_Display_Status_Event evt) {
   char time_buf[40];
   create_timestamp(time_buf, 40);
   intmax_t thread_id = get_thread_id();
  // printf("(%s) evt.dref=%p event_type=%d\n", __func__, evt.dref, evt.event_type);

  printf("[%s][%6jd](main.cpp/%s) Executing. dref=%s, bus=/dev/i2c-%d, event_type = %s\n",
        time_buf, thread_id, __func__, ddca_dref_repr(evt.dref), evt.io_path.path.i2c_busno, ddca_display_event_type_name(evt.event_type));

  if (evt.dref) {
     printf("[%s][%6jd](main.cpp/%s) ddca_validate_display_ref(%s) reports: %s\n",
           time_buf, thread_id, __func__,
           ddca_dref_repr(evt.dref),
           ddca_rc_name(ddca_validate_display_ref(evt.dref, true)));
  }
}


#ifdef UNUSED
// typedef void (*DDCA_Display_Detection_Callback_Func)(DDCA_Display_Detection_Event);
void display_detection_callback(DDCA_Display_Detection_Event report) {
   char time_buf[40];
   time_t epoch_seconds = time(NULL);
   struct tm broken_down_time;
   localtime_r(&epoch_seconds, &broken_down_time);
   strftime(time_buf, 40, "%b %d %T", &broken_down_time);

   intmax_t thread_id = get_thread_id();

  printf("[%s][%6jd](main.cpp/%s) Executing. dref=%p=%s, operation=%d=%s, busno=%d\n",
        time_buf, thread_id, __func__,
        report.dref, ddca_dref_repr(report.dref),
        report.event_type, ddca_display_event_type_name(report.event_type), report.io_path.path.i2c_busno);
  return;
}
#endif


static bool ddcui_opened_syslog = false;    // global

#ifdef COPIED_FROM_API_BASE
void report_parse_errors0(Error_Info * erec, int depth, int max_depth) {
   char * edesc = psc_text(erec->status_code);

   if (depth == 0)  {
      rpt_vstring(depth, "%s: %s", edesc, erec->detail);
   }
   else {
      rpt_vstring(depth, "%s", erec->detail);
   }
   if (depth < max_depth) {
      if (erec->cause_ct > 0) {
         for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
            Error_Info * cur = erec->causes[ndx];
            report_parse_errors0(cur, depth+1, max_depth);
         }
      }
   }
}


void report_parse_errors(Error_Info * erec) {
   if (erec) {
      rpt_push_output_dest(ferr());
      report_parse_errors0(erec, 0, 3);
      rpt_pop_output_dest();
   }
}
#endif

static bool init_ddcutil_library(Parsed_Ddcui_Cmd * parsed_cmd) {
   bool debug = false;
   if (debug)
      printf("(main.cpp:%s) Starting. parsed_cmd=%p\n", __func__, (void*)parsed_cmd);

   bool ok = true;

   DDCA_Init_Options opts = DDCA_INIT_OPTIONS_NONE;
   if (parsed_cmd->flags & CMD_FLAG_DISABLE_CONFIG_FILE)
      opts = (DDCA_Init_Options) (opts | DDCA_INIT_OPTIONS_DISABLE_CONFIG_FILE);
   if (ddcui_opened_syslog) {
      // so all syslog entries have the same program identifier
      opts = (DDCA_Init_Options) (opts | DDCA_INIT_OPTIONS_CLIENT_OPENED_SYSLOG);
   }
   // opts = (DDCA_Init_Options) (opts | DDCA_INIT_OPTIONS_ENABLE_INIT_MSGS);
   char ** infomsgs = NULL;
   DDCA_Status rc = ddca_init2(parsed_cmd->library_options, ddcui_syslog_level, opts,  &infomsgs );
   if (debug)
      printf("(main.cpp:%s) ddca_init() returned %d\n", __func__, rc);

   // printf("(%s) WOLF 0\n", __func__);
   // if (infomsgs)
   //    ntsa_show(infomsgs);
   // printf("WOLF A\n");

   if (infomsgs) {
      // printf("Null_Terminated_String_Array at %p:\n", (void*) infomsgs);
      int ndx = 0;
      while (infomsgs[ndx]) {
         printf("%s\n", infomsgs[ndx]);
         ndx++;
      }
      // printf("Total entries: %d\n", ndx);
      // g_strfreev(infomsgs);
      ntsa_free(infomsgs, true);
   }


   if (rc) {
     DDCA_Error_Detail * erec = ddca_get_error_detail();
     if (debug)
        ddca_report_error_detail(erec, 1);
     printf("%s\n", erec->detail);
     for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
        printf("   %s\n", erec->causes[ndx]->detail);
     }
     ddca_free_error_detail(erec);
     ok = false;
   }
   else {
      // Must be called before any API call that triggers display identification
      // DDCA_Status rc =  // unused, comment out for now, need to properly set
      // ddca_enable_usb_display_detection(parsed_cmd->flags & CMD_FLAG_NOUSB);
      // ddca_enable_udf(              parsed_cmd->flags & CMD_FLAG_ENABLE_UDF);
   }

   if (ok) {
      if (debug)
         printf("(main.cpp:%s) Registering callback functions..\n", __func__);
         ddca_register_display_status_callback(display_status_event_callback);
   }

   if (debug)
      printf("(main.cpp:%s) Done.  Returning %s\n", __func__, SBOOL(ok));
   return ok;
}


/** Gets the id number of the current process
 *
 *  \return  process number
 */
intmax_t get_process_id()
{
   pid_t pid = syscall(SYS_getpid);
   return pid;
}


int main(int argc, char *argv[])
{
    set_simple_dbgmsg_min_funcname_size(0);

    bool debug = false;
    if (debug) {
       printf("(%s) Starting\n", __func__);
       printf("(%s) prgname = %s, application_name = %s\n",
             __func__, g_get_prgname(), g_get_application_name() );
    }

    bool show_version = ntsa_find(argv, "-V") >= 0 || ntsa_find(argv, "--version") >= 0;
    // printf("(%s) %s\n", __func__, sbool(show_version));
    if (show_version) {
       printf("ddcui %s\n\n", DDCUI_VSTRING);
       printf("Built using libddcutil version %d.%d.%d, Qt version %s\n",
              DDCUTIL_VMAJOR, DDCUTIL_VMINOR, DDCUTIL_VMICRO, QT_VERSION_STR);
       printf("Executing using libddcutil %s, Qt %s\n\n",
              ddca_ddcutil_extended_version_string(), qVersion());
       puts("Copyright (C) 2018-2024 Sanford Rockowitz");
       puts("License GPLv2: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>");
       puts("This is free software: you are free to change and redistribute it.");
       puts("There is NO WARRANTY, to the extent permitted by law.");
       exit(0);
    }

    // bool enable_syslog = true;
    // if ( ntsa_find(argv, "--disable-syslog") >= 0 || ntsa_find(argv, "--nosyslog") >= 0 )
    //    enable_syslog = false;
    ddcui_syslog_level = DDCA_SYSLOG_NOTICE;  // ddcui default
    int syslog_pos = ntsa_find(argv, "--syslog");
    if (syslog_pos >= 0 && syslog_pos < (argc-1)) {
       DBGF(debug, "Parsing initial log level");
       Ddcui_Syslog_Level parsed_level;
       bool ok_level = parse_ddcui_syslog_level(argv[syslog_pos+1], &parsed_level);
       if (ok_level)
          ddcui_syslog_level = parsed_level;
    }
    DBGF(debug, "ddcui_syslog_level = %d", ddcui_syslog_level);

    if (ddcui_syslog_level != DDCA_SYSLOG_NOT_SET && ddcui_syslog_level > DDCA_SYSLOG_NEVER) {
       ddcui_opened_syslog = true;
       openlog("ddcui", LOG_CONS|LOG_PID, LOG_USER);
    }
    // bool emit_syslog_info = ddcui_syslog_level != DDCA_SYSLOG_NOT_SET && ddcui_syslog_level >= DDCA_SYSLOG_INFO;
    if (test_emit_ddcui_syslog(DDCA_SYSLOG_NOTICE))
       syslog(LOG_NOTICE, "Starting");
    // must be called before parsed_ddcui_command(), o.w. --help reports libddcutil as name
    // n. also sets application_name
    g_set_prgname("ddcui");

    int mainStatus = 0;

    DBGF(debug, "Calling QApplication() constructor");
    // will remove any arguments that it recognizes, e.g. --widgetcount
    // As of 9/18/2023, emits msg: QSocketNotifier can only be used with threads started with QThread
    QApplication application(argc, argv);
    DBGF(debug, "QApplication constructor done");
    application.setWindowIcon(QIcon(":/icons/ddcui_multires.ico"));

    GPtrArray * errmsgs = g_ptr_array_new_with_free_func(free);
    char ** new_argv = NULL;
    int     new_argc = 0;
    char *  combined_config_file_options = NULL;
    char *  config_fn = NULL;
    int     apply_config_rc = 0;

    bool skip_config = (ntsa_find(argv, "--noconfig") >= 0 || ntsa_find(argv, "--disable-config-file") >= 0);
    if (skip_config) {
       DBGF(debug, "Skipping config file");
       new_argv = ntsa_copy(argv, true);
       new_argc = argc;
    }
    else {
       DBGF(debug, "Calling apply_config_file()");
       apply_config_rc = apply_config_file(
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

       if (combined_config_file_options && strlen(combined_config_file_options) > 0) {
          printf("ddcui: Options from %s: %s\n",
                       config_fn, combined_config_file_options);
          if (test_emit_ddcui_syslog(DDCA_SYSLOG_NOTICE)) {
                syslog(LOG_NOTICE, "Applying ddcui options from %s: %s",
                      config_fn, combined_config_file_options);
          }
       }
       free(combined_config_file_options);
       bool emit_syslog = test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR);
       if (errmsgs->len > 0) {
          fprintf(stderr,  "(main.cpp) Error(s) reading ddcui configuration from file %s:\n", config_fn);
          if (emit_syslog)
             syslog(LOG_ERR, "(main.cpp) Error(s) reading ddcui configuration from file %s:", config_fn);
          for (guint ndx = 0; ndx < errmsgs->len; ndx++) {
             fprintf(stderr,  "   %s\n", (char*) g_ptr_array_index(errmsgs, ndx));
             if (emit_syslog)
                syslog(LOG_ERR, "   %s",   (char*) g_ptr_array_index(errmsgs, ndx));
          }
       }
       g_ptr_array_free(errmsgs, true);
       free(config_fn);
    }
    if (apply_config_rc < 0) {
       mainStatus = 1;
    }
    else {
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

       if (!init_ddcutil_library(parsed_cmd)) {
          mainStatus = 1;
       }
       else {
          if (parsed_cmd->flags & CMD_FLAG_F1) {
             dbgrpt_hidpi_environment_vars();
             dbgrpt_hidpiQApplication(application);
          }

          GlobalState & globalState = GlobalState::instance();
          init_core();

          DBGF(debug, "Calling MainWindow constructor", __func__);
          MainWindow w(parsed_cmd);
          if (debug)

          DBGF(debug,"MainWindow constructor completed", __func__);
          globalState._mainWindow = &w;
          globalState._application = &application;

          // without w.show(), initial serial message box does not appear over MainWindow
          w.show();

      #ifdef PERSISTENT_SERIAL_MSG_BOX
          w.initSerialMsgbox();
          // how to defer until after main event loop started, i.e. a.exec() called
          w.start_msgBoxThread();
      #endif

          DBGF(debug, "Calling Application::exec()");
          mainStatus = application.exec();
          DBGF(debug, "Application::exec() returned %d", mainStatus);
          ddca_stop_watch_displays(/*wait=*/ false);    // prevent zombie thread
          // ddca_report_locks(0);
          // char spid[40];
          // g_snprintf(spid, 40, "grep %ld /proc/locks", get_process_id() );
          // (void) system(spid);

          ddca_show_stats(parsed_cmd->stats_types,
                          false,              // include_per_thread_data
                          0);                 // depth

          free_parsed_ddcui_cmd(parsed_cmd);   // make valgrind happier
       }
    }
    if (test_emit_ddcui_syslog(DDCA_SYSLOG_NOTICE))
       syslog(LOG_NOTICE, "ddcui done.");

bye:
    exit(mainStatus);
}
