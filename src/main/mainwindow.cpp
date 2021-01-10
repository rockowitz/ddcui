/** \file mainwindow.cpp */

// Copyright (C) 2018-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "main/mainwindow.h"

#include <assert.h>
#include <iostream>

#include <QtCore/QList>
#include <QtCore/QThread>
#include <QtGui/QFont>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

#include <ddcutil_c_api.h>

#include "base/core.h"
#include "base/global_state.h"
#include "base/widget_debug.h"
#include "help/help_dialog.h"
#include "help/help_browser.h"

#include "base/ddcui_parms.h"
#include "base/global_state.h"
#include "base/monitor.h"
#include "base/other_options_state.h"

#include "nongui/msgbox_queue.h"
#include "nongui/vcpthread.h"    // includes vcprequest.h

#include "monitor_desc/monitor_desc_actions.h"
#include "monitor_desc/monitor_desc_ui.h"

#include "feature_value_widgets/value_stacked_widget.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/features_scrollarea_contents.h"
#include "feature_scrollarea/features_scrollarea_ui.h"
#include "feature_scrollarea/features_scrollarea_view.h"

#include "option_dialogs/feature_selection_dialog.h"
#include "option_dialogs/other_options_dialog.h"
#include "option_dialogs/user_interface_options_dialog.h"

#include "action_dialogs/debug_actions_dialog.h"

#include "main/mainwindow_ui.h"
#include "main/msgbox_thread.h"
#include "main/mainwindow.h"

using namespace std;

//
// Constructor, Destructor, Initialization
//

#ifdef NON_PERSISTENT
   QMessageBox * serialMbox2 = new QMessageBox(this);
   serialMbox2->setStandardButtons(QMessageBox::Ok);
   serialMbox2->setWindowModality(Qt::WindowModal);
   serialMbox2->setModal(true);
   serialMbox2->setFont(_ui->mainMenuFont);
#endif


#ifdef NON_PERSISTENT
   QObject::connect(
         serialMbox2,   &QMessageBox::finished,
         _msgBoxThread, &MsgBoxThread::msbgoxClosed );
#endif

void MainWindow::initSerialMsgbox() {
   bool debug = false;
   TRACEMCF(debug, "Starting");
   // QMessageBox for displaying error messages, one at a time
// #ifdef PERSISTENT_SERIAL_MSG_BOX
   _serialMsgBox = new QMessageBox(this);
   _serialMsgBox->setStandardButtons(QMessageBox::Ok);
   _serialMsgBox->setWindowModality(Qt::WindowModal);
   _serialMsgBox->setModal(true);
   _serialMsgBox->setFont(_ui->mainMenuFont);
// #endif
   _msgBoxQueue = new MsgBoxQueue();
   // TRACE("_msgboxQueue=%p", _msgboxQueue);
   _msgBoxThread = new MsgBoxThread(_msgBoxQueue);

// #ifdef PERSISTENT_SERIAL_MSG_BOX
   QObject::connect(
         _serialMsgBox, &QMessageBox::finished,
         _msgBoxThread, &MsgBoxThread::msbgoxClosed
         );
// #endif

   QObject::connect(_msgBoxThread, &MsgBoxThread::postSerialMsgBox,
                    this,          &MainWindow::showSerialMsgBox);

   // SIGNL/SLOT spec fails in same way
   // QObject::connect(_msgBoxThread, SIGNAL(postSerialMsgBox(QString, QString, QMessageBox::Icon) ),
   //                  this,          SLOT(  showSerialMsgBox(QString, QString, QMessageBox::Icon) ) );

   // Tried deferring until after MainWindow::show() in hopes that dialog box appears over main window
   // msgBoxThread->start();

    GlobalState& globalState = GlobalState::instance();
    globalState._msgBoxThread = _msgBoxThread;
    globalState._msgBoxQueue  = _msgBoxQueue;
    TRACEMCF(debug, "Oone");
}


