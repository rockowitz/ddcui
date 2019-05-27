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

#include "imported/QtWaitingSpinner/waitingspinnerwidget.h"

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "base/global_state.h"
#include "base/monitor.h"
#include "base/other_options_state.h"

#include "nongui/vcpthread.h"    // includes vcprequest.h
#include "nongui/msgbox_queue.h"

#include "monitor_desc/monitor_desc_actions.h"
#include "monitor_desc/monitor_desc_ui.h"

#include "msgbox_thread.h"
#include "feature_value_widgets/value_stacked_widget.h"
#include "mainwindow_ui.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/features_scrollarea_contents.h"
#include "feature_scrollarea/features_scrollarea_ui.h"
#include "feature_scrollarea/features_scrollarea_view.h"

#include "option_dialogs/feature_selection_dialog.h"
#include "option_dialogs/other_options_dialog.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui_MainWindow(this))
    // _ui(new Ui::MainWindow)
    // , PageChangeObserver()
{
    _cls = metaObject()->className();

    // _ui->setupUi(this);

    // with either ApplicationModal or WindowModal, moving the application does not move the spinner with it
    // _loadingMsgBox has same problem
    _spinner = new WaitingSpinnerWidget(
                      Qt::WindowModal,    // alt WindowModal, ApplicationModal, NonModal
                      nullptr,      // parent   - if set to this, spinning widget does not display
                      true,         // centerOnParent
                      true);        // disableParentWhenSpinning
    _loadingMsgBox = new QMessageBox(this);
    _loadingMsgBox->setText("Loading...");
    _loadingMsgBox->setStandardButtons(QMessageBox::NoButton);
    _loadingMsgBox->setWindowModality(Qt::WindowModal);
    // needs Qt::Dialog, o.w. does not appear
    _loadingMsgBox->setWindowFlags( Qt::Dialog| Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  // msg box does not display

#ifdef ALT
    _loadingMsgBox = new QMessageBox(
                            QMessageBox::NoIcon,
                            QString("Title"),
                            QString("Loading2..."),
                            Qt::NoButton,             // buttons
                            this,                 // parent
                            Qt::FramelessWindowHint | Qt::Dialog);
#endif

    QLabel* toolbarDisplayLabel = new QLabel("&Display:  ");
    _toolbarDisplayCB = new QComboBox();
    _toolbarDisplayCB->setObjectName("displaySelectorCombobox");
    _toolbarDisplayCB->setStyleSheet("background-color:white; color:black");
    toolbarDisplayLabel->setBuddy(_toolbarDisplayCB);
    _ui->mainToolBar->addWidget( toolbarDisplayLabel);
    _ui->mainToolBar->addWidget( _toolbarDisplayCB);

    // QMessageBox for displaying error messages, one at a time
    _serialMsgBox = new QMessageBox(this);
    _serialMsgBox->setStandardButtons(QMessageBox::Ok);
    _serialMsgBox->setWindowModality(Qt::WindowModal);
    _serialMsgBox->setModal(true);

    _msgboxQueue = new MsgBoxQueue();
    // PRINTFTCM("_msgboxQueue=%p", _msgboxQueue);
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
    _feature_selector   = new FeatureSelector();
    GlobalState& globalState = GlobalState::instance();
    _otherOptionsState = new OtherOptionsState;
    globalState._otherOptionsState = _otherOptionsState;
    // reportWidgetChildren(_ui->centralWidget, "Children of centralWidget, after initMonitors():");

    // Register metatypes for primitive types here.
    // Metatypes for classes are registered with the class definition.
    qRegisterMetaType<uint8_t>("uint8_t");

    qRegisterMetaType<NcValuesSource>("NcValuesSource");
    qRegisterMetaType<QMessageBox::Icon>("QMessageBox::Icon");

     QObject::connect(
        this,     &MainWindow::featureSelectionChanged,
        this,     &MainWindow::on_actionFeaturesScrollArea_triggered);

     // connect for OtherOptions

}


MainWindow::~MainWindow()
{
    delete _ui;
}


void MainWindow::initMonitors() {
   //  ui->displaySelectorComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
   //  ui->displaySelectorComboBox->setMinimumContentsLength(28);   // 2 + 3 + 3 + 3 + 13

    longRunningTaskStart();
    QString msg = QString("Loading display information... ");
    _ui->statusBar->showMessage(msg);

    ddca_set_trace_groups(DDCA_TRC_API);
    DDCA_Status ddcrc = ddca_get_display_info_list2(
                            true,        	// include invalid displays?
                            &_dlist);
    assert(ddcrc == 0);
    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        PRINTFTCM("Processing display %d", ndx);

        // Add entry for monitor in display selector combo box
// #ifdef OLD
        QString mfg_id     = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
// #endif
#ifdef ALT
        QString mfg_id     = _dlist->info[ndx].mmid.mfg_id;
        QString model_name = _dlist->info[ndx].mmid.model_name;
#endif
        // QString s = QString::number(ndx+1) + ":  " + model_name;
        QString s = model_name;

        int monitorNumber = ndx+1;
        _toolbarDisplayCB->addItem(s, QVariant(monitorNumber));

        // Create Monitor instance, initialize data structures
        Monitor * curMonitor = new Monitor(&_dlist->info[ndx], monitorNumber);
        _monitors.append(curMonitor);
        // PRINTFCM("After curMonitor = new Monitor()");
        // ddca_report_display_info(&_dlist->info[ndx], 3);

        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel(curMonitor);
        baseModel->setObjectName(QString::asprintf("baseModel-%d",ndx));

        initMonitorInfoWidget(
              curMonitor,
              _ui->centralWidget);

        initCapabilitiesWidget(
              curMonitor,
              _ui->centralWidget);

        // PRINTFTCM("_msgboxQueue=%p", _msgboxQueue);
        initFeaturesScrollAreaView(
              curMonitor,
              baseModel,
              _ui->centralWidget,
              _msgboxQueue
              );

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
   // needs counter
   PRINTFTCM("Executing");
   _spinner->start();
   // _loadingMsgBox->show();
}


void MainWindow::longRunningTaskEnd() {
   PRINTFTCM("Executing");
   _spinner->stop();
   // _loadingMsgBox->hide();
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

    if (!monitor->supportsDdc()) {
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
           PRINTFTCM("capture_capabilites_report returned %d", ddcrc);
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
    // PRINTFTCM("monitor=%p", monitor);
    // monitor->dbgrpt();
    QString msg = QString("Reading monitor features...");
    _ui->statusBar->showMessage(msg);

    DDCA_Feature_List featuresToShow = monitor->getFeatureList(_feature_selector->_featureListId);
    PRINTFTCMF(debugFeatureLists,
        "features_to_show: %s", ddca_feature_list_string(&featuresToShow, NULL, (char*)" "));

    if (_feature_selector->_respectCapabilities) {
       // need to test _parsed_caps is valid
       // n. simply manipulates data structures, does not perform monitor io
       DDCA_Feature_List caps_features =
             ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);
       PRINTFTCMF(debugFeatureLists,
           "Capabilities features: %s", ddca_feature_list_string(&caps_features, NULL, (char*)" "));
       featuresToShow = ddca_feature_list_and(&featuresToShow, &caps_features);
    }

    PRINTFTCMF(debugFeatureLists,
        "Final features_to_show: %s", ddca_feature_list_string(&featuresToShow, NULL, (char*)" "));

    // causes async feature reads in VcpThread, then load feature values from model into widgets
    monitor->_baseModel->setFeatureList(featuresToShow, _feature_selector->_showUnsupportedFeatures);

    // PRINTFTCM("Done");
}


// *** Features slots - FeaturesScrollArea ***

void MainWindow::on_actionFeaturesScrollArea_triggered()
{
    if (debugFeatureSelection) {

        PRINTFTCM("Desired view: %d, feature list:", View::FeaturesView);
        this->_feature_selector->dbgrpt();
    }

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = _monitors[monitorNdx];
    if (debugFeatureSelection) {
        PRINTFTCM("Current view: %d, feature list:", _curView);
        monitor->_curFeatureSelector.dbgrpt();
    }

    if (!monitor->supportsDdc()) {
       QMessageBox::warning(this,
                            "ddcui",
                            "Display does not support DDC",
                            QMessageBox::Ok);
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
       PRINTFTCM("Unchanged view and feature set, no need to load");
    }
}


//
// Options Menu Slots
//

// Feature Selection slots

void MainWindow::on_actionFeatureSelectionDialog_triggered()
{
   // PRINTFTCM("Executing. _fsd=%p", _fsd);

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
   bool debugFunc = debugSignals || debugFeatureSelection;
   if (debugFunc) {
       PRINTFTCM("Executing");
       _feature_selector->dbgrpt();
   }
   if (_curView == FeaturesView) {
      PRINTFTCMF(debugFunc, "Signaling featureSelectionChanged()");
      emit featureSelectionChanged();
   }
}


// OtherOptions slots

void MainWindow::on_actionOtherOptionsDialog_triggered()
{
     // TODO: allocate once and save dialog, cf feature selection
     // display dialog box for selecting features
    //  PRINTFTCM("triggered");

    OtherOptionsDialog* dialog = new OtherOptionsDialog(this->_otherOptionsState, this);
    QObject::connect(dialog,   &OtherOptionsDialog::ncValuesSourceChanged,
                     this,     &MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged);
    dialog->exec();
    delete dialog;
}

// named for_ .. instead of on_ so that connectSlotsByName doesn't report this as slot
// for which it could find no signal
void MainWindow::for_actionOtherOptionsDialog_ncValuesSourceChanged(NcValuesSource valuesSource )
{
   PRINTFTCM("valuesSource=%d", valuesSource);

   if (_curView == FeaturesView  )   {  // need also check if  FeaturesScrollAreaView
      int monitorNdx = _toolbarDisplayCB->currentIndex();
      Monitor * monitor = _monitors[monitorNdx];
      // or emit signal?
      monitor->_featuresScrollAreaView->onNcValuesSourceChanged(valuesSource);
   }
   PRINTFCM("Done");
}

#ifdef UNUSED
void MainWindow::on_actionOtherOptionsDialog_accepted()
{
   printf("%s::%s)\n", _cls, __func__); fflush(stdout);
}

DDCA_Feature_Subset_Id MainWindow::feature_list_id() const {
    return this->_feature_list_id;
}
#endif

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
   PRINTFCM("Executing");
   assert(_curView == FeaturesView);
   assert(_curDisplayIndex >= 0);
   _monitors[_curDisplayIndex]->_baseModel->reloadFeatures();
}

void MainWindow::on_actionRedetect_triggered() {
   PRINTFCM("Executing");
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
   PRINTFTCM("Starting.");
   _serialMsgBox->setText(text);
   _serialMsgBox->setWindowTitle(title);
   _serialMsgBox->setIcon(icon);
   _serialMsgBox->exec();
}


void MainWindow::showCentralWidgetPage(int pageno) {
   PRINTFTCM("===========> Setting current index, pageno = %d", pageno);
   _ui->centralWidget->setCurrentIndex(pageno);
   _ui->centralWidget->show();
}


void MainWindow::showCentralWidgetByWidget(QWidget * pageWidget) {
   PRINTFTCM("===========> Setting current index, pageWidget object name = %s",
          "dummy"  /* pageWidget->objectName() */);   // utf-8

   int pageno = _ui->centralWidget->indexOf(pageWidget);
   if (pageno < 0) {
      PRINTFTCM("page for widget not found");
   }
   else {
      PRINTFTCM("widget page number: %d\n", pageno);
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
