/** \file mainwindow.cpp */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <syslog.h>
#include <iostream>

#include <QList>
#include <QThread>
#include <QFont>
#include <QKeyEvent>
#include <QMessageBox>
#include <QWidget>
#include <QShortcut>     // 5.9

#include <ddcutil_c_api.h>
#include <ddcutil_status_codes.h>

#include "base/ddcui_core_aux.h"
#include "base/ddcui_core.h"
#include "base/ddcui_parms.h"
#include "base/global_state.h"
#include "base/monitor.h"
#include "base/nc_values_state.h"
#include "base/user_interface_options_state.h"
#include "base/widget_debug.h"
#include "c_util/debug_util.h"
#include "base/ddcui_rtti.h"

#include "help/help_browser.h"
#include "help/help_dialog.h"

#include "nongui/msgbox_queue.h"
#include "nongui/vcpthread.h"    // includes vcprequest.h

#include "cmdline/ddcui_parsed_cmd.h"

#include "monitor_desc/monitor_desc_actions.h"
#include "monitor_desc/monitor_desc_ui.h"

#include "core_widgets/spin_slider.h"

#include "feature_value_widgets/value_stacked_widget.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/features_scrollarea_contents.h"
#include "feature_scrollarea/features_scrollarea_ui.h"
#include "feature_scrollarea/features_scrollarea_view.h"

#include "option_dialogs/feature_selection_dialog.h"
#include "option_dialogs/user_interface_options_dialog.h"
#include "option_dialogs/nc_values_dialog.h"

#include "action_dialogs/debug_actions_dialog.h"

#include "main/mainwindow_ui.h"
#include "main/msgbox_thread.h"

#include "main/mainwindow.h"

using namespace std;

//
// Handle display change events
//

#ifdef EXECUTES_ON_WRONG_THREAD

void display_status_event_main_callback(DDCA_Display_Status_Event evt) {
   char time_buf[40];
   create_timestamp(time_buf, 40);
   intmax_t thread_id = get_thread_id();
  // printf("(%s) evt.dref=%p event_type=%d\n", __func__, evt.dref, evt.event_type);

  printf("[%s][%6jd](mainwindow.cpp/%s) Executing. dref=%s, event_type = %s\n",
        time_buf, thread_id, __func__, ddca_dref_repr(evt.dref), ddca_display_event_type_name(evt.event_type));

  printf("[%s][%6jd](mainwindow.cpp/%s) ddca_validate_display_ref(%s) reports: %s\n",
        time_buf, thread_id, __func__,
        ddca_dref_repr(evt.dref),
        ddca_rc_name(ddca_validate_display_ref(evt.dref, true)));

#ifdef REF
  QString qsTitle = QString("ddcutil Error");
  QString qsDetail = QString("Invalid Model: %1").arg(parsed_cmd->model);
  QMessageBox::Icon icon = QMessageBox::Warning;
  MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qsTitle, qsDetail, icon);
#ifdef DEFERRED_MSG_QUEUE
  _deferredMsgs.append(qe);     // not needed
#endif
  TRACECF_STARTING(debug, "Pre put, _msgBoxQueue=%p", _msgBoxQueue);
  _msgBoxQueue->put(qe);
#endif

  if (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED || evt.event_type == DDCA_EVENT_DISPLAY_DISCONNECTED) {
     QString qstitle("Display Status Change");
     QMessageBox::Icon icon = QMessageBox::Warning;
     QString qstext;;
     // QString qstext = (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED)
     //                    ?   QString("Display has been connected.   Redetect Displays")
     //                    :   QString("Display has been disconnected.  Redetect displays");
     if (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED)
        qstext = QString("Display connected on %1, bus /dev/i2c-%2.\n\nRedetect displays")
                        .arg(evt.connector_name).arg(  evt.io_path.path.i2c_busno) ;
      else {
        qstext = QString("Display disconnected on %1, bus /dev/i2c-%2.\n\nRedetect displays")
                        .arg(evt.connector_name).arg(evt.io_path.path.i2c_busno);
      }

     MsgBoxQueueEntry* qe = new MsgBoxQueueEntry(qstitle, qstext, icon);
     GlobalState::instance()._msgBoxQueue->put(qe);
     // GlobalState::instance()._mainWindow->on_actionRedetect_triggered();  // creates new window

     MainWindow* mainWindow =    GlobalState::instance()._mainWindow;
     if (evt.event_type == DDCA_EVENT_DISPLAY_CONNECTED) {
        mainWindow->addMonitor(evt.dref);
     }
     else {
        assert (evt.event_type == DDCA_EVENT_DISPLAY_DISCONNECTED);
        mainWindow->removeMonitor(evt.dref);
     }

  }
  else {
     printf("[%s][%6jd](mainwindow.cpp/%s) Ignoring event of type %s\n",
           time_buf, thread_id, __func__, ddca_display_event_type_name(evt.event_type));
  }
}
#endif



/** Find monitor in Monitors array by matching dref
 *
 * @oaran dref  Display reference for monitor being found
 * @return index in Monitors array of monitor found, or -1 if not found
 */
int MainWindow::findMonitor(DDCA_Display_Ref dref) {
   bool debug  = false;
   TRACECF_STARTING(debug, "dref=%s", ddca_dref_repr(dref));
   int result = -1;
   int ct0 = _monitors.size();
   // TRACECF(debug,"_monitors.size() = %d", ct0);
   for (int ndx = _monitors.size()-1; ndx >= 0; ndx--) {
      Monitor * curMonitor = _monitors.at(ndx);
      if (curMonitor->_displayInfo->dref == dref) {
         result = ndx;
         break;
      }
   }
   TRACECF_DONE(debug,"Returning: %d", result);
   return result;
}


/** Find monitor in Monitors array by matching io path
 *
 * @oaran dref  Display reference for monitor being found
 * @return index in Monitors array of monitor found, or -1 if not found
 */
int MainWindow::matchMonitor(DDCA_Display_Ref dref) {
   bool debug  = false;
   TRACECF_STARTING(debug, "dref=%s", ddca_dref_repr(dref));
   int result = -1;
   int ct0 = _monitors.size();
   // TRACECF(debug,"_monitors.size() = %d", ct0);

   DDCA_Display_Info2 * dinfo0 = NULL;
   ddca_get_display_info2(dref, &dinfo0);
   assert(dinfo0);
   DDCA_IO_Path p0 = dinfo0->path;
   for (int ndx = _monitors.size()-1; ndx >= 0; ndx--) {
      Monitor * curMonitor = _monitors.at(ndx);
      DDCA_IO_Path p1 = curMonitor->_displayInfo->path;
      if (ddcu_dpath_eq(p0, p1)) {
         result = ndx;
         break;
      }
   }
   ddca_free_display_info2(dinfo0);
   TRACECF_DONE(debug,"Returning: %d", result);
   return result;
}


/** Called when a new monitor is detected
 *
 * @oaran dref  Display reference for monitor being added
 * @return index in Monitors array of monitor added, or -1 if error
 *
 * Initializes monitor data structures, adds monitor to display selector combo box
 */
int MainWindow::addMonitor(DDCA_Display_Ref dref) {
   bool debug = false;
   TRACECF_STARTING(debug, "dref=%s", ddca_dref_repr(dref));
   int nextIndex = -1;
   DDCA_Display_Info2 * dinfo;
   DDCA_Status ddcrc = ddca_get_display_info2(dref, &dinfo);
   const char * explain = ddca_rc_name(ddcrc);
   TRACECF_NOPREFIX(debug, "ddca_get_display_info2() returned %d %s", ddcrc, explain);
   if (ddcrc != 0) {
      syslog(LOG_ERR, "ddca_get_display_info2() returned %s", explain);
      TRACECF_DONE(debug, "ddca_get_display_info2() failed. Returning -1");
      return -1;
   }
   // initialize monitor data structures, add to display selector combo box
   initOneMonitor(dinfo, nextIndex);
   TRACECF_DONE(debug, "Returning %d", nextIndex);
   return nextIndex;
}


/** Called when a monitor is removed
 *
 *  @oaran dref  Display reference for monitor being removed
 *  @return index in Monitors array of monitor removed, or -1 if not found
 *
 *  - Finds monitor in Monitors array by matching dref
 *  - Locates the monitor in display selector combo box
 *  - Disconnects signals to combo box from base model of monitor being removed
 *  - Deletes monitor from Monitors array
 *  - If current monitor is being removed, selects first monitor in combo box
 *    and sets summary view
 */