void MainWindow::start_msgBoxThread() {
   bool debug = false;
   TRACEMCF(debug, "Starting");

#ifdef DEFERRED_MSG_QUEUE
   TRACEMC("Putting %d MsgBoxQueueEntry on _msgBoxQueue", _deferredMsgs.count());
   for (int ndx = 0; ndx < _deferredMsgs.count(); ndx++) {
      MsgBoxQueueEntry * qe = _deferredMsgs.at(ndx);
    _msgBoxQueue->put(qe);
   }
#endif

   _msgBoxThread->start();
   TRACEMCF(debug, "Done.  _msgBoxThread started");
}


void MainWindow::initMonitors(Parsed_Cmd * parsed_cmd) {
    bool debug = false;

    longRunningTaskStart();
    _ui->statusBar->showMessage(QString("Loading display information..."));

    DDCA_Status ddcrc = ddca_get_display_info_list2(
                            true,         // include invalid displays
                            &_dlist);
    assert(ddcrc == 0);

    int initialDisplayIndex = -1;
    // QString qmodel(parsed_cmd->model);

    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        TRACECF(debug, "Processing display %d", ndx);

        // Add entry for monitor in display selector combo box
        QString mfg_id     = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
        QString sn         = _dlist->info[ndx].sn;
#ifdef ALT
        QString mfg_id     = _dlist->info[ndx].mmid.mfg_id;
        QString model_name = _dlist->info[ndx].mmid.model_name;
#endif

        QString s = model_name;
        if (s.isEmpty() ) {
           if ( sn.isEmpty() )
              s = QString("Laptop");
           else
              s = QString("Unknown");    // don't expect this
        }

        int monitorNumber = ndx+1;
        _toolbarDisplayCB->addItem(s, QVariant(monitorNumber));

        // Check if the model name of this monitor matches one specified
        // on the command line.
        TRACECF(debug, "ndx=%d, parsed_cmd->model = |%s|, _dlist->info[ndx].model_name = |%s|",
                ndx, parsed_cmd->model, _dlist->info[ndx].model_name);
        if (parsed_cmd->model) {
           QString userModelParm(parsed_cmd->model);
           if (QString(userModelParm) == model_name) {
              initialDisplayIndex = ndx;
              TRACECF(debug, "model found, ndx=%d", ndx);
           }
        }

        // Create Monitor instance, initialize data structures
        Monitor * curMonitor = new Monitor(&_dlist->info[ndx], monitorNumber);
        _monitors.append(curMonitor);
        // TRACEC("After curMonitor = new Monitor()");
        // ddca_report_display_info(&_dlist->info[ndx], 3);

        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel(curMonitor);
        baseModel->setObjectName(QString::asprintf("baseModel-%d",ndx));

        initMonitorInfoWidget(curMonitor, _ui->centralWidget);
        initCapabilitiesWidget(curMonitor, _ui->centralWidget);
        initFeaturesScrollAreaView(curMonitor, baseModel, _ui->centralWidget, _msgBoxQueue);

        QObject::connect(baseModel,  SIGNAL(signalVcpRequest(VcpRequest*)),
                         curMonitor, SLOT(  putVcpRequest(VcpRequest*)));
        QObject::connect(baseModel, &FeatureBaseModel::signalStartInitialLoad,
                         this,      &MainWindow::longRunningTaskStart);
        QObject::connect(baseModel, &FeatureBaseModel::signalEndInitialLoad,
                         this,      &MainWindow::longRunningTaskEnd);

        curMonitor->_baseModel = baseModel;

        VcpThread * curThread = new VcpThread(NULL,
                                              curMonitor->_displayInfo,
                                              curMonitor->_requestQueue,
                                              baseModel);
        QObject::connect(baseModel,   &FeatureBaseModel::signalStatusMsg,
                         this,        &MainWindow::setStatusMsg);

        curThread->start();
        _vcp_threads.append(curThread);

        // asynchronously get capabilities for current monitor
        if (_dlist->info[ndx].dispno > 0) {     // don't try if monitor known to not support DDC
            curMonitor->_requestQueue->put(new LoadDfrRequest());
            curMonitor->_requestQueue->put(new VcpCapRequest());
        }
    }

