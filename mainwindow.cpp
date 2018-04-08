/* mainwindow.cpp */

#include <iostream>

#include <QtGui/QFont>
#include <QtWidgets/QMessageBox>
#include <QtCore/QThread>
#include <QtWidgets/QWidget>

#include <ddcutil_c_api.h>

#include "QtWaitingSpinner/waitingspinnerwidget.h"

#include "feature_value_widgets/value_stacked_widget.h"

#include "table_model_view/feature_table_model.h"
#include "table_model_view/feature_value_tableitem_delegate.h"

#include "feature_selection_dialog.h"
#include "monitor.h"
#include "ui_mainwindow2.h"
#include "vcplineitem.h"
#include "feature_widget_basic.h"
#include "feature_item_model.h"
#include "features_scrollarea_view.h"

#include "mainwindow.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    // , PageChangeObserver()
{
   _cls = metaObject()->className();

    ui->setupUi(this);

    cout << "After setupUI()" << endl;

    QLabel* toolbarDisplayLabel = new QLabel("Display:  ");
    _toolbarDisplayCB = new QComboBox();
    ui->mainToolBar->addWidget( toolbarDisplayLabel);
    ui->mainToolBar->addWidget( _toolbarDisplayCB);
    ui->capabilities_plainText->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->moninfoPlainText->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);



#ifdef DEBUG
    cout << "============> Children of centralWidget: " << endl;
    QObjectList  childs = ui->centralWidget->children();
    for (int ndx = 0; ndx < childs.size(); ndx++) {
        QObject* curobj = childs.at(ndx);
        QString name   = curobj->objectName();
       // cout << "Child: " << QString("xxx") <<  endl;   // no match for operator<<
        QByteArray ba = name.toLatin1();
        const char * s = ba.data();
        printf("   Child: %s\n", s); fflush(stdout);
        delete curobj;  // works

    }
#endif
#ifdef DOESNT_WORK
    QObject * item = nullptr;
    while((item == childs.takeAt(0))) {
        delete item;
    }
#endif



    QWidget* page0 = new QWidget();
    page0->setObjectName("page_moninfo");
    _moninfoPlainText = new QPlainTextEdit();
    page0->setObjectName("moninfoPlainTextEdit");
    // how to add _moninfoPlainText to page0?

    QWidget* page1 = new QWidget();
    page1->setObjectName("page_table_view");
    _vcp_tableview = new QTableView();
    _vcp_tableview->setObjectName("vcpTableView");

    QWidget * page2 = new QScrollArea();
    page2->setObjectName("featureWidgetScrollArea");

    _views_StackedWidget = new QStackedWidget(ui->centralWidget);
    _views_StackedWidget->setObjectName("views_StackedWidget");



    _views_StackedWidget->addWidget(page0);
    _views_StackedWidget->addWidget(page1);
    _views_StackedWidget->addWidget(page2);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_views_StackedWidget);

    initDisplaySelector();
    feature_selector = new FeatureSelector();


    // ui->MainWindow.centralWidget->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

static QSizePolicy pageSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}


QSizePolicy tableWidgetSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}