int MainWindow::removeMonitor(DDCA_Display_Ref dref) {
   bool debug  = false;
   TRACECF_STARTING(debug, "dref=%s", ddca_dref_repr(dref));

   int monNdx = findMonitor(dref);
   if (monNdx >= 0) {
      Monitor * monitor = _monitors.at(monNdx);
      TRACECF_NOPREFIX(debug, "monitor=%p, dref=%s", monitor, ddca_dref_repr(dref));

      // Remove entry for monitor from display selector combo box.
      // The combo box entries parallel the _monitors array, so the entry to
      // delete is at monNdx.  Do not look it up with findText(): the result
      // is ambiguous if two monitors have the same model name.
      int curIndex = _toolbarDisplayCB->currentIndex();
      int indexToDelete = monNdx;

      // disconnect signals from base model of monitor being removed
      disconnectBaseModel(monitor);
     _toolbarDisplayCB->removeItem(indexToDelete);
      TRACECF_NOPREFIX(debug, "deleting monitor monNdx=%d, monitor=%p, dispno=%d",
                     monNdx, monitor, monitor->_displayInfo->dispno);
      _monitors.removeAt(monNdx);
      delete monitor;
      // TRACECF(debug, "deleted monitor monNdx=%d", monNdx);

      int newCurIndex = -1;
      if (curIndex == indexToDelete) {
         if (_toolbarDisplayCB->count() > 0) {
            _toolbarDisplayCB->setCurrentIndex(0);   // select first monitor
            newCurIndex = 0;
            // emit signalMonitorSummaryView();   // doesn't work
            on_actionMonitorSummary_triggered();
         }
      }
   }
   else {
      TRACECF_NOPREFIX(debug, "No monitor found for dref=%s", ddca_dref_repr(dref));
   }

   TRACECF_DONE(debug, "Returning %d", monNdx);
   return monNdx;
}

/** Called when a monitor becomes enabled
 *
 * @oaran dref  Display reference for monitor being enabled
 */
void MainWindow::enableMonitor(DDCA_Display_Ref dref) {
   bool debug = true;
   char * msg = NULL;
   TRACECF_STARTING(debug, "dref=%s", ddca_dref_repr(dref));
   int monNdx = findMonitor(dref);
   if (monNdx >= 0) {
      Monitor * monitor = _monitors.at(monNdx);
      msg = g_strdup_printf("Enabled monitor %p monNdx=%d", monitor, monNdx);
   }
   else {
      msg = g_strdup_printf("No monitor found for dref=%s", ddca_dref_repr(dref));
   }
   TRACECF_DONE(debug, "%s", msg);
   free(msg);
}


/** Called when a display change event is received
 *
 * @oaran evt  Display change event received
 *
 * - For DDCA_EVENT_DISPLAY_CONNECTED:
 *      adds monitor for display to Monitors array and display selector combo box
 * - For DDCA_EVENT_DISPLAY_DISCONNECTED:
 *      removes monitor for display from Monitors array and display selector combo box
 * - For DDCA_EVENT_DDC_ENABLED:
 *      enables monitor for DDC communication
 */
void MainWindow::forDisplayChanged(DDCA_Display_Status_Event evt) {
   bool debug = false;
   TRACECF_STARTING(debug, "event type: %d = %s, dref=%s",
          evt.event_type, ddca_display_event_type_name(evt.event_type),
          ddca_dref_repr(evt.dref) );

   int newDisplayIndex = -1;
   if (evt.event_type ==  DDCA_EVENT_DISPLAY_CONNECTED ) {
      int monndx = matchMonitor(evt.dref);
      TRACECF_NOPREFIX(debug, "matchMonitor returned %d", monndx);
      if (monndx >= 0) {
         Monitor * monitor = _monitors.at(monndx);
         // monitor->recheck();
         // hack
         removeMonitor(monitor->_displayInfo->dref);
      }
      newDisplayIndex = addMonitor(evt.dref);
   }

   else if (evt.event_type == DDCA_EVENT_DISPLAY_DISCONNECTED) {
      /* int removedIndex = */
      removeMonitor(evt.dref);
      // int curIndex = _toolbarDisplayCB->currentIndex();

   }

   else if (evt.event_type == DDCA_EVENT_DDC_ENABLED) {
      enableMonitor(evt.dref);
   }

   else {
      syslog(LOG_ERR, "Unexpected event type");
      TRACEC_NOPREFIX("Unexpected event type: %d = %s",
             evt.event_type, ddca_display_event_type_name(evt.event_type));
      // assert(false);
   }
   TRACECF_DONE(debug, "newDisplayIndex=%d", newDisplayIndex);
}


//
// Message Box Handling
//

// called from main.cpp before event mainwindow event loop started
void MainWindow::initSerialMsgbox() {
   bool debug  = false;
   TRACEMCF_STARTING(debug, "");
   // QMessageBox for displaying error messages, one at a time
   // using persistent serial message box
   _serialMsgBox = new QMessageBox(this);
   _serialMsgBox->setStandardButtons(QMessageBox::Ok);
   _serialMsgBox->setWindowModality(Qt::WindowModal);
   _serialMsgBox->setFont(_ui->mainMenuFont);

   TRACECF_NOPREFIX(debug, "_msgboxQueue=%p, _msgBoxThread=%p", _msgBoxQueue, _msgBoxThread);

   QObject::connect(_serialMsgBox, &QMessageBox::finished,
                    _msgBoxThread, &MsgBoxThread::msbgoxClosed);

   QObject::connect(_msgBoxThread, &MsgBoxThread::postSerialMsgBox,
                    this,          &MainWindow::showSerialMsgBox);

   // Defer until after MainWindow::show() so that dialog box appears over main window
   // _msgBoxThread->start();

   TRACEMCF_DONE(debug, "");
}

void MainWindow::start_msgBoxThread() {
   bool debug  = false;
   TRACEMCF_STARTING(debug, "");

#ifdef DEFERRED_MSG_QUEUE
   TRACEMC_NOPREFIX("Putting %d MsgBoxQueueEntry on _msgBoxQueue", _deferredMsgs.count());
   for (int ndx = 0; ndx < _deferredMsgs.count(); ndx++) {
      MsgBoxQueueEntry * qe = _deferredMsgs.at(ndx);
    _msgBoxQueue->put(qe);
   }
#endif

   _msgBoxThread->start();
   TRACEMCF_DONE(debug, "_msgBoxThread started");
}


//
// Constructor, Destructor, Initialization
//

void MainWindow::connectBaseModel(Monitor * curMonitor) {
   FeatureBaseModel * baseModel = curMonitor->_baseModel;

   QObject::connect(baseModel,  &FeatureBaseModel::signalStartInitialLoad,
                    this,       &MainWindow::longRunningTaskStart);
   QObject::connect(baseModel,  &FeatureBaseModel::signalEndInitialLoad,
                    this,       &MainWindow::longRunningTaskEnd);
   QObject::connect(baseModel,  &FeatureBaseModel::signalStatusMsg,
                    this,       &MainWindow::setTransitoryStatusMsg);
}

void MainWindow::disconnectBaseModel(Monitor * curMonitor) {
   FeatureBaseModel * baseModel = curMonitor->_baseModel;

   if (baseModel) {
      QObject::disconnect(baseModel,  &FeatureBaseModel::signalStatusMsg,
                          this,       &MainWindow::setTransitoryStatusMsg);
      QObject::disconnect(baseModel,  &FeatureBaseModel::signalStartInitialLoad,
                          this,       &MainWindow::longRunningTaskStart);
      QObject::disconnect(baseModel,  &FeatureBaseModel::signalEndInitialLoad,
                          this,       &MainWindow::longRunningTaskEnd);
   }
}


/** Deletes all monitors in Monitors array
 *
 *  For each monitor:
 *   - disconnects signals from base model
 *  - removes it from the Monitors array and deletes it
 *  Clears the display selector combo box
 *
 *  Called when Redetect Displays is selected from menu, and from destructor
 */
void MainWindow::freeMonitors() {
   bool debug = false;
   TRACECF_STARTING(debug, "");

   int ct0 = _monitors.size();
   TRACECF_NOPREFIX(debug,"_monitors.size() = %d", ct0);
   for (int ndx = _monitors.size()-1; ndx >= 0; ndx--) {
      Monitor * curMonitor = _monitors.at(ndx);
      TRACECF_NOPREFIX(debug, "deleting monitor ndx=%d, curMonitor=%p, dispno=%d",
                     ndx, curMonitor, curMonitor->_displayInfo->dispno);
      _monitors.removeAt(ndx);
      delete curMonitor;
      TRACECF_NOPREFIX(debug, "deleted monitor ndx=%d", ndx);
   }

   QObject::disconnect(_toolbarDisplayCB, SIGNAL(currentIndexChanged(int)),
                       this,              SLOT(  displaySelectorCombobox_currentIndexChanged(int)));

   int ct = _toolbarDisplayCB->count();
   TRACECF_NOPREFIX(debug,"_toolbarDisplayCB->size() = %d", ct);
   for (int ndx = ct-1; ndx >= 0; ndx--) {
      _toolbarDisplayCB->removeItem(ndx);
   }

   TRACECF_DONE(debug, "");
}

