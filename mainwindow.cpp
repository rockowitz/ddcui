/* mainwindow.cpp */

#include <iostream>

#include <QtGui/QFont>
#include <QtWidgets/QMessageBox>
#include <QtCore/QThread>
#include <QtWidgets/QWidget>

#include <ddcutil_c_api.h>

#include "QtWaitingSpinner/waitingspinnerwidget.h"

#include "base/debug_utils.h"
#include "feature_value_widgets/value_stacked_widget.h"

#include "table_model_view/feature_table_model.h"
#include "table_model_view/feature_value_tableitem_delegate.h"

#include "base/ddcui_globals.h"
#include "feature_selection/feature_selection_dialog.h"
#include "monitor.h"
#include "ui_mainwindow2.h"
// #include "vcplineitem.h"
#include "mainwindow.h"

#include "feature_scrollarea/feature_widget_basic.h"
#include "feature_scrollarea/features_scrollarea_view.h"
#include "list_model_view/feature_item_model.h"


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

    reportWidgetChildren(ui->viewsStackedWidget);
    initDisplaySelector();
    feature_selector = new FeatureSelector();
    reportWidgetChildren(ui->viewsStackedWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}


// Initialization for monitor info, capabilities are identical

void initPlaintextWidget(
      const char *          name,
      int                   monitorNumber,
      QStackedWidget *      stackedWidget,
      QWidget **            page_loc,
      int *                 pageno_loc,
      QPlainTextEdit **     pagePlainText_loc

)
{
   // TODO: CLEAN UP AND SIMPLIFY!

     QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
     sizePolicy1.setHorizontalStretch(1);
     sizePolicy1.setVerticalStretch(1);
     // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
     sizePolicy1.setHeightForWidth(false);

      // Layout stacked widget page: page_widget, contains moninfoPlainText
      QWidget * page_widget = new QWidget();


      QPlainTextEdit *plainTextWidget;

      // sizePolicy1.setHeightForWidth(page_widget->sizePolicy().hasHeightForWidth());
      page_widget->setSizePolicy(sizePolicy1);

      plainTextWidget = new QPlainTextEdit(page_widget);
      plainTextWidget->setObjectName(QString::asprintf("plainTextWidget-%s-%d", name, monitorNumber));
      plainTextWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

      plainTextWidget->setGeometry(QRect(6, 6, 700, 191));   // was 574,191
      // sizePolicy1.setHeightForWidth(plainTextWidget->sizePolicy().hasHeightForWidth());
      plainTextWidget->setSizePolicy(sizePolicy1);
      plainTextWidget->setMaximumSize(QSize(2000, 16777215));   // 574->2000
      plainTextWidget->setLineWrapMode(QPlainTextEdit::NoWrap);
      plainTextWidget->setReadOnly(true);

      // AMEN!
      QHBoxLayout *
      pageMoninfoLayout = new QHBoxLayout(page_widget);
      pageMoninfoLayout->setSpacing(6);
      // pageMoninfoLayout->setContentsMargins(11, 11, 11, 11);
      pageMoninfoLayout->setObjectName(QString::asprintf("pageLayout-%s-%d", name, monitorNumber));
      pageMoninfoLayout->addWidget(plainTextWidget);

      int pageno_widget = stackedWidget->count();
      stackedWidget->addWidget(page_widget);

      page_widget->setObjectName(
            QString::asprintf("page_widget-%s-%d-pageno-%d", name, monitorNumber, pageno_widget));

      *page_loc          = page_widget;
      *pageno_loc        = pageno_widget;
      *pagePlainText_loc = plainTextWidget;
}

QWidget * initMonitorInfoWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget)

{
   initPlaintextWidget(
         "MonitorInfo",
         curMonitor->_monitorNumber,              // 1 based
         stackedWidget,
         &curMonitor->_page_moninfo,
         &curMonitor->_pageno_moninfo,
         &curMonitor->_moninfoPlainText);

    return curMonitor->_page_moninfo;
}


QWidget * initCapabilitiesWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget)
{
   initPlaintextWidget(
         "Capabilities",
         curMonitor->_monitorNumber,              // 1 based
         stackedWidget,
         &curMonitor->_page_capabilities,
         &curMonitor->_pageno_capabilities,
         &curMonitor->_capabilitiesPlainText);

   return curMonitor->_page_capabilities;

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


QSizePolicy tableWidgetSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}


#ifdef NO
// really listWidget