#ifdef DEFERRED_MSG_QUEUE
    _deferredMsgs = QList<MsgBoxQueueEntry*>();  // not needed,
#endif
    // TRACECF(true, "initialDisplayIndex = %d", initialDisplayIndex);
    if (parsed_cmd->model) {
       if (initialDisplayIndex < 0) {
          // queue status dialog
          initialDisplayIndex = 0;

          QString qsTitle = QString("ddcutil Error");
          QString qsDetail = QString("Invalid Model: %1").arg(parsed_cmd->model);
          QMessageBox::Icon icon = QMessageBox::Warning;
          MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qsTitle, qsDetail, icon);
#ifdef DEFERRED_MSG_QUEUE
          _deferredMsgs.append(qe);     // not needed
#endif
          _msgBoxQueue->put(qe);
       }
    }
    else {
       initialDisplayIndex = 0;
    }
    // TRACECF(true, "initialDisplayIndex (2) = %d", initialDisplayIndex);

    _toolbarDisplayCB->setCurrentIndex(initialDisplayIndex);

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
    QString msg = QString("Detected ") + QString::number(_dlist->ct) + QString(" displays.");
    _ui->statusBar->showMessage(msg);
    longRunningTaskEnd();
}



MainWindow::MainWindow(Parsed_Cmd * parsed_cmd, QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui_MainWindow(this))
    // _ui(new Ui::MainWindow)
    // , PageChangeObserver()
{
    bool debug = false;
    _cls = metaObject()->className();
    TRACECF(debug, "Starting");

    // _ui->setupUi(this);

    // setWindowIcon(QIcon(":/icons/cinema_display_blue.png"));


#ifdef ALT
    _loadingMsgBox = new QMessageBox(
                            QMessageBox::NoIcon,
                            QString("Title"),
                            QString("Loading2..."),
                            Qt::NoButton,             // buttons
                            this,                 // parent
                            Qt::FramelessWindowHint | Qt::Dialog);on_actionButtonBox_accepted
#endif

    // Register metatypes for primitive types here.
    // Metatypes for classes are registered with the class definition.
    qRegisterMetaType<uint8_t>("uint8_t");
    qRegisterMetaType<bool>("bool");       // needed?

    qRegisterMetaType<NcValuesSource>("NcValuesSource");
    qRegisterMetaType<QMessageBox::Icon>("QMessageBox::Icon");


    QLabel* toolbarDisplayLabel = new QLabel("&Display:  ");
    toolbarDisplayLabel->setFont(_ui->mainMenuFont);
    _toolbarDisplayCB = new QComboBox();
    _toolbarDisplayCB->setObjectName("displaySelectorCombobox");
    _toolbarDisplayCB->setStyleSheet("background-color:white; color:black;");
    _toolbarDisplayCB->setFont(_ui->mainMenuFont);
    toolbarDisplayLabel->setBuddy(_toolbarDisplayCB);
    _ui->mainToolBar->addWidget( toolbarDisplayLabel);
    _ui->mainToolBar->addWidget( _toolbarDisplayCB);

#ifdef MOVED
    // QMessageBox for displaying error messages, one at a time
    _serialMsgBox = new QMessageBox(this);
    _serialMsgBox->setStandardButtons(QMessageBox::Ok);
    _serialMsgBox->setWindowModality(Qt::WindowModal);
    _serialMsgBox->setModal(true);
    _serialMsgBox->setFont(_ui->mainMenuFont);
#endif

    _msgBoxQueue = new MsgBoxQueue();
    // TRACE("_msgboxQueue=%p", _msgboxQueue);
    // MsgBoxThread * msgBoxThread = new MsgBoxThread(_msgBoxQueue);

#ifdef MOVED
    QObject::connect(
          _serialMsgBox, &QMessageBox::finished,
          msgBoxThread, &MsgBoxThread::msbgoxClosed
          );
    QObject::connect(
          msgBoxThread, &MsgBoxThread::postSerialMsgBox,
          this, &MainWindow::showSerialMsgBox
          );
#endif

    // try deferring until after MainWindow::show() in hopes that
    // dialog box appears over main window
    // msgBoxThread->start();
     GlobalState& globalState = GlobalState::instance();
#ifdef MOVED
     globalState._msgBoxThread = msgBoxThread;
#endif

    // reportWidgetChildren(ui->centralWidget, "Children of centralWidget, before initMonitors():");
    initMonitors(parsed_cmd);
    // GlobalState& globalState = GlobalState::instance();

    // Initialize Options menu
    _feature_selector   = new FeatureSelector(parsed_cmd);
    _otherOptionsState = new OtherOptionsState(parsed_cmd);
    globalState._otherOptionsState = _otherOptionsState;
    _uiOptionsState = new UserInterfaceOptionsState(parsed_cmd);
    globalState._uiOptionsState = _uiOptionsState;

    QObject::connect(
        this,     &MainWindow::featureSelectionChanged,
        this,     &MainWindow::on_actionFeaturesScrollArea_triggered);

     // Is this why double call?
     // Why does this need to be explicit, not performed by connectSlotsByName
     // QObject::connect(
     //    this->_ui->actionUserInterfaceOptionsDialog,    &QAction::triggered,
     //    this,     &MainWindow::on_actionUserInterfaceOptionsDialog_triggered);

     // Start with Monitor Summary of first monitor instead of no view selected
     if (_monitors.size() > 0)
// #ifdef WORKS
        emit signalMonitorSummaryView();
// #endif

#ifdef BAD   // get dialog box that capabilities incomplete before man screen appears
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

#ifdef WORKS_WITH_CMAKE
     initSerialMsgbox();     // sets _msgBoxThread

     start_msgBoxThread();
#endif

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

     TRACEMC("Emitting reportApplicationEventLoopStarted()");
     emit reportApplicationEventLoopStarted();   // will not be delivered until application event loop started
#endif

     TRACECF(debug, "Done");
}