void MainWindow::initOneMonitor(DDCA_Display_Info2 * info, int curIndex) {
   bool debug = false;
   TRACECF_STARTING(debug, "info=%p, curIndex=%d", info, curIndex);

   int monitorNumber = curIndex+1;
   _toolbarDisplayCB->addItem(ddcu_comboBoxModelName(info), QVariant(monitorNumber));

   // Create Monitor instance, initialize data structures
   Monitor * curMonitor = new Monitor(info, monitorNumber);

   TRACECF_NOPREFIX(debug, "connecting reportDisconnected");
   QObject::connect(curMonitor, &Monitor::reportDisconnected,
                    this,       &MainWindow::removeMonitor);
   TRACECF_NOPREFIX(debug, "connected reportDisconnected");

   _monitors.append(curMonitor);
   initMonitorInfoWidget(curMonitor, _ui->centralWidget);
   initCapabilitiesWidget(curMonitor, _ui->centralWidget);

   if (curMonitor->supportsDdc()) {
      initFeaturesScrollAreaView(curMonitor, curMonitor->_baseModel, _ui->centralWidget, _msgBoxQueue);

      connectBaseModel(curMonitor);
      // asynchronously get capabilities for current monitor
      // if (debug)
      //    ddca_report_display_info(info, 3);

      curMonitor->_requestQueue->put(new LoadDfrRequest());
      curMonitor->_requestQueue->put(new VcpCapRequest());

      // TODO: disable Capabiltest_emit_ddcui_syslogities and Features Views
      _ui->actionCapabilities->setEnabled(true);
      _ui->actionFeaturesScrollArea->setEnabled(true);
   }
   else {
      _ui->actionCapabilities->setEnabled(false);
      _ui->actionFeaturesScrollArea->setEnabled(false);
   }
   TRACECF_DONE(debug, "");
}

void MainWindow::setInitialDisplayIndex(Parsed_Ddcui_Cmd * parsed_cmd) {
   bool debug = false;
   TRACECF_STARTING(debug, "parsed_cmd=%p", parsed_cmd);
   int initialDisplayIndex = -1;
   if (parsed_cmd->model) {
      QString userModelParm(parsed_cmd->model);
      for (int ndx = 0; ndx < _toolbarDisplayCB->count(); ndx++) {
         QString curName(_toolbarDisplayCB->itemText(ndx));
         // Check if the model name of this monitor matches one specified
         // on the command line.
         TRACECF_NOPREFIX(debug, "ndx=%d, parsed_cmd->model = |%s|, curname = |%s|",
                 ndx, parsed_cmd->model, QS2S(curName));
         if (QString::compare(userModelParm, curName, Qt::CaseInsensitive) == 0) {
            initialDisplayIndex = ndx;
            TRACECF_NOPREFIX(debug, "model found, ndx=%d", ndx);
            break;
         }
      }

      TRACECF_NOPREFIX(debug, "after check model name, initialDisplayIndex = %d", initialDisplayIndex);
      if (initialDisplayIndex < 0) {
         // queue status dialog
         initialDisplayIndex = 0;
         QString qsTitle = QString("ddcui Error");
         QString qsDetail = QString("Invalid Model: %1").arg(parsed_cmd->model);
         QMessageBox::Icon icon = QMessageBox::Warning;
         MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qsTitle, qsDetail, icon);
#ifdef DEFERRED_MSG_QUEUE
         _deferredMsgs.append(qe);     // not needed
#endif
         TRACECF_NOPREFIX(debug, "Pre put, _msgBoxQueue=%p", _msgBoxQueue);
         _msgBoxQueue->put(qe);
      }
   }
   else {
      initialDisplayIndex = 0;
   }
   TRACECF_DONE(debug, "initialDisplayIndex (2) = %d", initialDisplayIndex);

   _toolbarDisplayCB->setCurrentIndex(initialDisplayIndex);
}

void MainWindow::initMonitors(Parsed_Ddcui_Cmd * parsed_cmd) {
    bool debug = false;
    TRACECF_STARTING(debug, "parsed_cmd=%p", parsed_cmd);

    longRunningTaskStart();
    statusBar()->showMessage(QString("Loading display information..."));

    DDCA_Status ddcrc = ddca_get_display_refs(/*include invalid displays=*/true, &_drefs);
    TRACECF_NOPREFIX(debug, "ddca_get_display_refs() returned %d, _drefs=%p", ddcrc, _drefs);
    assert(ddcrc == 0);

    DDCA_Error_Detail * errs = ddca_get_error_detail();
    if (errs) {
       if (debug)
          ddca_report_error_detail(errs, 2);
       QString errMsg(errs->detail);
       // bool permissionsError = false;
       if (errs->cause_ct > 0) {
          for (int ndx = 0; ndx < errs->cause_ct; ndx++) {
              DDCA_Error_Detail * cause = errs->causes[ndx];
              TRACECF_NOPREFIX(debug, "errs->status_code=%d", errs->status_code);
              // if (cause->status_code == -13) {    // -EACCES
              //    permissionsError = true;
              // }
              errMsg.append((ndx == 0) ? ":\n\n" : "\n");
              errMsg.append(cause->detail);
          }
          // if (permissionsError) {
          //    TRACECF(debug, "Appending URL");
          //    errMsg.append("<p>For help, see <a href=http://www.ddcutil.com/permissions >ddcutil permissions</a>");
          // }
       }
       ddca_free_error_detail(errs);

       QString qsTitle = QString("ddcutil Error");
       QString qsDetail = QString(errMsg);
       QMessageBox::Icon icon = QMessageBox::Warning;
       MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qsTitle, qsDetail, icon);
#ifdef DEFERRED_MSG_QUEUE
       _deferredMsgs.append(qe);     // not needed
#endif
       TRACECF_NOPREFIX(debug, "Pre put, _msgBoxQueue=%p", _msgBoxQueue);
       _msgBoxQueue->put(qe);
    } // end, error reporting

    for (_drefs_ct=0; _drefs[_drefs_ct]; _drefs_ct++) {}
    TRACECF_NOPREFIX(debug, "_drefs_ct = %d", _drefs_ct);

    for (int ndx = 0; ndx < _drefs_ct; ndx++) {
        TRACECF_NOPREFIX(debug, "Processing display %d", ndx);
        DDCA_Display_Info2 * dinfo;
        DDCA_Status ddcrc = ddca_get_display_info2(_drefs[ndx], &dinfo);
        if (ddcrc != 0) {
           const char * expl = ddca_rc_name(ddcrc);
           syslog(LOG_ERR, "ddca_get_display_info() returned %s", expl);
           TRACEC_NOPREFIX("ddca_get_display_info() returned %s", expl);
           continue;
        }
        initOneMonitor(dinfo, ndx);
    }

    _ui->actionMonitorSummary->setEnabled(false);
    _ui->actionCapabilities->setEnabled(false);
    _ui->actionFeaturesScrollArea->setEnabled(false);
    if (_drefs_ct > 0) {
       _ui->actionMonitorSummary->setEnabled(true);
    }
    else {
       MsgBoxQueueEntry * qe =
             new MsgBoxQueueEntry("ddcui", QString("No displays detected"), QMessageBox::Warning);
       TRACECF_NOPREFIX(debug, "Pre put, _msgBoxQueue=%p", _msgBoxQueue);
       _msgBoxQueue->put(qe);
    }

#ifdef DEFERRED_MSG_QUEUE
   _deferredMsgs = QList<MsgBoxQueueEntry*>();  // not needed,
#endif
    setInitialDisplayIndex(parsed_cmd);

    connect(_toolbarDisplayCB, SIGNAL(currentIndexChanged(int)),
            this,              SLOT(  displaySelectorCombobox_currentIndexChanged(int)));

#ifdef UNNEEDED
    connect(_toolbarDisplayCB, SIGNAL(activated(int)),
            this,              SLOT(  displaySelectorCombobox_activated(int)));
#endif

    // connect(_toolbarDisplayCB, qOverload<int>::of(&QComboBox::currentIndexChanged),
    //         this               &MainWindow::displaySelectorCombobox_currentIndexChanged);

    connect(this,      &MainWindow::signalMonitorSummaryView,
            this,      &MainWindow::on_actionMonitorSummary_triggered);

    connect(this,      &MainWindow::signalCapabilitiesView,
            this,      &MainWindow::on_actionCapabilities_triggered);

    connect(this,      &MainWindow::signalFeaturesView,
            this,      &MainWindow::on_actionFeaturesScrollArea_triggered);

    // Set message in status bar
    QString msg = QString("Detected ") + QString::number(_drefs_ct) + QString(" displays.");
    statusBar()->showMessage(msg);
    longRunningTaskEnd();

    TRACECF_DONE(debug, "");
}

void MainWindow::quitShortcut() {
   bool debug = false;
   TRACECF_EVENT(debug, "Executing");
   //close();
   TRACECF_NOPREFIX(debug, "Before _application->exit()");
   GlobalState::instance()._application->exit(0);
}