// initialization for model/view variant listview

QWidget * initListView(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   // Layout stacked widget page page_list_widget, contains vcp_feature_listwidget
   #ifdef REF
   QWidget *page_list_widget;
   QListWidget *feature_listWidget;
   int _pageno_list_widget;
   #endif

   QWidget * page_list_widget = new QWidget();
   page_list_widget->setObjectName(QString::fromUtf8("page_list_widget"));

   // TODO: size, font, etc

   // feature_listWidget = new QListWidget(page_list_widget);
   FeatureListWidget * feature_listWidget = new FeatureListWidget(page_list_widget);
   feature_listWidget->setObjectName(QString::fromUtf8("feature_listWidget"));

   int pageno  = stackedWidget->count();
   stackedWidget->addWidget(page_list_widget);

   curMonitor->page_list_widget   = page_list_widget;
   curMonitor->feature_listWidget = feature_listWidget;
   curMonitor->_pageno_listWidget = pageno;
}
#endif



QWidget * initListView(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
    FeatureItemModel * listModel = new FeatureItemModel(baseModel);

    // Layout stacked widget page page_list_view, contains vcp_listview
    QWidget * page_list_view = new QWidget();
    page_list_view->setObjectName(QString::fromUtf8("page_list_view"));

   QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   sizePolicy5.setHorizontalStretch(0);
   sizePolicy5.setVerticalStretch(0);
   sizePolicy5.setHeightForWidth(page_list_view->sizePolicy().hasHeightForWidth());
   page_list_view->setSizePolicy(sizePolicy5);

   QFont font;
   font.setFamily(QString::fromUtf8("LM Mono 9"));
   font.setPointSize(9);
   page_list_view->setFont(font);

   QListView * vcp_listView = new QListView(page_list_view);
   vcp_listView->setObjectName(QString::fromUtf8("vcp_listView"));

   vcp_listView->setGeometry(QRect(6, 6, 256, 192));
   sizePolicy5.setHeightForWidth(vcp_listView->sizePolicy().hasHeightForWidth());
   vcp_listView->setSizePolicy(sizePolicy5);
   QFont font1;
   font1.setFamily(QString::fromUtf8("Monospace"));
   font1.setPointSize(10);
   vcp_listView->setFont(font1);

   QLabel* label = new QLabel(page_list_view);
   label->setObjectName(QString::fromUtf8("label"));
   label->setGeometry(QRect(268, 6, 342, 17));
   label->setText(QApplication::translate("MainWindow", "Header line for page_list_view containing vcp_listView", 0));

   int pageno = stackedWidget->count();
   stackedWidget->addWidget(page_list_view);

   // These  connects belong are for listModel, not listWidget
   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                    listModel,  SLOT(  startInitialLoad()));
   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                    listModel,  SLOT(  endInitialLoad()));

   curMonitor->setFeatureItemModel(listModel);

   curMonitor->_pageno_list_view = pageno;
   curMonitor->page_list_view    = page_list_view;
   curMonitor->vcp_listView      = vcp_listView;

   return page_list_view;
}




// stackedwidget page contains:
//     a PageListWidget, extends QList Widget
//        contains FeatureWidget, extends QListWidgetItem, Widget
//
// sets curMonitor-> page_listWidget
//                ->_featureListWidget
//                ->_pageno_listWidget