QWidget * initListWidget(
      Monitor *         curMonitor,
      int               ndx,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   FeatureItemModel * listModel = new FeatureItemModel(baseModel);

   // FeatureListWidget * listWidget = ui->feature_listWidget;   // WRONG  -need separate instance for each monitor


   // page_list_widget/vcp_feature_listwidget
#ifdef REF
   QWidget *page_list_widget;
   QListWidget *feature_listWidget;
   int _pageno_list_widget;
#endif
   QWidget * page_listWidget =  new QWidget();
   page_listWidget->setObjectName(QString::asprintf("page_listWidget-%d",ndx));
   curMonitor->_page_listWidget = page_listWidget;
   page_listWidget->setSizePolicy(pageSizePolicy());
   page_listWidget->setMinimumSize(QSize(700,0));

#ifdef PER_MONITOR_FEATURE_SCROLLAREA
   QWidget * page_scrollArea = new QWidget();
   page_scrollArea->setObjectName(QString::asprintf("page_scrollArea-%d", ndx));
   curMonitor->_page_scrollArea = page_scrollArea;
   page_scrollArea->setSizePolicy(pageSizePolicy());
   page_scrollArea->setMinimumSize(QSize(700,0));
#endif

   // TODO: size, font, etc

   // feature_listWidget = new QListWidget(page_list_widget);
  //  FeatureListWidget * listWidget = ui->feature_listWidget;
   FeatureListWidget * featureListWidget= new FeatureListWidget(curMonitor->_page_listWidget);
   featureListWidget->setObjectName(QString::asprintf("featureListWidget-%d",ndx));
   featureListWidget->setModel(baseModel);

   // works
   // QString on1 = featureListWidget->objectName();
   // std::string on2 = on1.toStdString();
   // const char * on3 = on2.c_str();

   std::string on2 =  featureListWidget->objectName().toStdString();

   // fails
   // const char * on3 = featureListWidget->objectName().toStdString().c_str();
   // must be separate step.  why?
   const char * on3 = on2.c_str();

   printf("(MainWindow::%s) Allocated FeatureListWidget. objectName = %s\n", __func__, on3); fflush(stdout);
   featureListWidget->setSizePolicy(tableWidgetSizePolicy());
   curMonitor->_featureListWidget = featureListWidget;

   QHBoxLayout *hLayout = new QHBoxLayout(page_listWidget);
   hLayout->setSpacing(6);
   hLayout->setContentsMargins(11,11,11,11);
   hLayout->addWidget(featureListWidget);

   curMonitor->_pageno_listWidget = stackedWidget->count();
   stackedWidget->addWidget(page_listWidget);

   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                    listModel,  SLOT(  startInitialLoad()));
   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                    listModel,  SLOT(  endInitialLoad()));

   // *** Connect baseModel to ListWidget ***

    qRegisterMetaType<FeatureValue>("FeatureValue");
    QObject::connect(baseModel,         SIGNAL(signalStartInitialLoad()),
                     featureListWidget, SLOT(  startInitialLoad()));
    QObject::connect(baseModel,         SIGNAL(signalEndInitialLoad()),
                     featureListWidget, SLOT(  endInitialLoad()));
    QObject::connect(baseModel,         SIGNAL(signalFeatureAdded(FeatureValue)),    // char is a built-in QMetaType, uint8_t is not
                     featureListWidget, SLOT(  featureAdded(FeatureValue)));
    QObject::connect(baseModel,         SIGNAL(signalFeatureUpdated(char)),
                     featureListWidget, SLOT(  featureUpdated(char)));

    // Use Qt5 function pointers
    QObject::connect(baseModel,         &FeatureBaseModel::signalFeatureAdded,
                     featureListWidget, &FeatureListWidget::featureAdded);

    // use directly coded observers  - DISABLED slots now working
    // baseModel->addFeatureChangeObserver(featureListWidget);

    curMonitor->setFeatureItemModel(listModel);

   return page_listWidget;

}


void initTableView(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{

   FeatureTableModel * tableModel = new FeatureTableModel(baseModel);
   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                    tableModel, SLOT(  startInitialLoad()));
   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                    tableModel, SLOT(  endInitialLoad()));

   curMonitor->setFeatureTableModel(tableModel);

}


// Ugh: class method to locate the showCentralWidgetByWidget slot

