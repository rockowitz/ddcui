/* mainwindow.cpp */

#include <iostream>

#include <QtCore/QThread>
#include <QtGui/QFont>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>

#include <ddcutil_c_api.h>

#include "imported/QtWaitingSpinner/waitingspinnerwidget.h"

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "base/monitor.h"

#include "nongui/vcpthread.h"    // includes vcprequest.h

#include "feature_selection/feature_selection_dialog.h"

#include "monitor_desc/monitor_desc_actions.h"
#include "monitor_desc/monitor_desc_ui.h"

#include "feature_value_widgets/value_stacked_widget.h"

#include "feature_scrollarea/feature_widget_basic.h"
#include "feature_scrollarea/features_scrollarea_contents.h"
#include "feature_scrollarea/features_scrollarea_ui.h"
#include "feature_scrollarea/features_scrollarea_view.h"
#include "list_model_view/feature_item_model.h"
#include "list_model_view/list_model_view_ui.h"
#include "list_widget/list_widget_ui.h"
#include "table_model_view/feature_table_model.h"
#include "table_model_view/feature_value_tableitem_delegate.h"
#include "table_model_view/table_model_view_ui.h"
#include "table_widget/table_widget_ui.h"

#include "main/ui_mainwindow2.h"
#include "main/mainwindow.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    // , PageChangeObserver()
{
   _cls = metaObject()->className();

    ui->setupUi(this);

    QLabel* toolbarDisplayLabel = new QLabel("&Display:  ");
    _toolbarDisplayCB = new QComboBox();
    toolbarDisplayLabel->setBuddy(_toolbarDisplayCB);
    ui->mainToolBar->addWidget( toolbarDisplayLabel);
    ui->mainToolBar->addWidget( _toolbarDisplayCB);

    reportWidgetChildren(ui->viewsStackedWidget,
                         "Children of viewsStackedWidget, before initDisplaySelector():");
    initDisplaySelector();
    feature_selector = new FeatureSelector();
    reportWidgetChildren(ui->viewsStackedWidget,
                         "Children of viewsStackedWidget, after initDisplaySelector():");
}


MainWindow::~MainWindow()
{
    delete ui;
}



#ifdef CONFLICT
see features_scrollarea_view.cpp
// static
QSizePolicy pageSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}
#endif



void MainWindow::initDisplaySelector() {
   //  ui->displaySelectorComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
   //  ui->displaySelectorComboBox->setMinimumContentsLength(28);   // 2 + 3 + 3 + 3 + 13

    Qt::WindowModality modality = Qt::ApplicationModal;   // alt WindowModal, ApplicationModal, NonModal
    WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(
                                           modality,
                                           this,       // parent
                                           true);       /// centerOnParent
    spinner->start(); // starts spinning
    QString msg = QString("Loading display information... ");
    ui->statusBar->showMessage(msg);

    _dlist = ddca_get_display_info_list();
    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        printf("(%s) Processing display %d\n", __func__, ndx);  fflush(stdout);

        // Add entry for monitor in display selector combo box
        QString mfg_id     = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
        QString s = QString::number(ndx+1) + ":  " + mfg_id + " - " + model_name;
        int monitorNumber = ndx+1;
        // ui->displaySelectorComboBox->addItem(s, QVariant(ndx+1));
        _toolbarDisplayCB->addItem(s, QVariant(monitorNumber));

        // Create Monitor instance, initialize data structures
        Monitor * curMonitor = new Monitor(&_dlist->info[ndx], monitorNumber);
        monitors.append(curMonitor);

        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel();
        baseModel->setObjectName(QString::asprintf("baseModel-%d",ndx));

        initMonitorInfoWidget(
              curMonitor,
              ui->viewsStackedWidget);

        initCapabilitiesWidget(
              curMonitor,
              ui->viewsStackedWidget);

        initListWidget(
              curMonitor,
              monitorNumber,
              baseModel,
              ui->viewsStackedWidget
              );

        initTableWidget(
              curMonitor,
              baseModel,
              ui->viewsStackedWidget
              );

        initTableView(
              curMonitor,
              baseModel,
              ui->viewsStackedWidget
              );

//        initFeaturesScrollArea(
//              curMonitor,
//              baseModel,
//              ui->viewsStackedWidget
//              );

        initFeaturesScrollAreaView(
              curMonitor,
              baseModel,
              ui->viewsStackedWidget
              );


        QObject::connect(baseModel,  SIGNAL(signalVcpRequest(VcpRequest*)),
                         curMonitor, SLOT(  putVcpRequest(VcpRequest*)));

        curMonitor->_baseModel = baseModel;

        VcpThread * curThread = new VcpThread(NULL,
                                              curMonitor->_displayInfo,
                                              curMonitor->_requestQueue,
                                              baseModel);
        curThread->start();
        vcp_threads.append(curThread);
    }

    // ui->displaySelectorComboBox->setCurrentIndex(0);
    _toolbarDisplayCB->setCurrentIndex(0);

    // Set message in status bar
    msg = QString("Detected ") + QString::number(_dlist->ct) + QString(" displays.");
    ui->statusBar->showMessage(msg);
    spinner->stop();
}