// Extracted from MainWindow constructor for clarity
void MainWindow::startWatchDisplays() {
   bool debug = false;
   TRACECF_STARTING(debug, "");

   bool watching_active = false;
   DDCA_Display_Event_Class event_classes;
   // in case watch thread already started by libddcutil
   DDCA_Status watch_rc = ddca_get_active_watch_classes(&event_classes);
   if (watch_rc  == DDCRC_OK) {
      if ((event_classes & DDCA_EVENT_CLASS_DISPLAY_CONNECTION)) {
        watching_active = true;
      }
      else {
         ddca_stop_watch_displays(true);
         if (test_emit_ddcui_syslog(DDCA_SYSLOG_NOTICE))
            syslog(LOG_NOTICE, "Halted watch thread running without DDCA_EVENT_CLASS_DISPLAY_CONNECTION");
      }
   }
   if (!watching_active) {
      DDCA_Status rc = ddca_start_watch_displays(DDCA_EVENT_CLASS_DISPLAY_CONNECTION);
      if (rc == DDCRC_OK) {
         watching_active = true;
         if (test_emit_ddcui_syslog(DDCA_SYSLOG_NOTICE))
            syslog(LOG_NOTICE, "Started display watch thread with DDCA_EVENT_CLASS_DISPLAY_CONNECTION");
      }
      else {
         if (test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR))
            syslog(LOG_ERR, "Failed to start watch thread.");
         DDCA_Error_Detail * erec = ddca_get_error_detail();
         QString qstext;
         if (erec) {
            qstext = QString(erec->detail);
            ddca_free_error_detail(erec);
         }
         else {
            qstext = QString("Unable to start display watch thread.  Status %1").arg(ddca_rc_desc(rc));
         }

         MsgBoxQueueEntry* qe = new MsgBoxQueueEntry(
               "Failed to start display watch thread",
               qstext,
               QMessageBox::Warning);
         GlobalState::instance()._msgBoxQueue->put(qe);
      }
   }
   if (watching_active) {
      // ddca_register_display_status_callback(display_status_event_main_callback);
      CallbackManager::instance().registerCallbacks(this);
   }
   TRACECF_DONE(debug, "");
}


MainWindow::MainWindow(Parsed_Ddcui_Cmd * parsed_cmd, QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui_MainWindow(this))
    // _ui(new Ui::MainWindow)
    // , PageChangeObserver()1
    {
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACECF_STARTING(debug, "thread = %d", get_thread_id());

    GlobalState& globalState = GlobalState::instance();
    globalState._parsed_cmd = parsed_cmd;  // in case of reinitialization

    // Register metatypes for primitive types here.
    // Metatypes for classes are registered with the class definition.
    qRegisterMetaType<uint8_t>("uint8_t");
    qRegisterMetaType<bool>("bool");       // needed?

    qRegisterMetaType<NcValuesSource>("NcValuesSource");
    qRegisterMetaType<QMessageBox::Icon>("QMessageBox::Icon");

    qRegisterMetaType<DDCA_Display_Status_Event>("DDCA_Display_Status_Event");
    qRegisterMetaType<DDCA_Display_Event_Type>("DDCA_Display_Event_Type");
    qRegisterMetaType<DDCA_Display_Ref>("DDCA_Display_Ref");

    // ComboBox for display selection
    QLabel* toolbarDisplayLabel = new QLabel("&Display:  ");
    toolbarDisplayLabel->setFont(_ui->mainMenuFont);
    _toolbarDisplayCB = new QComboBox();
    _toolbarDisplayCB->setObjectName("displaySelectorCombobox");
    _toolbarDisplayCB->setStyleSheet("background-color:white; color:black;");
    _toolbarDisplayCB->setFont(_ui->mainMenuFont);
    toolbarDisplayLabel->setBuddy(_toolbarDisplayCB);
    _ui->mainToolBar->addWidget( toolbarDisplayLabel);
    _ui->mainToolBar->addWidget( _toolbarDisplayCB);

    _msgBoxQueue =  new MsgBoxQueue();
    _msgBoxThread = new MsgBoxThread(_msgBoxQueue);
    globalState._msgBoxThread = _msgBoxThread;
    globalState._msgBoxQueue  = _msgBoxQueue;
    if (parsed_cmd->flags & CMD_FLAG_WATCH_DISPLAYS) {
       startWatchDisplays();
    }

    _quit_shortcut = new QShortcut(QKeySequence(Qt::Key_Q | Qt::CTRL), this);
    _quit_shortcut->setContext(Qt::ApplicationShortcut);
    connect(_quit_shortcut,   &QShortcut::activated,
             this,            &MainWindow::quitShortcut);

    TRACECF_NOPREFIX(debug, "Before initMonitors()");
    // reportWidgetChildren(ui->centralWidget, "Children of centralWidget, before initMonitors():");
    initMonitors(parsed_cmd);

    // Initialize Options menu
    _feature_selector   = new FeatureSelector(parsed_cmd);
    _otherOptionsState  = new NcValuesState(parsed_cmd);
    _uiOptionsState     = new UserInterfaceOptionsState(parsed_cmd);
    globalState._ncValuesState  = _otherOptionsState;
    globalState._uiOptionsState = _uiOptionsState;
    QObject::connect(
        _uiOptionsState,  &UserInterfaceOptionsState::controlKeyRequired_changed,
        this,             &MainWindow::forControlKeyRequired_changed);
    QObject::connect(
        this,     &MainWindow::featureSelectionChanged,
        this,     &MainWindow::on_actionFeaturesScrollArea_triggered);

     switch (parsed_cmd->view) {
     case VIEW_SUMMARY:
     case VIEW_UNSET:
        _initialView = SummaryView;
        break;
     case VIEW_CAPABILITIES:
        _initialView = CapabilitiesView;
        break;
     case VIEW_FEATURES:
        _initialView = FeaturesView;
        break;
     }

// was deleted by VIEW_PR60, restored for reference
#ifdef BAD   // get dialog box that capabilities incomplete before main screen appears
     if (parsed_cmd->view == VIEW_UNSET || parsed_cmd->view == VIEW_SUMMARY)
        emit signalMonitorSummaryView();
     else if (parsed_cmd->view == VIEW_CAPABILITIES) {
        emit signalCapabilitiesView();
     }
     else {
        assert (parsed_cmd->view == VIEW_FEATURES);
        emit signalFeaturesView();
     }
#endif

     if (_monitors.size() > 0) {
        if (_initialView == SummaryView) {
           TRACECF_NOPREFIX(debug, "_monitors_size=%d. emitting signalMonitorSummaryView", _monitors.size());
           emit signalMonitorSummaryView();
        }
        else {
#ifdef OUT
           // on startup, don't want msg that capabilities not ready and be forced to summary view
           int sleep_millis = 000;  // *** TO BE TUNED ***
           TRACECF_NOPREFIX(debug, "Sleeping %d millis before emitting view signal", sleep_millis);
           QThread::msleep(sleep_millis);
#endif

           if (_initialView == CapabilitiesView) {
              TRACECF_NOPREFIX(debug, "_monitors_size=%d. emitting signalCapabilitiesView", _monitors.size());
              emit signalCapabilitiesView();
           }
           else {
              assert (_initialView == FeaturesView);
              TRACECF_NOPREFIX(debug, "_monitors_size=%d. emitting signalFeaturesView", _monitors.size());
              emit signalFeaturesView();
           }
        }
     }

#ifdef DOESNT_SOLVE_PROBLEM
      // An attempt to address the problem of the Message Box for "early" messages
      // doesn't appear centered over the application.
      // The problem is that DialogBoxes have their own event loop, so can get ahead
      // of the main application.
      // This solution waits until the main application event loop is running
      // before starting the message box thread.
      // Should work, but it didn't.

      QObject::connect(
         this,     &MainWindow::reportApplicationEventLoopStarted,
         this,     &MainWindow::start_msgBoxThread);

     TRACEMC_NOPREFIX("Emitting reportApplicationEventLoopStarted()");
     emit reportApplicationEventLoopStarted();   // will not be delivered until application event loop started
#endif

     QObject::connect(this,       &MainWindow::signalRedetectDisplaysStart,
                      this,       &MainWindow::longRunningTaskStart);
     QObject::connect(this,       &MainWindow::signalRedetectDisplaysEnd,
                      this,       &MainWindow::longRunningTaskEnd);

     TRACECF_DONE(debug, "");
}


MainWindow::~MainWindow()
{
    bool debug = false;
    TRACECF_STARTING(debug, "");
    delete _ui;

    delete _feature_selector;
    delete _otherOptionsState;
    delete _uiOptionsState;

    // Tear down monitors first: halting their VcpThreads can still post
    // messages (e.g. markDisconnected()) to _msgBoxQueue, so the consumer
    // thread must remain running until freeMonitors() completes.
    freeMonitors();

    _msgBoxThread->stop();   // request termination and join
    delete _msgBoxThread;
    _msgBoxThread = nullptr;
    TRACEMCF_DONE(debug, "_msgBoxThread halted");

    free(_drefs);
    TRACECF_DONE(debug, "");
    free((void*) _cls);
}


