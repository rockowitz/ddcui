/* mainwindow.cpp */

#include "assert.h"
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

#include "monitor_desc/monitor_desc_actions.h"
#include "monitor_desc/monitor_desc_ui.h"

#include "feature_value_widgets/value_stacked_widget.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/features_scrollarea_contents.h"
#include "feature_scrollarea/features_scrollarea_ui.h"
#include "feature_scrollarea/features_scrollarea_view.h"

#include "option_dialogs/feature_selection_dialog.h"
#include "option_dialogs/other_options_dialog.h"

#include "main/ui_mainwindow2.h"
#include "main/mainwindow.h"
#include "../alt/list_model_view/feature_item_model.h"
#include "../alt/list_model_view/list_model_view_ui.h"
#include "../alt/list_widget/list_widget_ui.h"
#include "../alt/table_model_view/feature_table_model.h"
#include "../alt/table_model_view/feature_value_tableitem_delegate.h"
#include "../alt/table_model_view/table_model_view_ui.h"
#include "../alt/table_widget/table_widget_ui.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
    // , PageChangeObserver()
{
   _cls = metaObject()->className();

    _ui->setupUi(this);

    _spinner = new WaitingSpinnerWidget(
                      Qt::ApplicationModal,    // alt WindowModal, ApplicationModal, NonModal
                      nullptr,      // parent   - if set to this, spinning widget does not display
                      true,      // centerOnParent
                      true);     // disableParentWhenSpinning
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
    _toolbarDisplayCB->setStyleSheet("background-color:white;");
    toolbarDisplayLabel->setBuddy(_toolbarDisplayCB);
    _ui->mainToolBar->addWidget( toolbarDisplayLabel);
    _ui->mainToolBar->addWidget( _toolbarDisplayCB);

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

    // _dlist = ddca_get_display_info_list();
    DDCA_Status ddcrc = ddca_get_display_info_list2(
                            false,         // don't include invalid displays
                            &_dlist);
    assert(ddcrc == 0);
    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        printf("(%s) Processing display %d\n", __func__, ndx);  fflush(stdout);

        // Add entry for monitor in display selector combo box
#ifdef OLD
        QString mfg_id     = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
#endif
        QString mfg_id     = _dlist->info[ndx].mmid.mfg_id;
        QString model_name = _dlist->info[ndx].mmid.model_name;
        QString s = QString::number(ndx+1) + ":  " + mfg_id + " - " + model_name;
        int monitorNumber = ndx+1;
        // ui->displaySelectorComboBox->addItem(s, QVariant(ndx+1));
        _toolbarDisplayCB->addItem(s, QVariant(monitorNumber));

        // Create Monitor instance, initialize data structures
        Monitor * curMonitor = new Monitor(&_dlist->info[ndx], monitorNumber);
        _monitors.append(curMonitor);

        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel(curMonitor);
        baseModel->setObjectName(QString::asprintf("baseModel-%d",ndx));

        initMonitorInfoWidget(
              curMonitor,
              _ui->centralWidget);

        initCapabilitiesWidget(
              curMonitor,
              _ui->centralWidget);

        if (enableAltFeatures) {

           initListWidget(
                 curMonitor,
                 monitorNumber,
                 baseModel,
                 _ui->centralWidget
                 );

           initTableWidget(
                 curMonitor,
                 baseModel,
                 _ui->centralWidget
                 );

           initTableView(
                 curMonitor,
                 baseModel,
                 _ui->centralWidget
                 );

   //        initFeaturesScrollArea(
   //              curMonitor,
   //              baseModel,
   //              ui->centralWidget
   //              );

        }

        initFeaturesScrollAreaView(
              curMonitor,
              baseModel,
              _ui->centralWidget
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
        curMonitor->_requestQueue->put(new VcpCapRequest());
    }

    _toolbarDisplayCB->setCurrentIndex(0);

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


//


void MainWindow::longRunningTaskStart() {
   // needs counter
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
   _spinner->start();
   // _loadingMsgBox->show();
}

void MainWindow::longRunningTaskEnd() {
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
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
   printf("(%s::%s) index=%d\n", _cls, __func__, index); fflush(stdout);
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
    std::cout << "(MainWindow::on_actionMonitorSummary_triggered()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();

    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    char * s = capture_display_info_report(dinfo);

    Monitor * monitor = _monitors[monitorNdx];
    QPlainTextEdit * moninfoPlainText = monitor->_moninfoPlainText;
    int pageno = monitor->_pageno_moninfo;
    moninfoPlainText->setPlainText(s);
    free(s);
    QFont fixedFont("Courier");
    moninfoPlainText->setFont(fixedFont);

    _curView = View::MonitorView;
    _ui->centralWidget->setCurrentIndex(pageno);

    _ui->centralWidget->show();
}


// Capabilities slots

void MainWindow::on_actionCapabilities_triggered()
{
    _curView = View::CapabilitiesView;

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Display_Ref dref = dinfo->dref;
    char * caps_report = NULL;

    Monitor * monitor = _monitors.at(monitorNdx);

    DDCA_Status ddcrc = capture_capabilities_report(monitor, dref, &caps_report);
    if (ddcrc != 0) {
        reportDdcApiError("ddca_open_display", ddcrc);
        printf("(%s::%s) capture_capabilites_report returned %d\n", _cls, __func__, ddcrc);
    }
    else {
        cout << "Parsed capabilities: " << endl;
        cout << caps_report << endl;

        Monitor * monitor = _monitors[monitorNdx];
        QPlainTextEdit * capabilitiesPlainText = monitor->_capabilitiesPlainText;
        int pageno = monitor->_pageno_capabilities;
        capabilitiesPlainText->setPlainText(caps_report);
        free(caps_report);
        QFont fixedFont("Courier");
        capabilitiesPlainText->setFont(fixedFont);

        // show widget
        _ui->centralWidget->setCurrentIndex(pageno);    // need proper constants
        _ui->centralWidget->show();
    }
}


// Features Slots - Common Functions


void MainWindow::loadMonitorFeatures(Monitor * monitor) {
    printf("(%s::%s) monitor=%p\n", _cls, __func__, monitor); fflush(stdout);
    monitor->dbgrpt();
    QString msg = QString("Reading monitor features...");
    _ui->statusBar->showMessage(msg);
    printf("(%s::%s) After showMessages()\n", _cls, __func__); fflush(stdout);

    DDCA_Feature_List features_to_show = monitor->getFeatureList(_feature_selector->_featureListId);
    char * s = ddca_feature_list_string(&features_to_show, NULL, (char*) " ");
    printf("(%s::%s) features_to_show: %s\n", _cls, __func__, s); fflush(stdout);
    free(s);

    if (_feature_selector->_respectCapabilities) {
       // need to test _parsed_caps is valid
       DDCA_Feature_List caps_features =
             ddca_feature_list_from_capabilities(monitor->_baseModel->_parsed_caps);
       char * s = ddca_feature_list_string(&caps_features, NULL, (char*) " ");
       printf("(%s::%s) Capabilities features: %s\n", _cls, __func__, s); fflush(stdout);
       free(s);
       features_to_show = ddca_feature_list_and(&features_to_show, &caps_features);
    }

    cout << "feature_list_id: " << _feature_selector->_featureListId << ",  Feature ids: " << endl;
    for (int ndx = 0; ndx <= 255; ndx++) {
        if ( ddca_feature_list_contains(&features_to_show, (uint8_t) ndx))
            printf("%02x ", ndx);
    }
    cout << endl;

    // causes async feature reads in VcpThread, then load feature values from model into widgets
    monitor->_baseModel->setFeatureList(features_to_show);

#ifdef MOVED
    DDCA_Feature_List unchecked_features =
          ddca_feature_list_minus(&features_to_show, &monitor->_baseModel->_featuresChecked);

    cout << "Unchecked features: " << endl;
    for (int ndx = 0; ndx <= 255; ndx++) {
        if ( ddca_feature_list_contains(&unchecked_features, (uint8_t) ndx))
            printf("%02x ", ndx);
    }
    cout << endl;

    monitor->_requestQueue->put(new VcpStartInitialLoadRequest);

    for (int ndx = 0; ndx <= 255; ndx++) {
        uint8_t vcp_code = (uint8_t) ndx;
        if ( ddca_feature_list_contains(&unchecked_features, vcp_code)) {
            monitor->_requestQueue->put( new VcpGetRequest(vcp_code));
        }
    }
    monitor->_requestQueue->put(new VcpEndInitialLoadRequest);
#endif

    printf("(%s::%s) Done\n", _cls, __func__); fflush(stdout);
}


// Features slots - Alternative feature views

void MainWindow::on_actionFeaturesTableView_triggered()
{
    printf("(=============> MainWindow::on_actionFeatures_TableView_triggered)\n");
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = _monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_TABLEVIEW;
    loadMonitorFeatures(monitor);


    // connect(tview, SIGNAL(cellClicked(int,int)),
    //         this,  SLOT  (cell_clicked(int,int)));

    connect(monitor->_vcp_tableView, SIGNAL(clicked(QModelIndex)),
            this,                    SLOT(on_vcpTableView_clicked(QModelIndex)));


    // int pageno = monitor->_pageno_table_view;
    // ui->centralWidget->setCurrentIndex(pageno);  // alt setCurrentWidget(tview)
    _ui->centralWidget->setCurrentWidget(monitor->_page_table_view);
    _ui->centralWidget->show();
}


void MainWindow::on_actionFeaturesListView_triggered()
{
    std::cout << "(MainWindow::on_actionFeaturesListView()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = _monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_LISTVIEW;
    loadMonitorFeatures(monitor);

    int pageno = monitor->_pageno_list_view;         // ???
    _ui->centralWidget->setCurrentIndex(pageno);

    _ui->centralWidget->show();
}


void MainWindow::on_actionFeaturesListWidget_triggered()
{
    printf("=================== (MainWindow::%s) Starting\n", __func__);  fflush(stdout);
    // if (_curView != View::FeaturesView) {
    //    _curView = View::FeaturesView;
    //    // ???
    // }
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = _monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_LISTWIDGET;
    loadMonitorFeatures(monitor);

    // TO FIX:
    // FeatureListWidget * lwidget = monitor->_featureListWidget;  // unused
    // lview->setModel(monitor->_listModel);

    // TO FIX:
    _ui->centralWidget->setCurrentIndex(monitor->_pageno_listWidget);
    _ui->centralWidget->show();
}


void MainWindow::on_actionFeaturesScrollAreaMock_triggered()
{
    printf("(MainWindow::%s) Starting\n", __func__);  fflush(stdout);

    Monitor * monitor = _monitors[0];
    DDCA_Display_Ref dref = monitor->_displayInfo->dref;


    ValueStdWidget * mock1 = new ValueStdWidget();
        DDCA_MCCS_Version_Spec vspec1 = {2,0};
        DDCA_Non_Table_Vcp_Value val1 = {0, 254, 0, 20};
        DDCA_Feature_Flags flags1 = DDCA_RW | DDCA_STD_CONT;
        DDCA_Feature_Metadata * md1 = (DDCA_Feature_Metadata*)calloc(1,sizeof(DDCA_Feature_Metadata));
        md1->feature_code = 0x22;
        md1->feature_desc =  (char*) "Description of feature X22";
        md1->feature_flags = flags1;
        md1->feature_name  = (char*) "Feature X22";
        memcpy(md1->marker, DDCA_FEATURE_METADATA_MARKER, 4);
        md1->mmid = NULL;
        md1->sl_values = NULL;
        md1->vspec = vspec1;

        FeatureValue * fv1 = new FeatureValue(0x22, dref, *md1, NULL, val1);
        mock1->setFeatureValue(*fv1);

    ValueContWidget * mock2 = new ValueContWidget();
        DDCA_Non_Table_Vcp_Value val2 = {0, 100, 0, 50};
        DDCA_Feature_Flags flags2 = DDCA_RW | DDCA_STD_CONT;
        DDCA_Feature_Metadata * md2 = (DDCA_Feature_Metadata*) calloc(1, sizeof(DDCA_Feature_Metadata));
        memcpy(md2, md1, sizeof(DDCA_Feature_Metadata));
        md2->feature_flags = flags2;
        FeatureValue * fv2 = new FeatureValue(0x10, dref, *md2, NULL, val2);
        mock2->setFeatureValue(*fv2);

    ValueStackedWidget * mock3 = new ValueStackedWidget();
        DDCA_Non_Table_Vcp_Value val3 = {0, 80, 0, 30};
        DDCA_Feature_Flags flags3 = DDCA_RW | DDCA_STD_CONT;
        md2->feature_flags = flags3;
        FeatureValue * fv3 = new FeatureValue(0x10, dref, *md2, NULL, val3);
        mock3->setFeatureValue(*fv3);

    ValueStackedWidget * mock4 = new ValueStackedWidget();
        DDCA_Feature_Flags flags4 = DDCA_RW | DDCA_COMPLEX_CONT;
        DDCA_Feature_Metadata * md4 = (DDCA_Feature_Metadata*) calloc(1, sizeof(DDCA_Feature_Metadata));
        memcpy(md4, md2, sizeof(DDCA_Feature_Metadata));
        md4->feature_flags = flags4;
        FeatureValue * fv4 = new FeatureValue(0x10, dref, *md4, NULL, val3);
        mock4->setFeatureValue(*fv4);

    FeatureWidget * mock5 = new FeatureWidget();
        mock5->setFeatureValue(*fv4);
        printf("mock5:\n");  mock5->dbgrpt();  fflush(stdout);

    FeatureWidget * mock6 = new FeatureWidget();
            mock6->setFeatureValue(*fv3);

    FeatureWidget * mock7 = new FeatureWidget();
        DDCA_Non_Table_Vcp_Value val7 = {0, 0, 0, 4};
        DDCA_Feature_Flags flags7 = DDCA_RW | DDCA_SIMPLE_NC;

        DDCA_Feature_Metadata * md7 = (DDCA_Feature_Metadata*)  calloc(1,sizeof(DDCA_Feature_Metadata));
        md7->feature_code = 0x60;
        md7->feature_desc = (char*) "Description of feature X22";
        md7->feature_flags = flags7;
        md7->feature_name  =  (char *) "Input Source";
        memcpy(md1->marker, DDCA_FEATURE_METADATA_MARKER, 4);
        md7->mmid = NULL;
        // doesn't compile, to address if I ever really need the mock data again
        // md7->sl_values = {{0x01, "Input 1"},{0x02, "Input 2"}};
        md7->vspec = vspec1;


        FeatureValue * fv7 = new FeatureValue(0x60, dref, *md7, NULL, val7);
        mock7->setFeatureValue(*fv7);

    FeaturesScrollAreaContents * featuresScrollAreaContents =
          new FeaturesScrollAreaContents();

    QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    featuresScrollAreaContents->setLayout(vLayout);   // ok
    // will it work here?  NO, FAIL-3 - even later take and reset
    // ui->featuresScrollArea->setWidget(ui->featuresScrollAreaContents);  // ALT-2

    vLayout->addWidget(mock4);
    vLayout->addWidget(mock3);
    vLayout->addWidget(mock5);
    vLayout->addWidget(mock7);
    vLayout->addWidget(mock6);

    vLayout->setContentsMargins(0,0,0,0);

    // ui->featuresScrollAreaContents->setLayout(vLayout);  // ok here

    // From doc for void QScrollArea::setWidget(QWidget *widget)
    // Note that You must add the layout of widget before you call this function;
    // if you add it later, the widget will not be visible - regardless of when you
    // show() the scroll area. In this case, you can also not show() the widget later.

    QScrollArea * page_features_scrollarea = new QScrollArea();

    page_features_scrollarea->setWidget(featuresScrollAreaContents);  // ALT-2


    if (debugLayout)
       featuresScrollAreaContents->setStyleSheet("background-color:beige;");

    _ui->centralWidget->addWidget(page_features_scrollarea);
    _ui->centralWidget->setCurrentWidget(page_features_scrollarea);
    _ui->centralWidget->show();
}


// Features slots - FeaturesScrollArea

void MainWindow::on_actionFeaturesScrollArea_triggered()
{
    printf("(MainWindow::%s) Desired view: %d, features view: %d, feature list: \n",
          __func__, View::FeaturesView, Monitor::FEATURES_VIEW_SCROLLAREA_VIEW);  fflush(stdout);
    this->_feature_selector->dbgrpt();

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = _monitors[monitorNdx];
    printf("(MainWindow::%s) Current view: %d, features view: %d, feature list:\n",
          __func__, _curView, monitor->_curFeaturesView);  fflush(stdout);
    monitor->_curFeatureSelector.dbgrpt();

    // TODO Combine View, features view
    if (_curView                     != View::FeaturesView                     ||
        _curDisplayIndex             != monitorNdx                             ||
        monitor->_curFeaturesView    != Monitor::FEATURES_VIEW_SCROLLAREA_VIEW ||
        monitor->_curFeatureSelector != *_feature_selector )
    {
       loadMonitorFeatures(monitor);
       _curDisplayIndex = monitorNdx;
       _curView = View::FeaturesView;
       monitor->_curFeaturesView = Monitor::FEATURES_VIEW_SCROLLAREA_VIEW;
       monitor->_curFeatureSelector   = *_feature_selector;
    }
    else {
       printf("(%s::%s) Unchanged view and feature set, no need to load\n", _cls, __func__);
    }
}


//
// Options Menu Slots
//

// Feature Selection slots

#ifdef UNUSED
void MainWindow::on_actionFeatureSelection_triggered()
{
    // display dialog box for selecting features
    cout << "(on_actionFeatureSelection_triggered)" << endl;
}
#endif

void MainWindow::on_actionFeatureSelectionDialog_triggered()
{
    // display dialog box for selecting features
    cout << "(on_actionFeatureSelectionDialog_triggered)" << endl;

    // FeatureSelectionDialog*
    if (fsd) {
        fsd->useSelectorData();
    }
    else {
        fsd = new FeatureSelectionDialog(this, this->_feature_selector);
        // signal not found in FeatureSelectionDialog
        // now this one is working, why?
        QObject::connect(fsd,      &FeatureSelectionDialog::accepted,
                         this,     &MainWindow::for_actionFeatureSelectionDialog_accepted);

#ifdef UNUSED
       QObject::connect(fsd,      &FeatureSelectionDialog::destroyed,
                        this,     &MainWindow::actionFeatureSelectionDialog_destroyed);

       QObject::connect(fsd,     &FeatureSelectionDialog::featureSelectionAccepted,
                        this,    &MainWindow::featureSelectionAccepted);
#endif
    }
    fsd->exec();
}

void MainWindow::featureSelectionDone() {
   printf("(%s::%s)\n", _cls, __func__);
   if (_curView == FeaturesView) {
      printf("%s::%s) Signaling \n", _cls, __func__);
      emit featureSelectionChanged();
   }
}

// named for_action... instead of on_action... to not use the connectSlotsByName naming convention
// FeatureSelectionDialog not allocated at time connectSlotsByName() called, must use
// explicit connect()
void MainWindow::for_actionFeatureSelectionDialog_accepted()
{
   // printf("%s::%s)\n", _cls, __func__);
   featureSelectionDone();
}

#ifdef UNUSED
void MainWindow::actionFeatureSelectionDialog_destroyed(QObject * obj)
{
   printf("%s::%s)\n", _cls, __func__);
}
#endif

#ifdef UNUSED
void MainWindow::featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list) {
   printf("%s::%s) feature_list=%d\n", _cls, __func__, feature_list);
}
#endif