// *** About ***

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


void MainWindow::loadMonitorFeatures(Monitor * monitor) {


    WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(
                                           Qt::ApplicationModal,    // alt WindowModal, ApplicationModal, NonModal
                                           this,       // parent
                                           true);       /// centerOnParent
    QString msg = QString("Reading monitor features...");
    ui->statusBar->showMessage(msg);
    spinner->start();

    DDCA_Feature_List feature_list = monitor->getFeatureList(feature_selector->feature_list_id);

    cout << "feature_list_id: " << feature_selector->feature_list_id << ",  Feature ids: " << endl;
    for (int ndx = 0; ndx <= 255; ndx++) {
        if ( ddca_feature_list_contains(&feature_list, (uint8_t) ndx)) {
            printf("%02x ", ndx);
        }
    }
    cout << endl;

    monitor->_requestQueue->put(new VcpStartInitialLoadRequest);

    for (int ndx = 0; ndx <= 255; ndx++) {
        uint8_t vcp_code = (uint8_t) ndx;
        if ( ddca_feature_list_contains(&feature_list, vcp_code)) {
            // printf("%02x ");

            monitor->_requestQueue->put( new VcpGetRequest(vcp_code));
        }
    }
    monitor->_requestQueue->put(new VcpEndInitialLoadRequest);

    // wrong location - should stop when last response received
    spinner->stop();
}




void MainWindow::reportDdcApiError(QString funcname, int rc) const {
     QString msg = funcname + "() returned " + QString::number(rc) + " - " + ddca_rc_name(rc);
     ui->statusBar->showMessage(msg);

     // QErrorMessage * emsg;
     // invalid conversion from const QWidget* to QWidget*
     // emsg = new QErrorMessage(this);
     // emsg->showMessage("oy vey");

}



// Action: Monitor

void MainWindow::on_actionMonitorSummary_triggered()
{
    std::cout << "(MainWindow::on_actionMonitorSummary_triggered()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();

    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    char * s = capture_display_info_report(dinfo);

    Monitor * monitor = monitors[monitorNdx];
    QPlainTextEdit * moninfoPlainText = monitor->_moninfoPlainText;
    int pageno = monitor->_pageno_moninfo;
    moninfoPlainText->setPlainText(s);
    free(s);
    QFont fixedFont("Courier");
    moninfoPlainText->setFont(fixedFont);

    ui->viewsStackedWidget->setCurrentIndex(pageno);

    ui->viewsStackedWidget->show();
}


// Action: Capabilities