MainWindow::~MainWindow()
{
    delete _ui;
}


//
// Status Message slots
//

void MainWindow::setStatusMsg(QString msg) {
   // printf("(%s::%s) msg: %s\n", _cls, __func__, msg.toLatin1().data());  fflush(stdout);
   statusBar()->showMessage(msg,30);
}


//
// SerialMsgBox slots
//

void MainWindow::showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon) {
   bool debugFunc = false;
   TRACEMCF(debugFunc, "Starting. text=%s", QS2S(text));
// #ifdef DIALOG_BOX_STILL_ON_SEPARATE_SCREEN
#ifdef NON_PERSISTENT
   QMessageBox * serialMbox2 = new QMessageBox(this);
   serialMbox2->setStandardButtons(QMessageBox::Ok);
   serialMbox2->setWindowModality(Qt::WindowModal);
   serialMbox2->setModal(true);
   serialMbox2->setFont(_ui->mainMenuFont);
#endif
   _serialMsgBox->setText(text);
   _serialMsgBox->setWindowTitle(title);
   _serialMsgBox->setIcon(icon);

#ifdef NON_PERSISTENT
   QObject::connect(
         serialMbox2,   &QMessageBox::finished,
         _msgBoxThread, &MsgBoxThread::msbgoxClosed );
#endif

   _serialMsgBox->exec();
   TRACEMCF(debugFunc, "Done.     After serialMsgBox2->exec() returns.");
// #endif


#ifdef NO
   _serialMsgBox->setText(text);
   _serialMsgBox->setWindowTitle(title);
   _serialMsgBox->setIcon(icon);
   _serialMsgBox->show();
   _serialMsgBox->exec();
#endif

}