//
// Control key option for changing feature values
//

// Slot for UserInterfaceOptionsState::controlKeyRequired_changed,
void MainWindow::forControlKeyRequired_changed(bool onoff) {
   bool debug = false;
   TRACECF_STARTING(debug, "onoff=%s", SBOOL(onoff));
   ctrlKeyStatusMsg();
   TRACECF_DONE(debug, "");
}

void MainWindow::ctrlKeyStatusMsg() {
   bool debug = false;
   TRACECF_STARTING(debug, "");
   if (_curView == View::FeaturesView && _uiOptionsState->_controlKeyRequired ) {
      statusBar()->addWidget(_ctlMsg);  // Normal message, no timeout
      _ctlMsg->show();
   }
   else {
      statusBar()->removeWidget(_ctlMsg);
   }
   TRACECF_DONE(debug, "");
}

//
// Status Message slots
//

void MainWindow::setTransitoryStatusMsg(QString msg) {
   bool debug = false;
   // n. do not save QS2S(msg) in a variable, it points into a destroyed temporary
   TRACECF_STARTING(debug, "msg = %s", QS2S(msg));
   statusBar()->showMessage(msg,2000);
   TRACECF_DONE(debug, "");
}

// Report API error in status bar

void MainWindow::reportDdcApiError(QString funcname, int rc) const {
     QString msg = funcname + "() returned " + QString::number(rc) + " - " + ddca_rc_name(rc);
     statusBar()->showMessage(msg);

     // QErrorMessage * emsg;
     // invalid conversion from const QWidget* to QWidget*
     // emsg = new QErrorMessage(this);
     // emsg->showMessage("oy vey");
}

//
// SerialMsgBox slots
//

void MainWindow::showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon) {
   bool debug = false;
   TRACEMCF_STARTING(debug, "text=%s", QS2S(text));
// #ifdef DIALOG_BOX_STILL_ON_SEPARATE_SCREEN
#ifdef NON_PERSISTENT
   QMessageBox * serialMbox2 = new QMessageBox(this);
   serialMbox2->setStandardButtons(QMessageBox::Ok);
   serialMbox2->setWindowModality(Qt::WindowModal);
   serialMbox2->setModal(true);
   serialMbox2->setFont(_ui->mainMenuFont);
#endif
   // _serialMsgBox->setTextFormat(Qt::RichText);
   _serialMsgBox->setText(text);
   _serialMsgBox->setWindowTitle(title);
   _serialMsgBox->setIcon(icon);

#ifdef NON_PERSISTENT
   QObject::connect(
         serialMbox2,   &QMessageBox::finished,
         _msgBoxThread, &MsgBoxThread::msbgoxClosed );
#endif

   _serialMsgBox->exec();
   TRACEMCF_DONE(debug, "After serialMsgBox2->exec() returns.");
// #endif

#ifdef NO
   _serialMsgBox->setText(text);
   _serialMsgBox->setWindowTitle(title);
   _serialMsgBox->setIcon(icon);
   _serialMsgBox->show();
   _serialMsgBox->exec();
#endif

}

//
// Miscellaneous Slots
//

static int longRunningTaskNesting = 0;

// Sets spinning cursor at start of a long running task
// Both a direct call from initMonitors and a slot for FeatureBaseModel
// Appears to have no effect when called because initial 
void MainWindow::longRunningTaskStart() {
   bool debug  = false;
   TRACECF_STARTING(debug, "longRunningTaskNesting=%d", longRunningTaskNesting);
   longRunningTaskNesting++;

   // _spinner->start();
   // _loadingMsgBox->show();
   if (longRunningTaskNesting == 1) {
      statusBar()->showMessage(QString("Please wait..."));
      QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   }
   TRACECF_DONE(debug, "longRunningTaskNesting=%d", longRunningTaskNesting);
}

// Restores normal cursor at the of a long running task
// Both a direct call from initMonitors and a slot for FeatureBaseModel
void MainWindow::longRunningTaskEnd() {
   bool debug  = false;
   TRACECF_STARTING(debug, "longRunningTaskNesting=%d", longRunningTaskNesting);
   if (longRunningTaskNesting > 0)
      longRunningTaskNesting--;
   if (longRunningTaskNesting == 0) {
      statusBar()->clearMessage();
      // _spinner->stop();
      // _loadingMsgBox->hide();
      ctrlKeyStatusMsg();
      QGuiApplication::restoreOverrideCursor();
   }
   else
      TRACECF_NOPREFIX(debug, "longRunningTaskNesting=%d > 0, not clearing status message", longRunningTaskNesting);

   TRACECF_DONE(debug, "longRunningTaskNesting=%d", longRunningTaskNesting);
}

//
// Display-selection combo box slots
//

void MainWindow::displaySelectorCombobox_currentIndexChanged(int index) {
   // printf("(%s::%s) index=%d\n", _cls, __func__, index); fflush(stdout);
   bool debug = false;
   TRACECF_STARTING(debug, "index=%d", index);

   // From index, get the Monitor object
   // if not support DDC, always emit signalMonitorSummaryView
   switch(_curView) {
   case SummaryView:
      emit signalMonitorSummaryView();
      break;
   case CapabilitiesView:
      emit signalCapabilitiesView();
      break;
   case FeaturesView:
      emit signalFeaturesView();
      break;
   case NoView:
      break;
   }

   TRACECF_DONE(debug, "");
}

#ifdef UNNEEDED
void MainWindow::displaySelectorCombobox_activated(int index) {
   printf("(%s::%s) index=%d\n", _cls, __func__, index); fflush(stdout);
}
#endif

//
// View menu slots
//

// MonitorsView

void MainWindow::on_actionMonitorSummary_triggered()
{
    bool debug = false;
    // std::cout << "(MainWindow::on_actionMo_initialViewnitorSummary_triggered()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    TRACEMF_STARTING(debug, "monitorNdx=%d", monitorNdx);

    if (monitorNdx < 0) {
       // _ui->centralWidget->hide();
    }
    else {
       // find Monitor rec
       Monitor * monitor = _monitors[monitorNdx];
       DDCA_Display_Info2 * dinfo =  monitor->_displayInfo;    // &_dlist->info[monitorNdx];
       DDCA_Display_Ref dref = dinfo->dref;
       TRACEMF_NOPREFIX(debug, "monitorNdx (%d), dref=%s", monitorNdx, ddca_dref_repr(dref));

       char * s = MonitorDescActions::capture_display_info_report(dinfo);

       TRACECF_NOPREFIX(debug, "monitor=%p, s=%p -> %s", monitor, s, s);
       QPlainTextEdit * moninfoPlainText = monitor->_moninfoPlainText;
       // int pageno = monitor->_pageno_moninfo;
       moninfoPlainText->setPlainText(s);
       free(s);

       _curView = View::SummaryView;
       _ui->actionRescan->setEnabled(false);
       _ui->actionMonitorSummary->setChecked(true);
       bool b = monitor->supportsDdc();
       _ui->actionCapabilities->setEnabled(b);
       _ui->actionFeaturesScrollArea->setEnabled(b);
       // _ui->centralWidget->setCurrentIndex(pageno);
       _ui->centralWidget->setCurrentWidget(monitor->_page_moninfo);
       _ui->centralWidget->show();
       _initialViewShown = true;
    }
    ctrlKeyStatusMsg();   // clears the message since not Features view
    TRACECF_DONE(debug, "_ui->actionCapabilities->isEnabled()=%s",
                   SBOOL(_ui->actionCapabilities->isEnabled() ));
}

