/* mainwindow.cpp */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "main/mainwindow.h"

#include <assert.h>
#include <iostream>

#include <QtCore/QThread>
#include <QtGui/QFont>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>

#include <ddcutil_c_api.h>

#include "../base/core.h"
#include "../base/widget_debug.h"
// #include "imported/QtWaitingSpinner/waitingspinnerwidget.h"

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


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui_MainWindow(this))
    // _ui(new Ui::MainWindow)
    // , PageChangeObserver()
{
    _cls = metaObject()->className();

    // _ui->setupUi(this);

#ifdef OLD
    // with either ApplicationModal or WindowModal, moving the application does not move the spinner with it
    // _loadingMsgBox has same problem
    _spinner = new WaitingSpinnerWidget(
                      Qt::WindowModal,    // alt WindowModal, ApplicationModal, NonModal
                      nullptr,     // parent   - if set to this, spinning widget does not display
                                     //            if set to this->_ui won't compile
                                     //                      &this->_ui, &this
                      true,         // centerOnParent
                      true);        // disableParentWhenSpinning
    // TRACE("Spinner Settings: ");
    // QColor wcolor = _spinner->color();
    // TRACE("   color:      %s", qs2s(wcolor.name()));

    _loadingMsgBox = new QMessageBox(this);
    _loadingMsgBox->setText("Loading...");
    _loadingMsgBox->setStandardButtons(QMessageBox::NoButton);
    _loadingMsgBox->setWindowModality(Qt::WindowModal);
    // needs Qt::Dialog, o.w. does not appear
    _loadingMsgBox->setWindowFlags( Qt::Dialog| Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  // msg box does not display
#endif

#ifdef ALT
    _loadingMsgBox = new QMessageBox(
                            QMessageBox::NoIcon,
                            QString("Title"),
                            QString("Loading2..."),
                            Qt::NoButton,             // buttons
                            this,                 // parent
                            Qt::FramelessWindowHint | Qt::Dialog);on_actionButtonBox_accepted
#endif

    QLabel* toolbarDisplayLabel = new QLabel("&Display:  ");
    toolbarDisplayLabel->setFont(_ui->mainMenuFont);
    _toolbarDisplayCB = new QComboBox();
    _toolbarDisplayCB->setObjectName("displaySelectorCombobox");
    _toolbarDisplayCB->setStyleSheet("background-color:white; color:black;");
    _toolbarDisplayCB->setFont(_ui->mainMenuFont);
    toolbarDisplayLabel->setBuddy(_toolbarDisplayCB);
    _ui->mainToolBar->addWidget( toolbarDisplayLabel);
    _ui->mainToolBar->addWidget( _toolbarDisplayCB);

    // QMessageBox for displaying error messages, one at a time
    _serialMsgBox = new QMessageBox(this);
    _serialMsgBox->setStandardButtons(QMessageBox::Ok);
    _serialMsgBox->setWindowModality(Qt::WindowModal);
    _serialMsgBox->setModal(true);
    _serialMsgBox->setFont(_ui->mainMenuFont);

    _msgboxQueue = new MsgBoxQueue();
    // TRACE("_msgboxQueue=%p", _msgboxQueue);
    MsgBoxThread * msgBoxThread = new MsgBoxThread(_msgboxQueue);

    QObject::connect(
          _serialMsgBox, &QMessageBox::finished,
          msgBoxThread, &MsgBoxThread::msbgoxClosed
          );
    QObject::connect(
          msgBoxThread, &MsgBoxThread::postSerialMsgBox,
          this, &MainWindow::showSerialMsgBox
          );

    msgBoxThread->start();

    // reportWidgetChildren(ui->centralWidget, "Children of centralWidget, before initMonitors():");
    initMonitors();
    GlobalState& globalState = GlobalState::instance();

    _feature_selector   = new FeatureSelector();

    _otherOptionsState = new OtherOptionsState;
    globalState._otherOptionsState = _otherOptionsState;

    _uiOptionsState = new UserInterfaceOptionsState;
    globalState._uiOptionsState = _uiOptionsState;

    // reportWidgetChildren(_ui->centralWidget, "Children of centralWidget, after initMonitors():");

    // Register metatypes for primitive types here.
    // Metatypes for classes are registered with the class definition.
    qRegisterMetaType<uint8_t>("uint8_t");
    qRegisterMetaType<bool>("bool");       // needed?

    qRegisterMetaType<NcValuesSource>("NcValuesSource");
    qRegisterMetaType<QMessageBox::Icon>("QMessageBox::Icon");

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
        emit signalMonitorSummaryView();
}


MainWindow::~MainWindow()
{
    delete _ui;
}


void MainWindow::initMonitors() {
   //  ui->displaySelectorComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
   //  ui->displaySelectorComboBox->setMinimumContentsLength(28);   // 2 + 3 + 3 + 3 + 13

   bool debug = false;

    longRunningTaskStart();
    QString msg = QString("Loading display information... ");
    _ui->statusBar->showMessage(msg);

    // ddca_set_trace_groups(DDCA_TRC_API);
    DDCA_Status ddcrc = ddca_get_display_info_list2(
                            true,        	// include invalid displays
                            &_dlist);
    assert(ddcrc == 0);
    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        TRACECF(debug, "Processing display %d", ndx);

        // Add entry for monitor in display selector combo box
// #ifdef OLD
        QString mfg_id     = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
        QString sn         = _dlist->info[ndx].sn;
// #endif
#ifdef ALT
        QString mfg_id     = _dlist->info[ndx].mmid.mfg_id;
        QString model_name = _dlist->info[ndx].mmid.model_name;
#endif
        // QString s = QString::number(ndx+1) + ":  " + model_name;
        QString s = model_name;
        if (s.isEmpty() ) {
           if ( sn.isEmpty() )
              s = QString("Laptop");
           else
              s = QString("Unknown");    // don't expect this
        }

        int monitorNumber = ndx+1;
        _toolbarDisplayCB->addItem(s, QVariant(monitorNumber));

        // Create Monitor instance, initialize data structures
        Monitor * curMonitor = new Monitor(&_dlist->info[ndx], monitorNumber);
        _monitors.append(curMonitor);
        // TRACE("After curMonitor = new Monitor()");
        // ddca_report_display_info(&_dlist->info[ndx], 3);

        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel(curMonitor);
        baseModel->setObjectName(QString::asprintf("baseModel-%d",ndx));

        initMonitorInfoWidget(curMonitor, _ui->centralWidget);
        initCapabilitiesWidget(curMonitor, _ui->centralWidget);
        // TRACE("_msgboxQueue=%p", _msgboxQueue);
        initFeaturesScrollAreaView(curMonitor, baseModel, _ui->centralWidget, _msgboxQueue);

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

    _toolbarDisplayCB->setCurrentIndex(0);    // *** Can set to 1 instead of 0 for testing ***

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
    msg = QString("Detected ") + QString::number(_dlist->ct) + QString(" displays.");
    _ui->statusBar->showMessage(msg);
    longRunningTaskEnd();
}


void MainWindow::longRunningTaskStart() {
   bool debug = false;
   // needs counter
   TRACECF(debug, "Executing");
   // _spinner->start();
   // _loadingMsgBox->show();
   QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}


void MainWindow::longRunningTaskEnd() {
   bool debug = false;
   TRACECF(debug, "Executing");
   // _spinner->stop();
   // _loadingMsgBox->hide();
   QGuiApplication::restoreOverrideCursor();
}


void MainWindow::setStatusMsg(QString msg) {
   // printf("(%s::%s) msg: %s\n", _cls, __func__, msg.toLatin1().data());  fflush(stdout);
   statusBar()->showMessage(msg,30);
}


void MainWindow::reportDdcApiError(QString funcname, int rc) const {
     QString msg = funcname + "() returned " + QString::number(rc) + " - " + ddca_rc_name(rc);
     _ui->statusBar->showMessage(msg);

     // QErrorMessage * emsg;
     // invalid conversion from const QWidget* to QWidget*
     // emsg = new QErrorMessage(this);
     // emsg->showMessage("oy vey");
}


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
// View Menu Slots
//

// Monitor slots

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


// Capabilities slots

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


// Features Slots - Common Functions

void MainWindow::loadMonitorFeatures(Monitor * monitor) {
    // TRACE("monitor=%p", monitor);
    // monitor->dbgrpt();
    QString msg = QString("Reading monitor features...");
    _ui->statusBar->showMessage(msg);

    DDCA_Feature_List featuresToShow = DDCA_EMPTY_FEATURE_LIST;
    if (_feature_selector->_featureListId == DDCA_SUBSET_CAPABILITIES) {
       featuresToShow = ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);

    }
    else {
       featuresToShow = monitor->getFeatureList(_feature_selector->_featureListId);
       TRACECF(debugFeatureLists,
           "features_to_show: (%d) %s", ddca_feature_list_count(&featuresToShow),
                                        ddca_feature_list_string(&featuresToShow, NULL, (char*)" "));

       if (_feature_selector->_includeOnlyCapabilities || _feature_selector->_includeAllCapabilities) {
          // need to test _parsed_caps is valid
          // n. simply manipulates data structures, does not perform monitor io
          DDCA_Feature_List caps_features =
                ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);
          TRACECF(debugFeatureLists,
              "Capabilities features: (%d) %s",
              ddca_feature_list_count(&caps_features),
              ddca_feature_list_string(&caps_features, NULL, (char*)" "));
          if (_feature_selector ->_includeOnlyCapabilities)
             featuresToShow = ddca_feature_list_and(&featuresToShow, &caps_features);
          else
             featuresToShow = ddca_feature_list_or(&featuresToShow, &caps_features);

       }
    }

    TRACECF(debugFeatureLists,
        "Final featuresToShow: (%d) %s",
        ddca_feature_list_count(&featuresToShow),
        ddca_feature_list_string(&featuresToShow, NULL, (char*)" "));

    // causes async feature reads in VcpThread, then load feature values from model into widgets
    monitor->_baseModel->setFeatureList(featuresToShow);

    // TRACE("Done");
}