//  ** Miscellaneous Slots

// Sets spinning cursor at start of a long running task
// Both a direct call from initMonitors and a slot for FeatureBaseModel
void MainWindow::longRunningTaskStart() {
   bool debug = false;
   // needs counter
   TRACECF(debug, "Executing");
   // _spinner->start();
   // _loadingMsgBox->show();
   QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

// Restores normal cursor at the of a long running task
// Both a direct call from initMonitors and a slot for FeatureBaseModel
void MainWindow::longRunningTaskEnd() {
   bool debug = false;
   TRACECF(debug, "Executing");
   // _spinner->stop();
   // _loadingMsgBox->hide();
   QGuiApplication::restoreOverrideCursor();
}


//
// Display-selection combo box slots
//

void MainWindow::displaySelectorCombobox_currentIndexChanged(int index) {
   // printf("(%s::%s) index=%d\n", _cls, __func__, index); fflush(stdout);
   switch(_curView) {
   case MonitorView:
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
}

#ifdef UNNEEDED
void MainWindow::displaySelectorCombobox_activated(int index) {
   printf("(%s::%s) index=%d\n", _cls, __func__, index); fflush(stdout);
}
#endif



//
// *** View menu slots
//

// View->Summary

void MainWindow::on_actionMonitorSummary_triggered()
{
    // std::cout << "(MainWindow::on_actionMonitorSummary_triggered()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    char * s = MonitorDescActions::capture_display_info_report(dinfo);

    Monitor * monitor = _monitors[monitorNdx];
    QPlainTextEdit * moninfoPlainText = monitor->_moninfoPlainText;
    // int pageno = monitor->_pageno_moninfo;
    moninfoPlainText->setPlainText(s);
    free(s);

    _curView = View::MonitorView;
    _ui->actionRescan->setEnabled(false);
    _ui->actionMonitorSummary->setChecked(true);
    // _ui->centralWidget->setCurrentIndex(pageno);
    _ui->centralWidget->setCurrentWidget(monitor->_page_moninfo);
    _ui->centralWidget->show();
}


// View->Capabilities


void MainWindow::on_actionCapabilities_triggered()
{
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Display_Ref dref = dinfo->dref;
    char * caps_report = NULL;

    Monitor * monitor = _monitors.at(monitorNdx);

    if (!monitor->capabilities_check_complete()) {
       QMessageBox::warning(this,
                            "ddcutil",
                            "Capabilities check still in progress",
                            QMessageBox::Ok);
       // emit signalMonitorSummaryView();   // doesn't work
       on_actionMonitorSummary_triggered();
    }
    else if (!monitor->supportsDdc()) {
       QMessageBox::warning(this,
                            "ddcutil",
                            "Display does not support DDC",
                            QMessageBox::Ok);
       // emit signalMonitorSummaryView();   // doesn't work
       on_actionMonitorSummary_triggered();
    }
    else {
       DDCA_Status ddcrc = MonitorDescActions::capture_capabilities_report(monitor, dref, &caps_report);
       if (ddcrc != 0) {
           reportDdcApiError("ddca_open_display", ddcrc);
           TRACEC("capture_capabilites_report returned %d", ddcrc);
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
       }
    }
}


void MainWindow::reportDdcApiError(QString funcname, int rc) const {
     QString msg = funcname + "() returned " + QString::number(rc) + " - " + ddca_rc_name(rc);
     _ui->statusBar->showMessage(msg);

     // QErrorMessage * emsg;
     // invalid conversion from const QWidget* to QWidget*
     // emsg = new QErrorMessage(this);
     // emsg->showMessage("oy vey");
}



// View->Features

void MainWindow::on_actionFeaturesScrollArea_triggered()
{
    if (debugFeatureSelection) {
        TRACEC("Desired view: %d, feature list:", View::FeaturesView);
        this->_feature_selector->dbgrpt();
    }

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = _monitors[monitorNdx];
    if (debugFeatureSelection) {
        TRACEC("Current view: %d, feature list:", _curView);
        monitor->_curFeatureSelector.dbgrpt();
    }

    if (!monitor->capabilities_check_complete()) {
       QMessageBox::warning(this, "ddcui", "Capabilities check incomplete", QMessageBox::Ok);
       on_actionMonitorSummary_triggered();
       return;
    }

    if (!monitor->supportsDdc()) {
       QMessageBox::warning(this, "ddcui", "Display does not support DDC", QMessageBox::Ok);
       on_actionMonitorSummary_triggered();
       return;
    }

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
       TRACECF(debugFeatureSelection, "Unchanged view and feature set, no need to load");
    }
}


void MainWindow::loadMonitorFeatures(Monitor * monitor) {
    // TRACE("monitor=%p", monitor);
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
       TRACECF(debugFeatureLists,
           "features_to_show: (%d) %s", ddca_feature_list_count(featuresToShow),
                                        ddca_feature_list_string(featuresToShow, NULL, (char*)" "));

       if (_feature_selector->_includeOnlyCapabilities || _feature_selector->_includeAllCapabilities) {
          // need to test _parsed_caps is valid
          // n. simply manipulates data structures, does not perform monitor io
          DDCA_Feature_List caps_features =
                ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);
          TRACECF(debugFeatureLists,
              "Capabilities features: (%d) %s",
              ddca_feature_list_count(caps_features),
              ddca_feature_list_string(caps_features, NULL, (char*)" "));
          if (_feature_selector ->_includeOnlyCapabilities)
             featuresToShow = ddca_feature_list_and(featuresToShow, caps_features);
          else
             featuresToShow = ddca_feature_list_or(featuresToShow, caps_features);

       }
    }

    TRACECF(debugFeatureLists,
        "Final featuresToShow: (%d) %s",
        ddca_feature_list_count(featuresToShow),
        ddca_feature_list_string(featuresToShow, NULL, (char*)" "));

    // causes async feature reads in VcpThread, then load feature values from model into widgets
    monitor->_baseModel->setFeatureList(featuresToShow);

    // TRACE("Done");
}