// Checks to be performed before entering alternate views (Capabilities, Features)
bool MainWindow::checkAltViewOk(Monitor * monitor) {
   bool debug  = false;
   TRACECF_STARTING(debug, "dref=%s, valid display %s, _initChecksDone=%s",
         QS2S(monitor->dref_repr()),
         sbool(monitor->supportsDdc()), sbool(monitor->_initChecksDone) );

   DDCA_Display_Info2 * dinfo = monitor->_displayInfo;

   MsgBoxQueueEntry * qe = nullptr;
   if (!monitor->_initChecksDone) {
      QString path = (dinfo->path.io_mode == DDCA_IO_I2C)
                         ? QString("bus /dev/i2c-") + QString::number(dinfo->path.path.i2c_busno)
                         : QString("usb /dev/usb/hiddev" + QString::number(dinfo->path.path.hiddev_devno) );

      QString msg;
      if (!monitor->supportsDdc()) {
         msg = QString("Display %1 on %2 does not support DDC (1)")
               .arg(dinfo->model_name)
               .arg(path);
        //  qe = new MsgBoxQueueEntry("ddcui", msg, QMessageBox::Warning);
      }

      else {
         bool capabilitiesChecked = false;
         if (monitor->capabilitiesCheckComplete()) {
            capabilitiesChecked = true;
         }
         else {
            if (!_initialViewShown) {
               // on startup, be patient
               TRACECF_NOPREFIX(debug, "Capabilities check not complete, but initial view not yet shown, waiting");
               int max_wait_millisec = 1000;
               int waited_millisec = 0;
               while (!(capabilitiesChecked=monitor->capabilitiesCheckComplete()) &&
                      waited_millisec < max_wait_millisec)
               {
                  QThread::msleep(100);   // wait a bit for capabilities check to finish
                  waited_millisec += 100;
               }
               TRACECF_NOPREFIX(debug, "After wait, capabilitiesChecked=%s after %d millisec",
                           SBOOL(capabilitiesChecked), waited_millisec);
            }
         }

         if (!capabilitiesChecked) {
            msg = QString("Capabilities check still in progress for display %1 on %2")
                    .arg(dinfo->model_name)
                    .arg(path);
            // qe = new MsgBoxQueueEntry("ddcui", msg, QMessageBox::Warning);
         }

         else {
            bool b = monitor->capabilitiesCheckSuccessful();
            _ui->actionCapabilities->setEnabled(b);
            _ui->actionFeaturesScrollArea->setEnabled(b);
            if (!b) {
               msg = QString("Display %1 on %2 does not report Capabilities (2)")
                  .arg(dinfo->model_name)
                  .arg(path);
               // qe = new MsgBoxQueueEntry("ddcui", msg, QMessageBox::Warning);
            }
         }
      }

      if (!msg.isNull()) {
         qe = new MsgBoxQueueEntry("ddcui", msg, QMessageBox::Warning);
   #ifdef DEFERRED_MSG_QUEUE
         _deferredMsgs.append(qe);     // not needed
   #endif
         TRACECF_NOPREFIX(debug, "Pre put, _msgBoxQueue=%p", _msgBoxQueue);
         _msgBoxQueue->put(qe);
      }
      monitor->_initChecksDone = true;
   }

   TRACECF_DONE(debug, "Returning %s", sbool(!qe));
   return !qe;
}

// CapabilitiesView

void MainWindow::on_actionCapabilities_triggered()
{
    bool debug  = false;
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    TRACECF_STARTING(debug, "monitorNdx=%d", monitorNdx);
    if (monitorNdx < 0) {
       TRACECF_DONE(debug, "monitorNdx = %d is < 0, ignoring", monitorNdx);
    }
    else {
       Monitor * monitor = _monitors.at(monitorNdx);
       DDCA_Display_Info2 * dinfo = monitor->_displayInfo; // &_dlist->info[monitorNdx];
       DDCA_Display_Ref dref = dinfo->dref;
       char * caps_report = NULL;
       if (!checkAltViewOk(monitor)) {
          // checkAltViewOk handles error reporting
          // emit signalMonitorSummaryView();   // doesn't work
          on_actionMonitorSummary_triggered();
       }
       else {
          DDCA_Status ddcrc = MonitorDescActions::capture_capabilities_report(monitor, dref, &caps_report);
          if (ddcrc != 0) {
              reportDdcApiError("ddca_open_display", ddcrc);
              TRACEC_NOPREFIX("capture_capabilites_report returned %d", ddcrc);
          }
          else {
              // cout << "Parsed capabilities: " << endl;
              // cout << caps_report << endl;

              Monitor * monitor = _monitors[monitorNdx];
              QPlainTextEdit * capabilitiesPlainText = monitor->_capabilitiesPlainText;
              // int pageno = monitor->_pageno_capabilities;
              capabilitiesPlainText->setPlainText(caps_report);
              free(caps_report);

              // show widget
              _curView = View::CapabilitiesView;
              _ui->actionRescan->setEnabled(false);
              _ui->actionCapabilities->setChecked(true);
              // _ui->centralWidget->setCurrentIndex(pageno);    // need proper constants
              _ui->centralWidget->setCurrentWidget(monitor->_page_capabilities);
              _ui->centralWidget->show();
              _initialViewShown = true;
          }
       }
    }
    ctrlKeyStatusMsg();   // clears the message since not Features view
    TRACECF_DONE(debug, "");
}

// FeaturesView

void MainWindow::on_actionFeaturesScrollArea_triggered()
{
   bool debug  = false;
   debug = debug || debugFeatureSelection;
    if (debug) {
        TRACEC_STARTING("Desired view: %d, feature list:", View::FeaturesView);
        this->_feature_selector->dbgrpt();
    }

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    if (monitorNdx < 0) {
       TRACECF_NOPREFIX(debug, "monitorNdx = %d is < 0, ignoring", monitorNdx);
    }
    else {
       Monitor * monitor = _monitors[monitorNdx];
       if (debug) {
           TRACEC_NOPREFIX("Current view: %d, feature list:", _curView);
           monitor->_curFeatureSelector.dbgrpt();
       }

       if (!checkAltViewOk(monitor)) {
          // checkAltViewOk handles error reporting
          // emit signalMonitorSummaryView();   // doesn't work
          on_actionMonitorSummary_triggered();
       }

       else {
          // TODO Combine View, features view
          if (_curView                     != View::FeaturesView  ||
              _curDisplayIndex             != monitorNdx          ||
              monitor->_curFeatureSelector != *_feature_selector )
          {
             loadMonitorFeatures(monitor);
             _curDisplayIndex = monitorNdx;
             _curView = View::FeaturesView;
             _ui->actionRescan->setEnabled(true);
             _ui->actionFeaturesScrollArea->setChecked(true);
             monitor->_curFeatureSelector   = *_feature_selector;
          }
          else {
             TRACECF_NOPREFIX(debug, "Unchanged view and feature set, no need to load");
          }
          _initialViewShown = true;   // right location?
       }
    }
    ctrlKeyStatusMsg();
    TRACECF_DONE(debug, "");
}

void MainWindow::loadMonitorFeatures(Monitor * monitor) {
   bool debug = false;
   debug |= debugFeatureLists;
    TRACECF_STARTING(debug, "monitor=%p", monitor);
    // monitor->dbgrpt();
    QString msg = QString("Reading monitor features...");
    _ui->statusBar->showMessage(msg);

    DDCA_Feature_List featuresToShow = DDCA_EMPTY_FEATURE_LIST;
    if (_feature_selector->_featureSubsetId == DDCA_SUBSET_CAPABILITIES) {
       featuresToShow = ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);
    }
    else if (_feature_selector->_featureSubsetId == DDCA_SUBSET_CUSTOM) {
       featuresToShow = _feature_selector->_customFeatureList;
    }
    else {
       featuresToShow = monitor->getFeatureList(_feature_selector->_featureSubsetId);
       TRACECF_NOPREFIX(debug,
           "features_to_show: (%d) %s", ddca_feature_list_count(featuresToShow),
                                        ddca_feature_list_string(featuresToShow, NULL, (char*)" "));
       if (_feature_selector->_includeOnlyCapabilities || _feature_selector->_includeAllCapabilities) {
          // need to test _parsed_caps is valid
          // n. simply manipulates data structures, does not perform monitor io
          DDCA_Feature_List caps_features =
                ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);
          TRACECF_NOPREFIX(debug,
              "Capabilities features: (%d) %s",
              ddca_feature_list_count(caps_features),
              ddca_feature_list_string(caps_features, NULL, (char*)" "));
          if (_feature_selector ->_includeOnlyCapabilities)
             featuresToShow = ddca_feature_list_and(featuresToShow, caps_features);
          else
             featuresToShow = ddca_feature_list_or(featuresToShow, caps_features);
       }
    }

    TRACECF_NOPREFIX(debug,
        "Final featuresToShow: (%d) %s",
        ddca_feature_list_count(featuresToShow),
        ddca_feature_list_string(featuresToShow, NULL, (char*)" "));

    // causes async feature reads in VcpThread, then load feature values from model into widgets
    monitor->_baseModel->setFeatureList(featuresToShow);

    TRACECF_DONE(debug, "");
}

//
// *** Actions Menu Slots
//

// Actions->Redetect

// redetect displays
void MainWindow::on_actionRedetect_triggered() {
   bool debug  = false;
   TRACECF_STARTING(debug, "");
   // longRunningTaskStart();
   emit signalRedetectDisplaysStart();

   this->freeMonitors();
   free(_drefs);
   // ddca_free_display_info_list(_dlist);
   // _dlist = NULL;

   TRACECF_NOPREFIX(debug, "Before ddca_redetect_displays");
   DDCA_Status ddcrc = ddca_redetect_displays();
   assert(ddcrc == 0);     // always returns 0
   _ui->actionMonitorSummary->setEnabled(false);
   _ui->actionCapabilities->setEnabled(false);
   _ui->actionFeaturesScrollArea->setEnabled(false);

   TRACECF_NOPREFIX(debug, "Before initMonitors()");
   this->initMonitors(GlobalState::instance()._parsed_cmd);
   TRACECF_NOPREFIX(debug, "After initMonitors()");

   // reinit UI to first monitor, summary view
   // if no monitors, set _curDisplayIndex = -1
   _curDisplayIndex = (_drefs_ct > 0) ? 0 : -1;

   // HANDLE CASE OF NO DDC MONITORS?
   emit signalMonitorSummaryView();

   // longRunningTaskEnd();
   emit signalRedetectDisplaysEnd();
   TRACECF_DONE(debug,"");
}