// *** Features slots - FeaturesScrollArea ***

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
    if (_curView                     != View::FeaturesView                     ||
        _curDisplayIndex             != monitorNdx                             ||
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


//
// Options Menu Slots
//

// Feature Selection slots

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



// OtherOptions slots

void MainWindow::on_actionOtherOptionsDialog_triggered()
{
   bool debug = false;
   // TODO: allocate once and save dialog, cf feature selection
   // display dialog box for selecting features
   TRACECF(debug, "triggered");

   OtherOptionsDialog* dialog = new OtherOptionsDialog(this->_otherOptionsState, this);
   QObject::connect(dialog,   &OtherOptionsDialog::ncValuesSourceChanged,
                     this,     &MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
   dialog->exec();
   delete dialog;
   TRACECF(debug, "Done");
}


// named for_ .. instead of on_ so that connectSlotsByName doesn't report this as slot
// for which it could find no signal
void MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged(NcValuesSource valuesSource )
{
   bool debug = false;
   TRACECF(debug, "valuesSource=%d", valuesSource);

   if (_curView == FeaturesView  )   {  // need also check if  FeaturesScrollAreaView
      int monitorNdx = _toolbarDisplayCB->currentIndex();
      Monitor * monitor = _monitors[monitorNdx];
      // or emit signal?
      monitor->_featuresScrollAreaView->onNcValuesSourceChanged(valuesSource);
   }
   TRACECF(debug, "Done");
}


// UserInterfaceOptionsDialog slots

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

    TRACECF(debug, "Calling setControKeyRequired(%s)", sbool(_uiOptionsState->controlKeyRequired) );
    dialog->setDialogBoxControlKeyRequired( _uiOptionsState->controlKeyRequired);

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

// DebugActionsDialog slots

void MainWindow::on_actionDebugActionsDialog_triggered()
{
   bool debug = false;
   // TODO: allocate once and save dialog, cf feature selection
   // display dialog box for selecting features
   TRACECF(debug, "triggered");

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

void MainWindow::for_reportStats_triggered(DDCA_Stats_Type stats_type) {
   bool debug = false;
   TRACECF(debug, "triggered. stats_type = %d", stats_type);
   ddca_show_stats(stats_type, 0);
}


DDCA_Feature_Subset_Id MainWindow::feature_list_id() const {
    return this->_feature_list_id;
}


#ifdef UNUSED
void MainWindow::set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id) {
    cout << "(set_feature_list_id) feature_list_id =" << feature_list_id <<endl;
    this->_feature_list_id = feature_list_id;
}
#endif

//
// Actions Menu Slots
//

void MainWindow::on_actionRescan_triggered() {
   bool debug = false;
   TRACECF(debug, "Executing");
   assert(_curView == FeaturesView);
   assert(_curDisplayIndex >= 0);
   _monitors[_curDisplayIndex]->_baseModel->reloadFeatures();
}

void MainWindow::on_actionRedetect_triggered() {
   TRACEC("Executing");
}


//
// Help Menu Slots
//

void MainWindow::on_actionAbout_triggered()
{
    QString ddcutil_version = ddca_ddcutil_version_string();
    uint8_t build_opts = ddca_build_options();
    QString ans1 = (build_opts & DDCA_BUILT_WITH_ADL) ? "true" : "false";
    QString ans2 = (build_opts & DDCA_BUILT_WITH_USB) ? "true" : "false";
    QString ans3 = (ddca_is_udf_enabled()) ? "true" : "false";

    // QMessageBox mbox;
    // mbox.setText("About ddcutil");

    QString msg = "";
    msg = msg + "ddcui version:    " + ddcui_version   + "\n";
    msg = msg + "   User defined features enabled:      " + ans3 + "\n\n";
    msg = msg + "ddcutil version:  " + ddcutil_version + "\n";
    msg = msg + "   Built with support for ADL connected monitors: " + ans1 + "\n";
    msg = msg + "   Built with support for USB connected monitors: " + ans2;
    // mbox.setInformativeText(msg);
    // mbox.exec();
    // QMessageBox::information(this, "..", msg);
    QMessageBox::about(this, "About ddcui", msg);
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}


//
// Miscellaneous Slots
//

void MainWindow::showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon) {
   _serialMsgBox->setText(text);
   _serialMsgBox->setWindowTitle(title);
   _serialMsgBox->setIcon(icon);
   _serialMsgBox->exec();
}


void MainWindow::showCentralWidgetPage(int pageno) {
   TRACEC("===========> Setting current index, pageno = %d", pageno);
   _ui->centralWidget->setCurrentIndex(pageno);
   _ui->centralWidget->show();
}


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