//
// *** Actions Menu Slots
//

// Actions->Rescan slots

void MainWindow::on_actionRescan_triggered() {
   bool debug = false;
   TRACECF(debug, "Executing");
   assert(_curView == FeaturesView);
   assert(_curDisplayIndex >= 0);
   _monitors[_curDisplayIndex]->_baseModel->reloadFeatures();
}


void MainWindow::on_actionRedetect_triggered() {
   TRACEC("Unimplemented");
}


// Actions->Debug: DebugActionsDialog slots

void MainWindow::on_actionDebugActionsDialog_triggered()
{
   // bool debug = false;
   // TODO: allocate once and save dialog, cf feature selection
   // display dialog box for selecting features

   // TRACECF(debug, "triggered");
   // assert(false);  // for testing

   DebugActionsDialog* dialog = new DebugActionsDialog(this);
   QObject::connect(dialog, &DebugActionsDialog::resetStats_triggered,
                    this,   &MainWindow::for_resetStats_triggered);
   QObject::connect(dialog, &DebugActionsDialog::reportStats_triggered,
                    this,   &MainWindow::for_reportStats_triggered);
   dialog->exec();
   delete dialog;
}


void MainWindow::for_resetStats_triggered() {
   bool debug = false;
   TRACECF(debug, "triggered");
   ddca_reset_stats();
}


void MainWindow::for_reportStats_triggered(DDCA_Stats_Type stats_type, bool show_thread_data) {
   bool debug = false;
   TRACECF(debug, "triggered. stats_type = %d", stats_type);
   // TO DO: Make per/thread setting a checkbox on dialog
   // ddca_show_stats(stats_type, show_thread_data, 0);
   capture_stats(stats_type, show_thread_data);
}


