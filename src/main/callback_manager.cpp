// callback_manager.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <glib-2.0/glib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>


#include "base/ddcui_core.h"
#include "base/global_state.h"

#include "c_util/ddcutil_config_file.h"
#include "c_util/debug_util.h"
#include "c_util/simple_ini_file.h"
#include "c_util/string_util.h"
#include "c_util/xdg_util.h"

#include "main/msgbox_thread.h"

#include <ddcutil_c_api.h>

#include "mainwindow.h"

#include "callback_manager.h"

// Static C++11 thread safe way
CallbackManager& CallbackManager::instance() {
   static CallbackManager _instance;
   return _instance;
}


void postEventMsg(DDCA_Display_Status_Event evt) {
   char time_buf[40];
   create_timestamp(time_buf, 40);
   intmax_t thread_id = get_thread_id();
  // printf("(%s) evt.dref=%p event_type=%d\n", __func__, evt.dref, evt.event_type);

  printf("[%s][%6jd](mainwindow.cpp/%s) Executing. dref=%s, event_type = %s\n",
        time_buf, thread_id, __func__, ddca_dref_repr(evt.dref), ddca_display_event_type_name(evt.event_type));

   if (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED || evt.event_type == DDCA_EVENT_DISPLAY_DISCONNECTED) {
      QString qstitle("Display Status Change");
      QMessageBox::Icon icon = QMessageBox::Warning;
      QString qstext;;
      // QString qstext = (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED)
      //                    ?   QString("Display has been connected.   Redetect Displays")
      //                    :   QString("Display has been disconnected.  Redetect displays");
      if (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED)
         qstext = QString("Display connected on %1, bus /dev/i2c-%2.")
                         .arg(evt.connector_name).arg(  evt.io_path.path.i2c_busno) ;
       else {
         qstext = QString("Display disconnected on %1, bus /dev/i2c-%2.\n")
                         .arg(evt.connector_name).arg(evt.io_path.path.i2c_busno);
       }

      MsgBoxQueueEntry* qe = new MsgBoxQueueEntry(qstitle, qstext, icon);
      GlobalState::instance()._msgBoxQueue->put(qe);
      // GlobalState::instance()._mainWindow->on_actionRedetect_triggered();  // creates new window
   }
  else {
     printf("[%s][%6jd](mainwindow.cpp/%s) Ignoring event of type %s\n",
           time_buf, thread_id, __func__, ddca_display_event_type_name(evt.event_type));
  }
}



CallbackManager::CallbackManager() {
   _cls = strdup(metaObject()->className());
}


void CallbackManager::emitDisplayChanged(DDCA_Display_Status_Event evt) {
   bool debug = true;
   TRACECF(debug, "Executing. event_type=%s, dref=%s",
           ddca_display_event_type_name(evt.event_type), ddca_dref_repr(evt.dref));
   emit displayChanged(evt);
}


void display_status_event_callback(DDCA_Display_Status_Event evt) {
   bool debug = false;
   char time_buf[40];
   create_timestamp(time_buf, 40);
   intmax_t thread_id = get_thread_id();
  // printf("(%s) evt.dref=%p event_type=%d\n", __func__, evt.dref, evt.event_type);

   if (debug)
      printf("(%s) Executing. dref=%s, bus=/dev/i2c-%d, event_type = %s\n",
              __func__,
              ddca_dref_repr(evt.dref), evt.io_path.path.i2c_busno,
              ddca_display_event_type_name(evt.event_type));

  if (evt.dref) {
     if (debug)
        printf("(%s) ddca_validate_display_ref(%s) reports: %s\n",
               __func__,
               ddca_dref_repr(evt.dref),
               ddca_rc_name(ddca_validate_display_ref(evt.dref, true)));

     postEventMsg(evt);

     CallbackManager& callbackManager = CallbackManager::instance();
     callbackManager.emitDisplayChanged(evt);

  }
}


void CallbackManager::registerCallbacks(MainWindow * mainwindow) {
   ddca_register_display_status_callback(display_status_event_callback);

   QObject::connect(this,       &CallbackManager::displayChanged,
                    mainwindow, &MainWindow::forDisplayChanged);
}