QWidget * MainWindow::initFeaturesScrollArea(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{

#ifdef PER_MONITOR_FEATURE_SCROLLAREA
        ValueStdWidget * mock1 = new ValueStdWidget();

        DDCA_Non_Table_Vcp_Value val1 = {0, 254, 0, 20};
        DDCA_Feature_Flags flags1 = DDCA_RW | DDCA_STD_CONT;
        DDCA_MCCS_Version_Spec vspec1 = {2,0};
        FeatureValue * fv1 = new FeatureValue(0x22, vspec1, flags1, val1);
        mock1->setFeatureValue(*fv1);

        QVBoxLayout * vLayout = new QVBoxLayout(page_scrollArea);
        vLayout->addWidget(mock1);
        // vLayout->addWidget(mock2);

        curMonitor->_pageno_scrollArea = ui->views_stackedWidget->count();
        ui->views_stackedWidget->addWidget(page_scrollArea);
#endif

        //
        // Per-monitor permanent scroll area
        //

        printf("(MainWindow::%s) Allocating per-monitor features scrollarea page\n", __func__); fflush(stdout);
        QScrollArea *   featuresScrollArea = new QScrollArea();
        FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
        scrollAreaContents->setObjectName("created in initDisplaySelector");
        QVBoxLayout* vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName("created in initDisplaySelector");
        scrollAreaContents->setLayout(vboxLayout);

        curMonitor->_page_features_scrollarea   = featuresScrollArea;
        curMonitor->_featuresScrollAreaContents = scrollAreaContents;  // n. constructor sets the layout

        // TODO: include monitor number in name


        featuresScrollArea->setWidget(scrollAreaContents);
        scrollAreaContents->setContainingScrollArea(featuresScrollArea);

        int pageno = stackedWidget->count();
        curMonitor->_pageno_features_scrollarea = pageno;
        stackedWidget->addWidget(featuresScrollArea);

        featuresScrollArea->setObjectName(QString::asprintf("page_features_scrollarea-%d", pageno));
        scrollAreaContents->setObjectName(QString::asprintf("featuresScrollAreaContents-%d", pageno));

        // probably premature
        // printf("(%s) Setting current index %d\n",  __func__, pageno); fflush(stdout);
        stackedWidget->setCurrentIndex(pageno);



        // *** Connect base model to scroll area ***

        QObject::connect(baseModel,          SIGNAL(signalStartInitialLoad()),
                         scrollAreaContents, SLOT(  startInitialLoad()));
        QObject::connect(baseModel,          SIGNAL(signalEndInitialLoad()),
                         scrollAreaContents, SLOT(  endInitialLoad()));
        QObject::connect(baseModel,          SIGNAL(signalFeatureAdded(FeatureValue)),    // char is a built-in QMetaType, uint8_t is not
                         scrollAreaContents, SLOT(  featureAdded(FeatureValue)));
        QObject::connect(baseModel,          SIGNAL(signalFeatureUpdated(char)),
                         scrollAreaContents, SLOT(  featureUpdated(char)));

        // Use Qt5 function pointers
        QObject::connect(baseModel,          &FeatureBaseModel::signalFeatureAdded,
                         scrollAreaContents, &FeaturesScrollAreaContents::featureAdded);

        QObject::connect(baseModel,          &FeatureBaseModel::signalFeatureUpdated,
                         scrollAreaContents, &FeaturesScrollAreaContents::featureUpdated);

        // not working, try old mechanism
        // now it's working, sigh
        QObject::connect(scrollAreaContents, &FeaturesScrollAreaContents::showCentralWidgetByWidget,
                         this,               &MainWindow::showCentralWidgetByWidget);

        QObject::connect(scrollAreaContents, SIGNAL(showCentralWidgetByWidget(QWidget*)),
                         this,               SLOT(  showCentralWidgetByWidget(QWidget*)) );

        // slots not working, but observer is triggered
        // scrollAreaContents->addPageChangeObserver(this);

        // *** End scroll area connections

   return featuresScrollArea;
}


void initFeaturesScrollAreaView(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   printf("(%s) _views_StackedWidget=%p\n", __func__, stackedWidget); fflush(stdout);

   FeaturesScrollAreaView * featuresView =
            new FeaturesScrollAreaView(
                    curMonitor,
                    baseModel,
                    stackedWidget);
   curMonitor->_featuresScrollAreaView = featuresView;

   QObject::connect(baseModel,          SIGNAL(signalEndInitialLoad()),
                    featuresView,       SLOT(  onEndInitialLoad()));

      // QObject::connect(baseModel,          &FeatureBaseModel::signalEndInitialLoad,
      //                  featuresView,       &FeaturesScrollAreaView::onEndInitialLoad);

}



void MainWindow::initDisplaySelector() {
   //  ui->displaySelectorComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
   //  ui->displaySelectorComboBox->setMinimumContentsLength(28);   // 2 + 3 + 3 + 3 + 13

    Qt::WindowModality modality = Qt::ApplicationModal;   // alt WindowModal, ApplicationModal, NonModal
    WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(
                                           modality,
                                           this,       // parent
                                           true);       /// centerOnParent
    spinner->start(); // starts spinning

    _dlist = ddca_get_display_info_list();
    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        printf("(%s) Processing display %d\n", __func__, ndx);  fflush(stdout);

        // Add entry for monitor in display selector combo box
        QString mfg_id     = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
        QString s = QString::number(ndx+1) + ":  " + mfg_id + " - " + model_name;
        // ui->displaySelectorComboBox->addItem(s, QVariant(ndx+1));
        _toolbarDisplayCB->addItem(s, QVariant(ndx+1));

        // Create Monitor instance, initialize data structures
        Monitor * curMonitor = new Monitor(&_dlist->info[ndx]);
        monitors.append(curMonitor);

        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel();
        baseModel->setObjectName(QString::asprintf("baseModel-%d",ndx));

        // ListWidget

        initListWidget(
              curMonitor,
              ndx,
              baseModel,
              ui->views_stackedWidget
              );

        initTableView(
              curMonitor,
              baseModel,
              ui->views_stackedWidget
              );

        initFeaturesScrollArea(
              curMonitor,
              baseModel,
              ui->views_stackedWidget
              );

        initFeaturesScrollAreaView(
              curMonitor,
              baseModel,
              ui->views_stackedWidget
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
    QString msg = QString("Detected ") + QString::number(_dlist->ct) + QString(" displays.");
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


// TEMP - try stuffing some dummy items into list
// was iftested out in on_action_features_triggered()
#ifdef NO
VcpLineItem * item1 = new VcpLineItem(ui->vcp_listWidget);
item1->setFeatureCode(0x10);
item1->setFeatureName(QString("brightness"));
item1->setFeatureRW(QString("RW"));
item1->setFeatureMccsType(QString("C"));

VcpLineItem * item2 = new VcpLineItem(ui->vcp_listWidget);
item2->setFeatureCode(0x12);
item2->setFeatureName(QString("Contrast"));
item2->setFeatureRW(QString("RW"));
item2->setFeatureMccsType(QString("C"));

cout << ui->page_vcp->objectName().toStdString() << endl;

// ui->vcp_listWidget->addItem(item1);
//    ui->vcp_listWidget->addItem(item2);


// QObject * parentObject = item1->parent;
// QString pname = QString("none");
// if (parentObject)
//    pname = parentObject->objectName;

if (item1->parent())
   cout << "item1->parent->objectName():  none"  << endl;
else
   cout << "item1->parent->objectName(): " << item1->parent()->objectName().toStdString() << endl;
// cout << "item1->parentWidget->objectName(): " << item1->parentWidget()->objectName().toStdString() << endl;


// show widget
ui->views_stackedWidget->setCurrentIndex(2);    // need proper constants
ui->views_stackedWidget->show();
#endif


#ifdef OLD_NAME
void MainWindow::on_actionFeatures_triggered()
{
    if (m_curView != View::FeaturesView) {
       m_curView = View::FeaturesView;
       // ???
    }
    int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    loadMonitorFeatures(monitor);

    QListView * lview = ui->vcp_listView;
    lview->setModel(monitor->_listModel);

    ui->views_stackedWidget->setCurrentIndex(5);
    ui->views_stackedWidget->show();
}
#endif


void MainWindow::reportDdcApiError(QString funcname, int rc) const {
     QString msg = funcname + "() returned " + QString::number(rc) + " - " + ddca_rc_name(rc);
     ui->statusBar->showMessage(msg);

     // QErrorMessage * emsg;
     // invalid conversion from const QWidget* to QWidget*
     // emsg = new QErrorMessage(this);
     // emsg->showMessage("oy vey");

}


// Action: Capabilities

void MainWindow::on_actionCapabilities_triggered()
{
    char * caps = NULL;
    char * caps_report = NULL;
    DDCA_Capabilities * parsed_caps;
    // if (m_curView != View::CapabilitiesView) {
    _curView = View::CapabilitiesView;
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Display_Ref dref = dinfo->dref;
    DDCA_Display_Handle dh;
    DDCA_Status rc = ddca_open_display(dref,&dh);
    if (rc != 0) {
        reportDdcApiError("ddca_open_display", rc);
        // put up dialog box?
    }
    if (rc == 0) {
        rc = ddca_get_capabilities_string(dh, &caps);
        if (rc != 0) {
            // do something
            // QString msg = "ddca_get_capabilities_string() returned " + QString::number(rc) + " - " + ddca_rc_name(rc);
            // ui->statusBar->showMessage(msg);
            reportDdcApiError(QString("ddca_get_capabilities_string"), rc);
            // goto bye;
        }
    }
    if (rc == 0) {
        rc = ddca_parse_capabilities_string(caps, &parsed_caps);
        if (rc != 0) {
            // do something
            reportDdcApiError("ddca_parse_capabilities_string", rc);
        }
    }
    if (rc == 0) {
        // wrap in collector
        DDCA_Output_Level saved_ol = ddca_get_output_level();
        ddca_set_output_level(DDCA_OL_VERBOSE);
        ddca_start_capture(DDCA_CAPTURE_NOOPTS);
        ddca_report_parsed_capabilities(parsed_caps, 0);
        caps_report = ddca_end_capture();
        ddca_set_output_level(saved_ol);

        cout << "Parsed capabilities: " << endl;
        cout << caps_report << endl;


        // write to widget
        ui->capabilities_plainText->setPlainText(caps_report);
        QFont fixedFont("Courier");
        ui->capabilities_plainText->setFont(fixedFont);
        free(caps_report);

        // show widget
        ui->views_stackedWidget->setCurrentIndex(1);    // need proper constants
         ui->views_stackedWidget->show();
    }

    // }

    return;
}


// Action: Monitor

void MainWindow::on_actionMonitorSummary_triggered()
{
    std::cout << "(MainWindow::on_actionMonitorSummary_triggered()" << endl;

    int monitorNdx = _toolbarDisplayCB->currentIndex();

    ddca_start_capture(DDCA_CAPTURE_NOOPTS);
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Output_Level saved_ol = ddca_get_output_level();
    ddca_set_output_level(DDCA_OL_VERBOSE);
    ddca_dbgrpt_display_info(dinfo, 0);
    ddca_set_output_level(saved_ol);
    char * s = ddca_end_capture();

    ui->moninfoPlainText->setPlainText(s);
    free(s);
    QFont fixedFont("Courier");
    ui->moninfoPlainText->setFont(fixedFont);

    ui->views_stackedWidget->setCurrentIndex(0);    // need proper constants
    ui->views_stackedWidget->show();
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
    loadMonitorFeatures(monitor);
    QTableView * tview = ui->vcp_tableView;
    tview->setModel(monitor->_tableModel);
    tview->setColumnWidth(0,40);   // feature code
    tview->setColumnWidth(1, 200);  // feature name
    tview->setColumnWidth(2, 40);   // C/NC/T
    tview->setColumnWidth(3, 30);   //  RW/WO/RO
    tview->setColumnWidth(4, 400);  // feature value

        printf("(=============> MainWindow::on_actionFeatures_TableView_triggered) Before editing config\n");

    tview->setItemDelegateForColumn(4, new FeatureValueTableItemDelegate);
    tview->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // connect(tview, SIGNAL(cellClicked(int,int)),
    //         this,  SLOT  (cell_clicked(int,int)));

    connect(tview, SIGNAL(clicked(QModelIndex)),
            this,  SLOT(on_vcpTableView_clicked(QModelIndex)));

    tview->setSelectionBehavior(QAbstractItemView::SelectItems);
    tview->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->views_stackedWidget->setCurrentIndex(4);
    ui->views_stackedWidget->show();
}


void MainWindow::on_actionFeaturesListView_triggered()
{
    std::cout << "(MainWindow::on_actionFeaturesListView()" << endl;
    if (_curView != View::FeaturesView) {
       _curView = View::FeaturesView;
       // ???
    }
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    loadMonitorFeatures(monitor);

    QListView * lview = ui->vcp_listView;
    lview->setModel(monitor->_listModel);

    ui->views_stackedWidget->setCurrentIndex(5);
    ui->views_stackedWidget->show();
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
    loadMonitorFeatures(monitor);

    // TO FIX:
    // FeatureListWidget * lwidget = monitor->_featureListWidget;  // unused
    // lview->setModel(monitor->_listModel);

    // TO FIX:
    ui->views_stackedWidget->setCurrentIndex(monitor->_pageno_listWidget);
    ui->views_stackedWidget->show();
}

#ifdef OLD
// Keep copy while refactoring
void MainWindow::on_actionFeaturesScrollArea_triggered_old()
{
    printf("=================== (MainWindow::%s) Starting\n", __func__);  fflush(stdout);
    // if (_curView != View::FeaturesView) {
    //    _curView = View::FeaturesView;
    //    // ???
    // }
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    loadMonitorFeatures(monitor);

    // TO FIX:
    // FeatureListWidget * lwidget = monitor->_featureListWidget;
    // lview->setModel(monitor->_listModel);

    // Add some dummy data

    // ISSUE: Get per-monitor page?
    // For per-monitor page
    // ui->views_stackedWidget->setCurrentIndex(monitor->_pageno_scrollArea);
    // Single page for all monitors
    ui->views_stackedWidget->setCurrentIndex(ui->_pageno_scrollarea);


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

    QVBoxLayout * vLayout = new QVBoxLayout();
    // will it work here?  yes
    ui->featuresScrollAreaContents->setLayout(vLayout);
    // will it work here?  NO, FAIL-3 - even later take and reset
    // ui->featuresScrollArea->setWidget(ui->featuresScrollAreaContents);  // ALT-2


    vLayout->addWidget(mock4);
    vLayout->addWidget(mock3);
    vLayout->addWidget(mock5);
    vLayout->addWidget(mock7);
    vLayout->addWidget(mock6);
    // vLayout->addWidget(mock1);
    // vLayout->addWidget(mock2);

    // ok here:
    // ui->featuresScrollAreaContents->setLayout(vLayout);

    // From doc for void QScrollArea::setWidget(QWidget *widget)
    // Note that You must add the layout of widget before you call this function;
    // if you add it later, the widget will not be visible - regardless of when you
    // show() the scroll area. In this case, you can also not show() the widget later.

    // works here
    ui->featuresScrollArea->setWidget(ui->featuresScrollAreaContents);  // ALT-2

    // take and reset when original setWidget comes before addWidget fails
    QWidget * tempWidget = ui->featuresScrollArea->takeWidget();  // FAIL -3
    printf("===> after takeWidget(), scroll area widget now: %p\n",
           ui->featuresScrollArea->widget());
    ui->featuresScrollArea->setWidget(tempWidget);  // FAIL - 3
    printf("===> after setWidget(), scroll area widget now: %p\n",
           ui->featuresScrollArea->widget());



    printf("(%s)  ================>  ui->_pageno_scrollarea=%d\n", __func__, ui->_pageno_scrollarea); fflush(stdout);
    ui->views_stackedWidget->setCurrentIndex(ui->_pageno_scrollarea);
    ui->views_stackedWidget->show();


#ifdef NO
    // wrong location
    printf("(MainWindow::%s) ===========> Setting current index %d\n", __func__,
           monitor->_pageno_features_scrollarea);
    QThread::sleep(4);

    ui->views_stackedWidget->setCurrentIndex(monitor->_pageno_features_scrollarea);
#endif

}
#endif


void MainWindow::on_actionFeaturesScrollAreaMock_triggered()
{
    printf("(MainWindow::%s) Starting\n", __func__);  fflush(stdout);
    // int monitorNdx = _toolbarDisplayCB->currentIndex();
    // Monitor * monitor = monitors[monitorNdx];
    //  loadMonitorFeatures(monitor);

    // N. using single page for all monitors
    ui->views_stackedWidget->setCurrentIndex(ui->_pageno_scrollarea);

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

    QVBoxLayout * vLayout = new QVBoxLayout();
    // will it work here?  yes
    ui->featuresScrollAreaContents->setLayout(vLayout);
    // will it work here?  NO, FAIL-3 - even later take and reset
    // ui->featuresScrollArea->setWidget(ui->featuresScrollAreaContents);  // ALT-2


    vLayout->addWidget(mock4);
    vLayout->addWidget(mock3);
    vLayout->addWidget(mock5);
    vLayout->addWidget(mock7);
    vLayout->addWidget(mock6);

    // ok here:
    // ui->featuresScrollAreaContents->setLayout(vLayout);

    // From doc for void QScrollArea::setWidget(QWidget *widget)
    // Note that You must add the layout of widget before you call this function;
    // if you add it later, the widget will not be visible - regardless of when you
    // show() the scroll area. In this case, you can also not show() the widget later.

    // works here
    ui->featuresScrollArea->setWidget(ui->featuresScrollAreaContents);  // ALT-2

    // take and reset when original setWidget comes before addWidget fails
    QWidget * tempWidget = ui->featuresScrollArea->takeWidget();  // FAIL -3
    printf("===> after takeWidget(), scroll area widget now: %p\n",
           ui->featuresScrollArea->widget());
    ui->featuresScrollArea->setWidget(tempWidget);  // FAIL - 3
    printf("===> after setWidget(), scroll area widget now: %p\n",
           ui->featuresScrollArea->widget());

    printf("(%s) ui->_pageno_scrollarea=%d\n", __func__, ui->_pageno_scrollarea); fflush(stdout);
    ui->views_stackedWidget->setCurrentIndex(ui->_pageno_scrollarea);
    ui->views_stackedWidget->show();


#ifdef NO
    // wrong location
    printf("(MainWindow::%s) ===========> Setting current index %d\n", __func__,
           monitor->_pageno_features_scrollarea);
    QThread::sleep(4);

    ui->views_stackedWidget->setCurrentIndex(monitor->_pageno_features_scrollarea);
#endif

}



void MainWindow::on_actionFeaturesScrollArea_triggered()
{
    printf("(MainWindow::%s) Starting\n", __func__);  fflush(stdout);
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    loadMonitorFeatures(monitor);
}




void MainWindow::showCentralWidgetPage(int pageno) {
   printf("(%s::%s) ===========> Setting current index, pageno = %d\n", _cls, __func__,
          pageno);  fflush(stdout);
   ui->views_stackedWidget->setCurrentIndex(pageno);
   ui->views_stackedWidget->show();
}

void MainWindow::showCentralWidgetByWidget(QWidget * pageWidget) {
   printf("(%s::%s) ===========> Setting current index, pageWidget object name = %s\n", _cls, __func__,
          "dummy"  /* pageWidget->objectName() */);   // utf-8
   fflush(stdout);

   int pageno = ui->views_stackedWidget->indexOf(pageWidget);
   if (pageno < 0) {
      printf("(%s::%s) page for widget not found\n", _cls, __func__); fflush(stdout);
   }
   else {
      printf("(%s::%s) widget page number: %d\n", _cls, __func__, pageno); fflush(stdout);
      ui->views_stackedWidget->setCurrentWidget(pageWidget);
      ui->views_stackedWidget->show();
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