QWidget * initListWidget(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
  // FeatureItemModel * listModel = new FeatureItemModel(baseModel);

   // FeatureListWidget * listWidget = ui->feature_listWidget;   // WRONG  -need separate instance for each monitor


   // page_list_widget/vcp_feature_listwidget
#ifdef REF
   QWidget *page_list_widget;
   QListWidget *feature_listWidget;
   int _pageno_list_widget;
#endif

   QWidget * page_listWidget =  new QWidget();

   // which?
   QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
   sizePolicy3.setHorizontalStretch(1);
   sizePolicy3.setVerticalStretch(1);
   // sizePolicy3.setHeightForWidth(page_vcp->sizePolicy().hasHeightForWidth());
   sizePolicy3.setHeightForWidth(false);
   // WHICH?
   page_listWidget->setSizePolicy(sizePolicy3);
   // page_listWidget->setSizePolicy(pageSizePolicy());
   page_listWidget->setMinimumSize(QSize(700,0));

   curMonitor->_page_listWidget = page_listWidget;


#ifdef PER_MONITOR_FEATURE_SCROLLAREA
   QWidget * page_scrollArea = new QWidget();
   page_scrollArea->setObjectName(QString::asprintf("page_scrollArea-%d", monitorNumber));
   curMonitor->_page_scrollArea = page_scrollArea;
   page_scrollArea->setSizePolicy(pageSizePolicy());
   page_scrollArea->setMinimumSize(QSize(700,0));
#endif

   // TODO: size, font, etc

   // feature_listWidget = new QListWidget(page_list_widget);
  //  FeatureListWidget * listWidget = ui->feature_listWidget;
   FeatureListWidget * featureListWidget= new FeatureListWidget(curMonitor->_page_listWidget);
   featureListWidget->setObjectName(QString::asprintf("featureListWidget-%d",monitorNumber));
   featureListWidget->setModel(baseModel);

   std::string on2 =  featureListWidget->objectName().toStdString();
   // fails: const char * on3 = featureListWidget->objectName().toStdString().c_str();
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
   page_listWidget->setObjectName(
         QString::asprintf("page_listWidget-%d-pageno-%d",monitorNumber, curMonitor->_pageno_listWidget));

   // These  connects belong are for listModel, not listWidget
//   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
//                    listModel,  SLOT(  startInitialLoad()));
//   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
//                    listModel,  SLOT(  endInitialLoad()));

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

    // listModel, not listWidget
    // curMonitor->setFeatureItemModel(listModel);


   return page_listWidget;

}

// initialization for QTableWidget variant

void initTableWidget(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   // Layout stacked widget page page_table_item, contains tableWidget

   QSizePolicy  sizePolicy1 = QSizePolicy();    // probably should take from elsewhere
   QSizePolicy  sizePolicy3 = QSizePolicy();    // probably should take from elsewhere

   QWidget *
   page_table_item = new QWidget();
   page_table_item->setObjectName(QString::fromUtf8("page_table_item"));

   sizePolicy1.setHeightForWidth(page_table_item->sizePolicy().hasHeightForWidth());
   page_table_item->setSizePolicy(sizePolicy1);

   QTableWidget *
   tableWidget = new QTableWidget(page_table_item);
   if (tableWidget->columnCount() < 5)
       tableWidget->setColumnCount(5);
   QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
   QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
   QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
   QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
   QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
   tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
   tableWidget->setGeometry(QRect(0, -9, 751, 251));

   // sizePolicy3.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
   tableWidget->setSizePolicy(sizePolicy3);
   tableWidget->setMinimumSize(QSize(581, 0));
   tableWidget->setColumnCount(5);


   // Set header titles
   QTableWidgetItem * tableWidgetItem = NULL;
   tableWidgetItem = tableWidget->horizontalHeaderItem(0);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Code", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(1);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Name", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(2);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Type", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(3);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "RW", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(4);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Value", 0));


   int pageno = stackedWidget->count();
   stackedWidget->addWidget(page_table_item);

   curMonitor->_pageno_table_item = pageno;
   curMonitor->page_table_item    = page_table_item;
   curMonitor->tableWidget        = tableWidget;
}


// Initialization for Model/View Table variant

void initTableView(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   int monitorNumber = curMonitor->_monitorNumber;

   FeatureTableModel * tableModel = new FeatureTableModel(baseModel);

   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                    tableModel, SLOT(  startInitialLoad()));
   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                    tableModel, SLOT(  endInitialLoad()));

   curMonitor->setFeatureTableModel(tableModel);

   QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   sizePolicy1.setHorizontalStretch(1);
   sizePolicy1.setVerticalStretch(1);
   // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
   sizePolicy1.setHeightForWidth(false);

   // Layout stacked widget page page_table_view, contains vcp_tableView
   QWidget * page_table_view = new QWidget();

   // sizePolicy1.setHeightForWidth(page_table_view->sizePolicy().hasHeightForWidth());
   page_table_view->setSizePolicy(sizePolicy1);

   QHBoxLayout *
   horizontalLayout = new QHBoxLayout(page_table_view);
   horizontalLayout->setSpacing(6);
   horizontalLayout->setContentsMargins(11, 11, 11, 11);
   horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

   QTableView *
   vcp_tableView = new QTableView(page_table_view);

   QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
   sizePolicy4.setHorizontalStretch(1);
   sizePolicy4.setVerticalStretch(1);
   // sizePolicy4.setHeightForWidth(vcp_tableView->sizePolicy().hasHeightForWidth());
   sizePolicy4.setHeightForWidth(false);
   vcp_tableView->setSizePolicy(sizePolicy4);
   vcp_tableView->setMinimumSize(QSize(700, 0));     // was 561,0

   vcp_tableView->setSelectionMode(QAbstractItemView::NoSelection);
   vcp_tableView->setSortingEnabled(false);

   horizontalLayout->addWidget(vcp_tableView);

   int pageno = stackedWidget->count();

   page_table_view->setObjectName(QString::asprintf("page_table_view-%d-pageno-%d",
         monitorNumber, pageno));
   vcp_tableView->setObjectName(QString::asprintf("vcp_tableView-%d-pageno-%d",
         monitorNumber, pageno));

   stackedWidget->addWidget(page_table_view);
   curMonitor->_pageno_table_view = pageno;
   curMonitor->_page_table_view = page_table_view;
   curMonitor->_vcp_tableView = vcp_tableView;

}