void MainWindow::on_actionCapabilities_triggered()
{
    _curView = View::CapabilitiesView;

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Display_Ref dref = dinfo->dref;
    char * caps_report = NULL;

    DDCA_Status ddcrc = capture_capabilities_report(dref, &caps_report);
    if (ddcrc != 0) {
        reportDdcApiError("ddca_open_display", ddcrc);
        printf("(%s::%s) capture_capabilites_report returned %d\n", _cls, __func__, ddcrc);
    }
    else {
        cout << "Parsed capabilities: " << endl;
        cout << caps_report << endl;

        Monitor * monitor = monitors[monitorNdx];
        QPlainTextEdit * capabilitiesPlainText = monitor->_capabilitiesPlainText;
        int pageno = monitor->_pageno_capabilities;
        capabilitiesPlainText->setPlainText(caps_report);
        free(caps_report);
        QFont fixedFont("Courier");
        capabilitiesPlainText->setFont(fixedFont);

        // show widget
        ui->viewsStackedWidget->setCurrentIndex(pageno);    // need proper constants
        ui->viewsStackedWidget->show();
    }
}



// Actions:  Feature Selection

void MainWindow::on_actionFeatureSelection_triggered()
{
    // display dialog box for selecting features
    cout << "(on_actionFeatureSelection_triggered)" << endl;
}

void MainWindow::on_actionFeatureSelectionDialog_triggered()
{
        // display dialog box for selecting features
        cout << "(on_actionFeatureSelectionDialog_triggered)" << endl;

       FeatureSelectionDialog* fsd = new FeatureSelectionDialog(this, this->feature_selector);
       fsd->show();
}

DDCA_Feature_Subset_Id MainWindow::feature_list_id() const {
    return this->_feature_list_id;
}

void MainWindow::set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id) {
    cout << "(set_feature_list_id) feature_list_id =" << feature_list_id <<endl;
    this->_feature_list_id = feature_list_id;
}


// Actions: Feature Views

void MainWindow::on_actionFeaturesTableView_triggered()
{
    printf("(=============> MainWindow::on_actionFeatures_TableView_triggered)\n");
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_TABLEVIEW;
    loadMonitorFeatures(monitor);


    // connect(tview, SIGNAL(cellClicked(int,int)),
    //         this,  SLOT  (cell_clicked(int,int)));

    connect(monitor->_vcp_tableView, SIGNAL(clicked(QModelIndex)),
            this,                    SLOT(on_vcpTableView_clicked(QModelIndex)));


    int pageno = monitor->_pageno_table_view;
    // ui->viewsStackedWidget->setCurrentIndex(pageno);  // alt setCurrentWidget(tview)
    ui->viewsStackedWidget->setCurrentWidget(monitor->_page_table_view);
    ui->viewsStackedWidget->show();
}


void MainWindow::on_actionFeaturesListView_triggered()
{
    std::cout << "(MainWindow::on_actionFeaturesListView()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_LISTVIEW;
    loadMonitorFeatures(monitor);

    int pageno = monitor->_pageno_list_view;         // ???
    ui->viewsStackedWidget->setCurrentIndex(pageno);

    ui->viewsStackedWidget->show();
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
    Monitor * monitor = monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_LISTWIDGET;
    loadMonitorFeatures(monitor);

    // TO FIX:
    // FeatureListWidget * lwidget = monitor->_featureListWidget;  // unused
    // lview->setModel(monitor->_listModel);

    // TO FIX:
    ui->viewsStackedWidget->setCurrentIndex(monitor->_pageno_listWidget);
    ui->viewsStackedWidget->show();
}