void MainWindow::capture_stats(DDCA_Stats_Type stats_type, bool show_thread_data) {
       ddca_start_capture(DDCA_CAPTURE_NOOPTS);
       // DDCA_Output_Level saved_ol = ddca_get_output_level();
       // ddca_set_output_level(DDCA_OL_VERBOSE);
       ddca_show_stats(stats_type, show_thread_data, 0);
       // ddca_set_output_level(saved_ol);
       char * s = ddca_end_capture();  // API's buffer, do not free

       QString qs(s);
       const QFont& textFont = QFont(       "Monospace",  9, QFont::Normal);
       // viewHelpByTextX(qs, QString("Statistics Report"), textFont,  this);
       HelpDialog2* hd = new HelpDialog2(this);
       hd->setFont(textFont);
       hd->setText(qs);
          // hd->_textBrowser->setSource(fn);
       hd->setWindowTitle( "Execution Statistics..." );
       hd->exec();     // always modal
       // delete hd;   // causes crash
}


//
// Options Menu Slots
//

// Feature Selection slots: FeatureSelectionDialog

void MainWindow::on_actionFeatureSelectionDialog_triggered()
{
   bool debug = false;
   TRACECF(debug, "Executing. _fsd=%p", _fsd);

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
}


// named "for_action..." instead of "on_action..." to avoid the connectSlotsByName naming convention
// FeatureSelectionDialog not allocated at time connectSlotsByName() called, must use
// explicit connect()
void MainWindow::for_actionFeatureSelectionDialog_accepted()
{
   bool debugFunc = false;
   debugFunc = debugFunc || debugSignals || debugFeatureSelection;
   if (debugFunc) {
       TRACEC("Executing");
       _feature_selector->dbgrpt();
   }
   if (_curView == FeaturesView) {
      TRACECF(debugFunc, "in FeaturesView, signaling featureSelectionChanged()");
      emit featureSelectionChanged();
   }
   else {
      TRACECF(debugFunc, "Not in FeaturesView, so not signaling featureSelectionChanged()");
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
   TRACECF(debug, "triggered");

   OtherOptionsDialog* dialog = new OtherOptionsDialog(this->_otherOptionsState, this);
   // QObject::connect(dialog,   &OtherOptionsDialog::ncValuesSourceChanged,
   //                   this,    &MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
   // QObject::connect(_otherOptionsState,   &OtherOptionsState::ncValuesSourceChanged,
   //                  this,    &MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
   dialog->exec();
   delete dialog;
   TRACECF(debug, "Done");
}

#ifdef OLD
// named for_ .. instead of on_ so that connectSlotsByName doesn't report this as slot
// for which it could find no signal
void MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged(
                     NcValuesSource valuesSource, bool useLatestNcValueNames )
{
   bool debug = true;
   TRACECF(debug, "valuesSource=%d, useLatestNcValueNames=%s", valuesSource, SBOOL(useLatestNcValueNames));

   if (_curView == FeaturesView  )   {  // need also check if  FeaturesScrollAreaView
      int monitorNdx = _toolbarDisplayCB->currentIndex();
      Monitor * monitor = _monitors[monitorNdx];
      // or emit signal?
      monitor->_featuresScrollAreaView->onNcValuesSourceChanged(valuesSource, useLatestNcValueNames);
   }
   TRACECF(debug, "Done");
}
#endif

// Options->User Interface Options slots: UserInterfaceOptionsDialog

// causes the dialog to display
void MainWindow::on_actionUserInterfaceOptionsDialog_triggered()
{
   bool debug = false;
   TRACECF(debug, "Executing. _uid=%p", _uid);

#ifdef NO  // don't bother keeping the dialog box around and hidden
   if (_uid) {
       _uid->_state = new UserInterfaceOptionsState();
   }
   else {
        _uid = new UserInterfaceOptionsData(this, this->_userInterfaceOptionsData);
       QObject::connect(_uid,     &UserInterfaceOptionsDialog::userInterfaceOptionsChanged,
                        this,     &MainWindow::for_UserInterfaceOptionsDialog_accepted);
    }
    _uid->exec();
    //   delete _uid;
#endif

    UserInterfaceOptionsDialog* dialog = new UserInterfaceOptionsDialog(this->_uiOptionsState, this);
    QObject::connect(dialog,   &UserInterfaceOptionsDialog::accepted,
                     this,     &MainWindow::for_actionUserInterfaceOptionsDialog_accept);
    // need a connection for reset?

    TRACECF(debug, "Calling setControKeyRequired(%s)", SBOOL(_uiOptionsState->_controlKeyRequired) );
    dialog->setDialogBoxControlKeyRequired( _uiOptionsState->_controlKeyRequired);

    dialog->exec();
    delete dialog;
}


void MainWindow::for_actionUserInterfaceOptionsDialog_accept()
{
   bool debug = false;
   TRACECF(debug, "Executing, Emitting userIntefaceOptionsChanged");
   // need to test if real?
   emit userInterfaceOptionsChanged();

   // unneeded here - set in UserInterfaceOptionsCialog
   // _uiOptionsState->setControlKeyRequired(newval);
}


//
// Help Menu Slots
//

void MainWindow::on_actionContentsHelp_triggered()
{
    // TRACEC("Executing");
    HelpBrowser::showPage(QString("qrc:/docs/help_general.html"), /*navigable=*/ true);
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


void MainWindow::on_actionAbout_triggered()
{
    QString ddcutil_version = ddca_ddcutil_version_string();
    uint8_t build_opts = ddca_build_options();
    // QString ans1 = (build_opts & DDCA_BUILT_WITH_ADL) ? "true" : "false";
    QString ans2 = (build_opts & DDCA_BUILT_WITH_USB) ? "true" : "false";
    QString ans3 = (ddca_is_udf_enabled()) ? "true" : "false";

    // QMessageBox mbox;
    // mbox.setText("About ddcutil");

    QString copyright = "Copyright (C) 2014-2020 Sanford Rockowitz\n";
    copyright = copyright + "Licensed under the terms of the GNU Public License (GPL) ";
    copyright = copyright + "Version 2.0 or later.";


    QString msg = "";
    msg = msg + "ddcui version:    " + ddcui_version   + "\n";
    msg = msg + "   User defined features enabled:      " + ans3 + "\n\n";
    msg = msg + "ddcutil version:  " + ddcutil_version + "\n";
 // msg = msg + "   Built with support for ADL connected monitors: " + ans1 + "\n";
    msg = msg + "   Built with support for USB connected monitors: " + ans2;
    msg = msg + "\n\n";
    msg = msg + copyright;

    // mbox.setInformativeText(msg);
    // mbox.exec();
    // QMessageBox::information(this, "..", msg);
    QMessageBox::about(this, "About ddcui", msg);
}

// Misc

// Compilation fails without these, but where are they used?

void MainWindow::showCentralWidgetByWidget(QWidget * pageWidget) {
   TRACEC("===========> Setting current index, pageWidget object name = %s",
          "dummy"  /* pageWidget->objectName() */);   // utf-8

   int pageno = _ui->centralWidget->indexOf(pageWidget);
   if (pageno < 0) {
      TRACEC("page for widget not found");
   }
   else {
      TRACEC("widget page number: %d\n", pageno);
      _ui->centralWidget->setCurrentWidget(pageWidget);
      _ui->centralWidget->show();
   }
}



void MainWindow::showCentralWidgetPage(int pageno) {
   TRACEC("===========> Setting current index, pageno = %d", pageno);
   _ui->centralWidget->setCurrentIndex(pageno);
   _ui->centralWidget->show();
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