// Actions->Rescan

// rescan features for current monitor
void MainWindow::on_actionRescan_triggered() {
   bool debug = false;
   TRACECF_EVENT(debug, "Executing");
   assert(_curView == FeaturesView);
   assert(_curDisplayIndex >= 0);
   _monitors[_curDisplayIndex]->_baseModel->reloadFeatures();
}

// Actions->Execution Statistics

void MainWindow::for_resetStats_triggered() {
   bool debug = false;
   TRACECF_STARTING(debug, "triggered");
   ddca_reset_stats();
   TRACECF_DONE(debug, "");
}

void MainWindow::for_reportStats_triggered(DDCA_Stats_Type stats_type, bool show_thread_data) {
   bool debug = false;
   TRACECF_STARTING(debug, "triggered. stats_type = %d", stats_type);
   // TO DO: Make per/thread setting a checkbox on dialog
   // ddca_show_stats(stats_type, show_thread_data, 0);
   capture_stats(stats_type, show_thread_data);
   TRACECF_DONE(debug, "");
}

void MainWindow::showCapturedText(QString windowTitle, QString text) {
   const QFont& textFont = QFont(       "Monospace",  9, QFont::Normal);
   // viewHelpByTextX(qs, QString("Statistics Report"), textFont,  this);
   HelpDialog* hd = new HelpDialog(this);
   hd->setFont(textFont);
   hd->setText(text);
   hd->resize(650,40);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( windowTitle );
   hd->exec();     // always modal
}

void MainWindow::capture_stats(DDCA_Stats_Type stats_type, bool show_thread_data) {
       ddca_start_capture(DDCA_CAPTURE_NOOPTS);
       // DDCA_Output_Level saved_ol = ddca_get_output_level();
       // ddca_set_output_level(DDCA_OL_VERBOSE);
       ddca_show_stats(stats_type, show_thread_data, 0);
       // ddca_set_output_level(saved_ol);
       char * s = ddca_end_capture();
       QString qs(s);
       free(s);
       showCapturedText("Execution Statistics", qs);
#ifdef MOVED
       const QFont& textFont = QFont(       "Monospace",  9, QFont::Normal);
       // viewHelpByTextX(qs, QString("Statistics Report"), textFont,  this);
       HelpDialog* hd = new HelpDialog(this);
       hd->setFont(textFont);
       hd->setText(qs);
       hd->resize(650,40);
          // hd->_textBrowser->setSource(fn);
       hd->setWindowTitle( "Execution Statistics" );
       hd->exec();     // always modal
#endif
}

// Actions->Debug Locks

void MainWindow::on_actionDebugLocks_triggered() {
   bool debug = false;
   TRACECF_STARTING(debug, "");
   ddca_report_locks(0);
   captureLocks();
   TRACECF_DONE(debug,"");
}

void MainWindow::captureLocks() {
   bool debug = false;
   TRACECF_STARTING(false, "");

   ddca_start_capture(DDCA_CAPTURE_NOOPTS);
   // DDCA_Output_Level saved_ol = ddca_get_output_level();
   // ddca_set_output_level(DDCA_OL_VERBOSE);
   ddca_report_locks(0);
   // ddca_set_output_level(saved_ol);
   char * s = ddca_end_capture();
   QString qs(s);
   free(s);
   showCapturedText("Execution Statistics", qs);

   TRACECF_DONE(debug, "");
}

// Actions->Debug: DebugActionsDialog slots

void MainWindow::on_actionDebugActionsDialog_triggered()
{
   // bool debug = false;
   // TODO: allocate once and save dialog, cf feature selection
   // display dialog box for selecting features

   // TRACECF_STARTING(debug, "triggered");
   // assert(false);  // for testing

   DebugActionsDialog* dialog = new DebugActionsDialog(this);
   QObject::connect(dialog, &DebugActionsDialog::resetStats_triggered,
                    this,   &MainWindow::for_resetStats_triggered);
   QObject::connect(dialog, &DebugActionsDialog::reportStats_triggered,
                    this,   &MainWindow::for_reportStats_triggered);
   dialog->exec();
   delete dialog;
}

//
// Options Menu Slots
//

// Feature Selection slots: FeatureSelectionDialog

void MainWindow::on_actionFeatureSelectionDialog_triggered()
{
   bool debug = false;
   TRACECF_STARTING(debug, "fsd=%p", _fsd);

    // FeatureSelectionDialog*
   if (_fsd) {
       _fsd->useSelectorData();
   }
   else {
        _fsd = new FeatureSelectionDialog(this, this->_feature_selector);
       QObject::connect(_fsd,     &FeatureSelectionDialog::featureSelectionChanged,
                        this,     &MainWindow::for_actionFeatureSelectionDialog_accepted);
    }
    _fsd->exec();
  //   delete _fsd;
    TRACECF_DONE(debug, "");
}

// named "for_action..." instead of "on_action..." to avoid the connectSlotsByName naming convention
// FeatureSelectionDialog not allocated at time connectSlotsByName() called, must use
// explicit connect()
void MainWindow::for_actionFeatureSelectionDialog_accepted()
{
   bool debugFunc = false;
   debugFunc = debugFunc || debugSignals || debugFeatureSelection;
   TRACECF_STARTING(debugFunc, "triggered");
   if (debugFunc) {
       _feature_selector->dbgrpt();
   }
   if (_curView == FeaturesView) {
      TRACECF_NOPREFIX(debugFunc, "in FeaturesView, signaling featureSelectionChanged()");
      emit featureSelectionChanged();
   }
   else {
      TRACECF_NOPREFIX(debugFunc, "Not in FeaturesView, so not signaling featureSelectionChanged()");
   }

#ifdef UNNEEDED
   // TODO need semaphore - wait until monitors initialized
   for(int ndx = 0; ndx < _monitors.size(); ndx++) {

       Monitor * monitor = _monitors[ndx];
       FeatureSelector fsel = monitor->_curFeatureSelector;
       bool showUnsupported = fsel._showUnsupportedFeatures;
       VcpRequestQueue * curQueue = monitor->_requestQueue;
       VcpRequest request;
       curQueue->put(request);
   }
#endif

   TRACECF_DONE(debugFunc, "");
}

#ifdef UNUSED
DDCA_Feature_Subset_Id MainWindow::feature_list_id() const {
    return this->_feature_list_id;
}

void MainWindow::set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id) {
    cout << "(set_feature_list_id) feature_list_id =" << feature_list_id <<endl;
    this->_feature_list_id = feature_list_id;
}
#endif

// Options->OtherOptions slots

void MainWindow::on_actionOtherOptionsDialog_triggered()
{
   bool debug = false;
   // TODO: allocate once and save dialog, cf feature selection
   // display dialog box for selecting features
   TRACECF_STARTING(debug, "triggered");

   NcValuesDialog* dialog = new NcValuesDialog(this->_otherOptionsState, this);
   // QObject::connect(dialog,   &OtherOptionsDialog::ncValuesSourceChanged,
   //                   this,    &MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
   // QObject::connect(_otherOptionsState,   &OtherOptionsState::ncValuesSourceChanged,
   //                  this,    &MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
   dialog->exec();
   delete dialog;

   TRACECF_DONE(debug, "");
}

#ifdef OLD
// named for_ .. instead of on_ so that connectSlotsByName doesn't report this as slot
// for which it could find no signal
void MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged(
                     NcValuesSource valuesSource, bool useLatestNcValueNames )
{
   bool debug = false;
   TRACECF_STARTING(debug, "valuesSource=%d, useLatestNcValueNames=%s", valuesSource, SBOOL(useLatestNcValueNames));

   if (_curView == FeaturesView  )   {  // need also check if  FeaturesScrollAreaView
      int monitorNdx = _toolbarDisplayCB->currentIndex();
      Monitor * monitor = _monitors[monitorNdx];
      // or emit signal?
      monitor->_featuresScrollAreaView->onNcValuesSourceChanged(valuesSource, useLatestNcValueNames);
   }
   TRACECF_DONE(debug, "");
}
#endif

// Options->User Interface Options slots: UserInterfaceOptionsDialog