void MainWindow::on_actionFeaturesScrollAreaMock_triggered()
{
    printf("(MainWindow::%s) Starting\n", __func__);  fflush(stdout);

    ValueStdWidget * mock1 = new ValueStdWidget();
        DDCA_MCCS_Version_Spec vspec1 = {2,0};
        DDCA_Non_Table_Vcp_Value val1 = {0, 254, 0, 20};
        DDCA_Feature_Flags flags1 = DDCA_RW | DDCA_STD_CONT;
        FeatureValue * fv1 = new FeatureValue(0x22, vspec1, flags1, val1);
        mock1->setFeatureValue(*fv1);

    ValueContWidget * mock2 = new ValueContWidget();
        DDCA_Non_Table_Vcp_Value val2 = {0, 100, 0, 50};
        DDCA_Feature_Flags flags2 = DDCA_RW | DDCA_STD_CONT;
        FeatureValue * fv2 = new FeatureValue(0x10, vspec1, flags2, val2);
        mock2->setFeatureValue(*fv2);

    ValueStackedWidget * mock3 = new ValueStackedWidget();
        DDCA_Non_Table_Vcp_Value val3 = {0, 80, 0, 30};
        DDCA_Feature_Flags flags3 = DDCA_RW | DDCA_STD_CONT;
        FeatureValue * fv3 = new FeatureValue(0x10, vspec1, flags3, val3);
        mock3->setFeatureValue(*fv3);

    ValueStackedWidget * mock4 = new ValueStackedWidget();
        DDCA_Feature_Flags flags4 = DDCA_RW | DDCA_COMPLEX_CONT;
        FeatureValue * fv4 = new FeatureValue(0x10, vspec1, flags4, val3);
        mock4->setFeatureValue(*fv4);

    FeatureWidgetBasic * mock5 = new FeatureWidgetBasic();
        mock5->setFeatureValue(*fv4);
        printf("mock5:\n");  mock5->dbgrpt();  fflush(stdout);

    FeatureWidgetBasic * mock6 = new FeatureWidgetBasic();
            mock6->setFeatureValue(*fv3);

    FeatureWidgetBasic * mock7 = new FeatureWidgetBasic();
        DDCA_Non_Table_Vcp_Value val7 = {0, 0, 0, 4};
        DDCA_Feature_Flags flags7 = DDCA_RW | DDCA_SIMPLE_NC;
        FeatureValue * fv7 = new FeatureValue(0x60, vspec1, flags7, val7);
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

    ui->viewsStackedWidget->addWidget(page_features_scrollarea);
    ui->viewsStackedWidget->setCurrentWidget(page_features_scrollarea);
    ui->viewsStackedWidget->show();
}


void MainWindow::on_actionFeaturesScrollArea_triggered()
{
    printf("(MainWindow::%s) Starting\n", __func__);  fflush(stdout);
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_SCROLLAREA_VIEW;
    loadMonitorFeatures(monitor);
}




void MainWindow::showCentralWidgetPage(int pageno) {
   printf("(%s::%s) ===========> Setting current index, pageno = %d\n", _cls, __func__,
          pageno);  fflush(stdout);
   ui->viewsStackedWidget->setCurrentIndex(pageno);
   ui->viewsStackedWidget->show();
}

void MainWindow::showCentralWidgetByWidget(QWidget * pageWidget) {
   printf("(%s::%s) ===========> Setting current index, pageWidget object name = %s\n", _cls, __func__,
          "dummy"  /* pageWidget->objectName() */);   // utf-8
   fflush(stdout);

   int pageno = ui->viewsStackedWidget->indexOf(pageWidget);
   if (pageno < 0) {
      printf("(%s::%s) page for widget not found\n", _cls, __func__); fflush(stdout);
   }
   else {
      printf("(%s::%s) widget page number: %d\n", _cls, __func__, pageno); fflush(stdout);
      ui->viewsStackedWidget->setCurrentWidget(pageWidget);
      ui->viewsStackedWidget->show();
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




void MainWindow::on_vcpTableView_clicked(const QModelIndex &index)
{
    printf("-------------> (MainWindow::on_vcpTableView_clicked) row=%d, col=%d\n", index.row(), index.column() );
}

void MainWindow::on_vcpTableView_doubleClicked(const QModelIndex &index)
{
     printf("----------> (MainWindow::on_vcpTableView_doubleClicked) row=%d, col=%d\n", index.row(), index.column() );
}
