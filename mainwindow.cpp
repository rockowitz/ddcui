#include "mainwindow.h"
#include "ui_mainwindow2.h"
#include <QMessageBox>
#include <ddcutil_c_api.h>
// #include "inmemoryfile.h"
#include <iostream>
#include <QFont>
#include "featureselectiondialog.h"
#include "monitor.h"
#include "vcplineitem.h"
#include "feature_item_model.h"
#include "feature_table_model.h"
#include "feature_value_tableitem_delegate.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cout << "After setupUI()" << endl;

    QLabel* toolbarDisplayLabel = new QLabel("DisplayX:  ");
    _toolbarDisplayCB = new QComboBox();
    ui->mainToolBar->addWidget( toolbarDisplayLabel);
    ui->mainToolBar->addWidget( _toolbarDisplayCB);
    ui->capabilities_plainText->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->moninfoPlainText->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    initDisplaySelector();
    feature_selector = new FeatureSelector();

#ifdef NO
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
#ifdef DOESNT_WORK
    QObject * item = nullptr;
    while((item == childs.takeAt(0))) {
        delete item;
    }
#endif

    _views_StackedWidget = new QStackedWidget(ui->centralWidget);
    _views_StackedWidget->setObjectName("views_StackedWidget");

    QWidget* page0 = new QWidget();
    page0->setObjectName("page_moninfo");
    _moninfoPlainText = new QPlainTextEdit();
    page0->setObjectName("moninfoPlainTextEdit");
    // how to add _moninfoPlainText to page0?

    QWidget* page1 = new QWidget();
    page1->setObjectName("page_table_view");
    _vcp_tableview = new QTableView();
    _vcp_tableview->setObjectName("vcp_tableview");

    _views_StackedWidget->addWidget(page0);
    _views_StackedWidget->addWidget(page1);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_views_StackedWidget);


    // ui->MainWindow.centralWidget->setLayout(layout);
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initDisplaySelector() {
   //  ui->displaySelectorComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
   //  ui->displaySelectorComboBox->setMinimumContentsLength(28);   // 2 + 3 + 3 + 3 + 13

    _dlist = ddca_get_display_info_list();
    for (int ndx = 0; ndx < _dlist->ct; ndx++) {
        QString mfg_id = _dlist->info[ndx].mfg_id;
        QString model_name = _dlist->info[ndx].model_name;
        QString s = QString::number(ndx+1) + ":  " + mfg_id + " - " + model_name;
        // ui->displaySelectorComboBox->addItem(s, QVariant(ndx+1));
        _toolbarDisplayCB->addItem(s, QVariant(ndx+1));

        Monitor * curMonitor = new Monitor(&_dlist->info[ndx]);
        monitors.append(curMonitor);

        cout << "(initDisplaySelector) Staring vcp thread" << endl;
        curMonitor->_requestQueue = new VcpRequestQueue();
        FeatureBaseModel * baseModel = new FeatureBaseModel();
        FeatureItemModel * listModel = new FeatureItemModel(baseModel);

        QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                         listModel,  SLOT(startInitialLoad()));
        QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                         listModel,  SLOT(endInitialLoad()));
        QObject::connect(baseModel,  SIGNAL(signalVcpRequest(VcpRequest*)),
                         curMonitor, SLOT(putVcpRequest(VcpRequest*)));


        FeatureTableModel * tableModel = new FeatureTableModel(baseModel);
        QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                         tableModel, SLOT(startInitialLoad()));
        QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                         tableModel, SLOT(endInitialLoad()));

        curMonitor->setFeatureItemModel(listModel);
        curMonitor->setFeatureTableModel(tableModel);
        VcpThread * curThread = new VcpThread(NULL,
                                              curMonitor->_displayInfo,
                                              curMonitor->_requestQueue,
                                          //  listModel,
                                              baseModel);
        curThread->start();
        vcp_threads.append(curThread);

    }
    // ui->displaySelectorComboBox->setCurrentIndex(0);
    _toolbarDisplayCB->setCurrentIndex(0);
    QString msg = QString("Detected ") + QString::number(_dlist->ct) + QString(" displays.");
    ui->statusBar->showMessage(msg);
}


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

    DDCA_Feature_List feature_list = monitor->getFeatureList(feature_selector->feature_list_id);

    cout << "feature_list_id: " << feature_selector->feature_list_id << ",  Feature ids: " << endl;
    for (int ndx = 0; ndx <= 255; ndx++) {
        if ( ddca_feature_list_test(&feature_list, (uint8_t) ndx)) {
            printf("%02x ", ndx);
        }

    }
    cout << endl;

    monitor->_requestQueue->put(new VcpStartInitialLoadRequest);

    for (int ndx = 0; ndx <= 255; ndx++) {
        uint8_t vcp_code = (uint8_t) ndx;
        if ( ddca_feature_list_test(&feature_list, vcp_code)) {
            // printf("%02x ");

            monitor->_requestQueue->put( new VcpGetRequest(vcp_code));
        }
    }
    monitor->_requestQueue->put(new VcpEndInitialLoadRequest);


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
}

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
        }
    }
    if (rc == 0) {
        // wrap in collector
        DDCA_Output_Level saved_ol = ddca_get_output_level();
        ddca_set_output_level(DDCA_OL_VERBOSE);
        ddca_start_capture();
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

void MainWindow::on_actionMonitor_Summary_triggered()
{
    std::cout << "(MainWindow::on_actionMonitor_Summary_triggered()" << endl;
    // if (m_curView != View::MonitorView) {
       _curView = View::MonitorView;
       // ???
      // int monitorNdx = ui->displaySelectorComboBox->currentIndex();
      int monitorNdx = _toolbarDisplayCB->currentIndex();

#ifdef OLD
      InMemoryFile * memfile = new InMemoryFile();
      FILE * f = memfile->handle();
      ddca_set_fout(f);
#endif
      ddca_start_capture();

      DDCA_Display_Info * dinfo = &_dlist->info[monitorNdx];
      DDCA_Output_Level saved_ol = ddca_get_output_level();
      ddca_set_output_level(DDCA_OL_VERBOSE);
      ddca_report_display_info(dinfo, 0);
      ddca_set_output_level(saved_ol);

#ifdef OLD
      ddca_set_fout_to_default();
      char * s = memfile->buffer();
      cout << "len: " << memfile->size() << endl;
      cout << "s: " << s << endl;
#endif
      char * s = ddca_end_capture();

      ui->moninfoPlainText->setPlainText(s);
      free(s);
      QFont fixedFont("Courier");
      ui->moninfoPlainText->setFont(fixedFont);

      ui->views_stackedWidget->setCurrentIndex(0);    // need proper constants

       ui->views_stackedWidget->show();
    // }
}

void MainWindow::on_actionFeature_Selection_triggered()
{
    // display dialog box for selecting features
    cout << "(on_actionFeatureSelection_triggered)" << endl;
}

void MainWindow::on_actionFeature_Selection_Dialog_triggered()
{
        // display dialog box for selecting features
        cout << "(on_actionFeatureSelection_Dialog_triggered)" << endl;

       FeatureSelectionDialog* fsd = new FeatureSelectionDialog(this, this->feature_selector);
       fsd->show();
}

DDCA_Feature_List_Id MainWindow::feature_list_id() const {
    return this->_feature_list_id;
}

void MainWindow::set_feature_list_id(DDCA_Feature_List_Id feature_list_id) {
    cout << "(set_feature_list_id) feature_list_id =" << feature_list_id <<endl;
    this->_feature_list_id = feature_list_id;
}

void MainWindow::on_actionFeatures_TableView_triggered()
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
            this,  SLOT(on_vcp_tableView_clicked(QModelIndex)));

    tview->setSelectionBehavior(QAbstractItemView::SelectItems);
    tview->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->views_stackedWidget->setCurrentIndex(4);
    ui->views_stackedWidget->show();
}

void MainWindow::on_actionFeaturesListView_triggered()
{
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

void MainWindow::on_vcp_tableView_clicked(const QModelIndex &index)
{
    printf("-------------> (MainWindow::on_vcp_tableView_clicked) row=%d, col=%d\n", index.row(), index.column() );
}

void MainWindow::on_vcp_tableView_doubleClicked(const QModelIndex &index)
{
     printf("----------> (MainWindow::on_vcp_tableView_doubleClicked) row=%d, col=%d\n", index.row(), index.column() );
}