#ifdef UNUSED
// Ugh: class method to locate the showCentralWidgetByWidget slot

// Sets curMonitor->_page_features_scrollarea
//      curMonitor->_featuresScrollAreaContents
//                  _pageno features scroll area

QWidget * MainWindow::initFeaturesScrollArea(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
        //
        // Per-monitor permanent scroll area
        //

        printf("(MainWindow::%s) Allocating per-monitor features scrollarea page\n", __func__); fflush(stdout);


        QScrollArea *   featuresScrollArea = new QScrollArea();

        FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
        QVBoxLayout* vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName("created in initDisplaySelector");
        scrollAreaContents->setLayout(vboxLayout);

        curMonitor->_page_features_scrollarea   = featuresScrollArea;
        curMonitor->_featuresScrollAreaContents = scrollAreaContents;  // n. constructor sets the layout

        featuresScrollArea->setWidget(scrollAreaContents);
        scrollAreaContents->setContainingScrollArea(featuresScrollArea);

        int pageno = stackedWidget->count();
        curMonitor->_pageno_features_scrollarea = pageno;
        stackedWidget->addWidget(featuresScrollArea);

        featuresScrollArea->setObjectName(
            QString::asprintf("features_scrollarea-%d-pageno-%d", curMonitor->_monitorNumber,pageno));
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
#endif


// Sets curMonitor->featuresScrollAreaView
// on endInitialLoad, creates new scrollarea, scrollareacontents, does not set them in Monitor

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

   QObject::connect(featuresView, &FeaturesScrollAreaView::signalVcpRequest,
                    curMonitor,   &Monitor::putVcpRequest);


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

    ddca_start_capture(DDCA_CAPTURE_NOOPTS);
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Output_Level saved_ol = ddca_get_output_level();
    ddca_set_output_level(DDCA_OL_VERBOSE);
    ddca_dbgrpt_display_info(dinfo, 0);
    ddca_set_output_level(saved_ol);
    char * s = ddca_end_capture();


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
    char * caps = NULL;
    char * caps_report = NULL;
    DDCA_Capabilities * parsed_caps;
    // if (m_curView != View::CapabilitiesView) {
    _curView = View::CapabilitiesView;
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
    DDCA_Display_Ref dref = dinfo->dref;
    DDCA_Display_Handle dh = NULL;
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

    // }

    if (dh) {
       rc = ddca_close_display(dh);
       if (rc != 0)
          reportDdcApiError("ddca_open_display", rc);
    }

    return;
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

    QTableView * tview = monitor->_vcp_tableView;
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

    int pageno = monitor->_pageno_table_view;
    // ui->viewsStackedWidget->setCurrentIndex(pageno);  // alt setCurrentWidget(tview)
    ui->viewsStackedWidget->setCurrentWidget(tview);
    ui->viewsStackedWidget->show();
}


void MainWindow::on_actionFeaturesListView_triggered()
{
    std::cout << "(MainWindow::on_actionFeaturesListView()" << endl;
    // if (_curView != View::FeaturesView) {
    //    _curView = View::FeaturesView;
    //    // ???
    // }
    // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
    int monitorNdx = _toolbarDisplayCB->currentIndex();
    Monitor * monitor = monitors[monitorNdx];
    monitor->_curFeaturesView = Monitor::FEATURES_VIEW_LISTVIEW;
    loadMonitorFeatures(monitor);

    QListView * lview = monitor->vcp_listView;
    lview->setModel(monitor->_listModel);

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