// causes the dialog to display
void MainWindow::on_actionUserInterfaceOptionsDialog_triggered()
{
   bool debug = false;
   TRACECF_STARTING(debug, "uid=%p", _uid);

#ifdef NO  // don't bother keeping the dialog box around and hidden
   if (_uid) {
       _uid->_state = new UserInterfaceOptionsState();
   }
   else {
        _uid = new UserInterfaceOptionsData(this, this->_userInterfaceOptionsData);
       QObject::connect(_uid,     &UserInterfaceOptionsDialog::userInterfaceOptionsChanged,
                        this,     &MainWindow::for_UserInterfaceOptionsDialog_accepted);
    }
    _uid->TRACECF_NOPREFIX();
    //   delete _uid;
#endif

    UserInterfaceOptionsDialog* dialog = new UserInterfaceOptionsDialog(this->_uiOptionsState, this);
    QObject::connect(dialog,   &UserInterfaceOptionsDialog::accepted,
                     this,     &MainWindow::for_actionUserInterfaceOptionsDialog_accept);
    // need a connection for reset?

    TRACECF_NOPREFIX(debug, "Calling setControKeyRequired(%s)", SBOOL(_uiOptionsState->_controlKeyRequired) );
    dialog->setDialogBoxControlKeyRequired( _uiOptionsState->_controlKeyRequired);

    dialog->exec();
    delete dialog;

    TRACECF_DONE(debug, "");
}

void MainWindow::for_actionUserInterfaceOptionsDialog_accept()
{
   bool debug = false;
   TRACECF_EVENT(debug, "Emitting userIntefaceOptionsChanged");
   emit userInterfaceOptionsChanged();
}

//
// Help Menu Slots
//

void MainWindow::on_actionContentsHelp_triggered()
{
    // TRACEC_EVENT("Executing");
    HelpBrowser::showPage(QString("qrc:/docs/help_general.html"), /*navigable=*/ true);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionAbout_triggered()
{
   bool debug = false;
   TRACECF_STARTING(debug, "triggered");
    QString ddcutil_version = ddca_ddcutil_version_string();
    uint8_t build_opts = ddca_build_options();
    // QString ans1 = (build_opts & DDCA_BUILT_WITH_ADL) ? "true" : "false";
    QString ans2 = (build_opts & DDCA_BUILT_WITH_USB) ? "true" : "false";
    QString ans3 = (ddca_is_udf_enabled()) ? "true" : "false";
    QString libname = ddca_libddcutil_filename();

    // QMessageBox mbox;
    // mbox.setText("About ddcutil");

    QString copyright = "Copyright (C) 2014-2026 Sanford Rockowitz\n";
    copyright = copyright + "Licensed under the terms of the GNU General Public License (GPL) ";
    copyright = copyright + "Version 2.0 or later.";

    QString msg = "";
    msg = msg + "ddcui version:    " + ddcui_version   + "\n";
    msg = msg + "   User defined features enabled:      " + ans3 + "\n\n";
    msg = msg + "Using shared library:\n   " + libname + "\n\n";
    msg = msg + "ddcutil version:  " + ddcutil_version + "\n";
 // msg = msg + "   Built with support for ADL connected monitors: " + ans1 + "\n";
    msg = msg + "   Built with support for USB connected monitors: " + ans2;
    msg = msg + "\n\n";
    msg = msg + copyright;

    // mbox.setInformativeText(msg);
    // mbox.exec();
    // QMessageBox::information(this, "..", msg);
    QMessageBox::about(this, "About ddcui", msg);

    TRACECF_DONE(debug, "");
}

//
// Slots related to control key events
//

void MainWindow::keyPressEvent(QKeyEvent *   ev) {
   bool debug = false;
   TRACEMCF_STARTING(debug, "Executing");
   if (debug)
      dbgrptQKeyEvent(ev);

   if (ev->key() == Qt::Key_Control)  {   // 68
      _ctrl_key_is_pressed = true;
      TRACEMCF_NOPREFIX(debug, "Control key recognized. Emitting signalControlKeyPressed(true)");
      signalControlKeyPressed(true);
   }
   QMainWindow::keyPressEvent(ev);
   ev->ignore();

   TRACECF_DONE(debug, "");
}

void MainWindow::keyReleaseEvent(QKeyEvent *   ev) {
   bool debug = false;
   TRACEMCF_STARTING (debug, "");
   if (debug)
      dbgrptQKeyEvent(ev);

   if (ev->key() == Qt::Key_Control) {   // 68
      _ctrl_key_is_pressed = false;
      TRACEMCF_NOPREFIX(debug, "Control key recognized. Emitting signalControlKeyPressed(false)");
      signalControlKeyPressed(false);
   }
   QMainWindow::keyReleaseEvent(ev);
   ev->ignore();

   TRACECF_DONE(debug, "");
}

//
// Archived Unused Slots
//

#ifdef UNUSED
void MainWindow::pageChanged(int pageno) {
    printf("(%s::%s) pageno: %d\n", _cls, __func__, pageno); fflush(stdout);
   //  std::cout << "    objectName: " << objectName.toStdString() << std::endl;
    showCentralWidgetPage(pageno);
}

void MainWindow::pageChangedByWidget(QWidget * widget) {
    printf("(%s::%s) widget=%p\n", _cls, __func__, widget); fflush(stdout);
   //  std::cout << "    objectName: " << objectName.toStdString() << std::endl;
    showCentralWidgetByWidget(widget);
}
#endif

#ifdef UNUSED
void MainWindow::on_vcpTableView_clicked(const QModelIndex &index)
{
    printf("-------------> (MainWindow::on_vcpTableView_clicked) row=%d, col=%d\n", index.row(), index.column() );
}

void MainWindow::on_vcpTableView_doubleClicked(const QModelIndex &index)
{
     printf("----------> (MainWindow::on_vcpTableView_doubleClicked) row=%d, col=%d\n", index.row(), index.column() );
}
#endif


void init_mainwindow() {
   bool debug = false;
   DBGF(debug, "Starting");
   RTTI_ADD_METHOD(MainWindow::findMonitor);
   RTTI_ADD_METHOD(MainWindow::matchMonitor);
   RTTI_ADD_METHOD(MainWindow::addMonitor);
   RTTI_ADD_METHOD(MainWindow::removeMonitor);
   RTTI_ADD_METHOD(MainWindow::enableMonitor);
   RTTI_ADD_METHOD(MainWindow::forDisplayChanged);
   RTTI_ADD_METHOD(MainWindow::initSerialMsgbox);
   RTTI_ADD_METHOD(MainWindow::start_msgBoxThread);
   RTTI_ADD_METHOD(MainWindow::freeMonitors);
   RTTI_ADD_METHOD(MainWindow::initOneMonitor);
   RTTI_ADD_METHOD(MainWindow::setInitialDisplayIndex);
   RTTI_ADD_METHOD(MainWindow::initMonitors);
   RTTI_ADD_METHOD(MainWindow::quitShortcut);
   RTTI_ADD_METHOD(MainWindow::startWatchDisplays);
   RTTI_ADD_METHOD(MainWindow::startRedetectDisplays);
   RTTI_ADD_METHOD(MainWindow::MainWindow);
   RTTI_ADD_METHOD(MainWindow::~MainWindow);
   RTTI_ADD_METHOD(MainWindow::forControlKeyRequired_changed);
   RTTI_ADD_METHOD(MainWindow::ctrlKeyStatusMsg);
   RTTI_ADD_METHOD(MainWindow::setTransitoryStatusMsg);
   RTTI_ADD_METHOD(MainWindow::showSerialMsgBox);
   RTTI_ADD_METHOD(MainWindow::longRunningTaskStart);
   RTTI_ADD_METHOD(MainWindow::longRunningTaskEnd);
   RTTI_ADD_METHOD(MainWindow::displaySelectorCombobox_currentIndexChanged);
   RTTI_ADD_METHOD(MainWindow::on_actionMonitorSummary_triggered);
   RTTI_ADD_METHOD(MainWindow::checkAltViewOk);
   RTTI_ADD_METHOD(MainWindow::on_actionCapabilities_triggered);
   RTTI_ADD_METHOD(MainWindow::on_actionFeaturesScrollArea_triggered);
   RTTI_ADD_METHOD(MainWindow::loadMonitorFeatures);
   RTTI_ADD_METHOD(MainWindow::on_actionRedetect_triggered);
   RTTI_ADD_METHOD(MainWindow::on_actionRescan_triggered);
   RTTI_ADD_METHOD(MainWindow::for_resetStats_triggered);
   RTTI_ADD_METHOD(MainWindow::for_reportStats_triggered);
   RTTI_ADD_METHOD(MainWindow::on_actionDebugLocks_triggered);
   RTTI_ADD_METHOD(MainWindow::captureLocks);
   RTTI_ADD_METHOD(MainWindow::on_actionFeatureSelectionDialog_triggered);
   RTTI_ADD_METHOD(MainWindow::for_actionFeatureSelectionDialog_accepted);
   RTTI_ADD_METHOD(MainWindow::on_actionOtherOptionsDialog_triggered);
   RTTI_ADD_METHOD(MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
   RTTI_ADD_METHOD(MainWindow::for_actionUserInterfaceOptionsDialog_accept);
   RTTI_ADD_METHOD(MainWindow::on_actionUserInterfaceOptionsDialog_triggered);
   RTTI_ADD_METHOD(MainWindow::on_actionAbout_triggered);
   RTTI_ADD_METHOD(MainWindow::keyPressEvent);
   RTTI_ADD_METHOD(MainWindow::keyReleaseEvent);
   DBGF(debug, "Done");
}