// OtherOptions slots

void MainWindow::on_actionOtherOptionsDialog_triggered()
{
        // display dialog box for selecting features
        cout << "(on_actionOtherOptionsDialog_triggered)" << endl;

       OtherOptionsDialog* dialog = new OtherOptionsDialog(this->_otherOptionsState, this);
       QObject::connect(dialog,   &OtherOptionsDialog::ncValuesSourceChanged,
                        this,     &MainWindow::on_actionOtherOptionsDialog_ncValuesSourceChanged);
       dialog->exec();
}

void MainWindow::on_actionOtherOptionsDialog_ncValuesSourceChanged(NcValuesSource valuesSource )
{
   printf("%s::%s) valuesSource=%d\n", _cls, __func__, valuesSource); fflush(stdout);

   if (_curView == FeaturesView  )   {  // need also check if  FeaturesScrollAreaView
      int monitorNdx = _toolbarDisplayCB->currentIndex();
      Monitor * monitor = _monitors[monitorNdx];
      // or emit signal?
      monitor->_featuresScrollAreaView->onNcValuesSourceChanged(valuesSource);
   }
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
// Help Menu Slots
//

void MainWindow::on_actionAbout_triggered()
{
    QString ddcutil_version = ddca_ddcutil_version_string();
    uint8_t build_opts = ddca_build_options();
    QString ans1 = (build_opts & DDCA_BUILT_WITH_ADL) ? "true" : "false";
    QString ans2 = (build_opts & DDCA_BUILT_WITH_USB) ? "true" : "false";

    // QMessageBox mbox;
    // mbox.setText("About ddcutil");

    QString msg = "";
    msg = msg + "ddcui version:    " + "0.0.0" + "\n\n";
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
// Misc Slots
//

void MainWindow::showCentralWidgetPage(int pageno) {
   printf("(%s::%s) ===========> Setting current index, pageno = %d\n", _cls, __func__,
          pageno);  fflush(stdout);
   _ui->centralWidget->setCurrentIndex(pageno);
   _ui->centralWidget->show();
}

void MainWindow::showCentralWidgetByWidget(QWidget * pageWidget) {
   printf("(%s::%s) ===========> Setting current index, pageWidget object name = %s\n", _cls, __func__,
          "dummy"  /* pageWidget->objectName() */);   // utf-8
   fflush(stdout);

   int pageno = _ui->centralWidget->indexOf(pageWidget);
   if (pageno < 0) {
      printf("(%s::%s) page for widget not found\n", _cls, __func__); fflush(stdout);
   }
   else {
      printf("(%s::%s) widget page number: %d\n", _cls, __func__, pageno); fflush(stdout);
      _ui->centralWidget->setCurrentWidget(pageWidget);
      _ui->centralWidget->show();
   }
}


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
