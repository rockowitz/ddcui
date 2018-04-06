/* ui_main.cpp */

#include "ui_main.h"
#include <QtWidgets/QApplication>


    void Ui_MainWindow::setupUi()
    {
                QMainWindow * MainWindow = this;

        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 400);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAboutQt = new QAction(MainWindow);
        actionAboutQt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionMonitorSummary = new QAction(MainWindow);
        actionMonitorSummary->setObjectName(QString::fromUtf8("actionMonitor_Summary"));
        actionCapabilities = new QAction(MainWindow);
        actionCapabilities->setObjectName(QString::fromUtf8("actionCapabilities"));
        actionFeaturesListView = new QAction(MainWindow);
        actionFeaturesListView->setObjectName(QString::fromUtf8("actionFeaturesListView"));
        actionFeatureSelection = new QAction(MainWindow);
        actionFeatureSelection->setObjectName(QString::fromUtf8("actionFeature_Selection"));
        actionShowUnsupportedFeatures = new QAction(MainWindow);
        actionShowUnsupportedFeatures->setObjectName(QString::fromUtf8("actionShow_Unsupported_Features"));
        actionShowUnsupportedFeatures->setCheckable(true);
        actionKnown = new QAction(MainWindow);
        actionKnown->setObjectName(QString::fromUtf8("actionKnown"));
        actionScan = new QAction(MainWindow);
        actionScan->setObjectName(QString::fromUtf8("actionScan"));
        actionColor = new QAction(MainWindow);
        actionColor->setObjectName(QString::fromUtf8("actionColor"));
        actionProfile = new QAction(MainWindow);
        actionProfile->setObjectName(QString::fromUtf8("actionProfile"));
        actionManufacturer = new QAction(MainWindow);
        actionManufacturer->setObjectName(QString::fromUtf8("actionManufacturer"));
        actionFeatureSelectionDialog = new QAction(MainWindow);
        actionFeatureSelectionDialog->setObjectName(QString::fromUtf8("actionFeature_Selection_Dialog"));
        actionFeaturesTableView = new QAction(MainWindow);
        actionFeaturesTableView->setObjectName(QString::fromUtf8("actionFeatures_TableView"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(800, 0));
        centralWidget->setMaximumSize(QSize(800, 16777215));
        centralWidget->setSizeIncrement(QSize(10, 10));
        views_stackedWidget = new QStackedWidget(centralWidget);
        views_stackedWidget->setObjectName(QString::fromUtf8("views_stackedWidget"));
        views_stackedWidget->setGeometry(QRect(2, 42, 618, 206));
        sizePolicy1.setHeightForWidth(views_stackedWidget->sizePolicy().hasHeightForWidth());
        views_stackedWidget->setSizePolicy(sizePolicy1);
        views_stackedWidget->setFrameShape(QFrame::Panel);
        views_stackedWidget->setFrameShadow(QFrame::Sunken);
        page_moninfo = new QWidget();
        page_moninfo->setObjectName(QString::fromUtf8("page_moninfo"));
        sizePolicy1.setHeightForWidth(page_moninfo->sizePolicy().hasHeightForWidth());
        page_moninfo->setSizePolicy(sizePolicy1);
        moninfoPlainText = new QPlainTextEdit(page_moninfo);
        moninfoPlainText->setObjectName(QString::fromUtf8("moninfoPlainText"));
        moninfoPlainText->setGeometry(QRect(6, 6, 574, 191));
        sizePolicy1.setHeightForWidth(moninfoPlainText->sizePolicy().hasHeightForWidth());
        moninfoPlainText->setSizePolicy(sizePolicy1);
        moninfoPlainText->setMaximumSize(QSize(574, 16777215));
        moninfoPlainText->setLineWrapMode(QPlainTextEdit::NoWrap);
        moninfoPlainText->setReadOnly(true);
        views_stackedWidget->addWidget(page_moninfo);
        page_capabilities = new QWidget();
        page_capabilities->setObjectName(QString::fromUtf8("page_capabilities"));
        sizePolicy1.setHeightForWidth(page_capabilities->sizePolicy().hasHeightForWidth());
        page_capabilities->setSizePolicy(sizePolicy1);
        capabilities_plainText = new QPlainTextEdit(page_capabilities);
        capabilities_plainText->setObjectName(QString::fromUtf8("capabilities_plainText"));
        capabilities_plainText->setGeometry(QRect(16, 6, 574, 231));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(capabilities_plainText->sizePolicy().hasHeightForWidth());
        capabilities_plainText->setSizePolicy(sizePolicy2);
        capabilities_plainText->setMaximumSize(QSize(574, 16777215));
        capabilities_plainText->setReadOnly(true);
        capabilities_plainText->setCenterOnScroll(false);
        views_stackedWidget->addWidget(page_capabilities);
        page_vcp = new QWidget();
        page_vcp->setObjectName(QString::fromUtf8("page_vcp"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(page_vcp->sizePolicy().hasHeightForWidth());
        page_vcp->setSizePolicy(sizePolicy3);
        vcpListWidget = new QListWidget(page_vcp);
        vcpListWidget->setObjectName(QString::fromUtf8("vcp_listWidget"));
        vcpListWidget->setGeometry(QRect(5, 1, 771, 251));
        sizePolicy3.setHeightForWidth(vcpListWidget->sizePolicy().hasHeightForWidth());
        vcpListWidget->setSizePolicy(sizePolicy3);
        views_stackedWidget->addWidget(page_vcp);
        page_table_item = new QWidget();
        page_table_item->setObjectName(QString::fromUtf8("page_table_item"));
        sizePolicy1.setHeightForWidth(page_table_item->sizePolicy().hasHeightForWidth());
        page_table_item->setSizePolicy(sizePolicy1);
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
        sizePolicy3.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy3);
        tableWidget->setMinimumSize(QSize(581, 0));
        tableWidget->setColumnCount(5);
        views_stackedWidget->addWidget(page_table_item);
        page_table_view = new QWidget();
        page_table_view->setObjectName(QString::fromUtf8("page_table_view"));
        sizePolicy1.setHeightForWidth(page_table_view->sizePolicy().hasHeightForWidth());
        page_table_view->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(page_table_view);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        vcp_tableView = new QTableView(page_table_view);
        vcp_tableView->setObjectName(QString::fromUtf8("vcp_tableView"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(1);
        sizePolicy4.setHeightForWidth(vcp_tableView->sizePolicy().hasHeightForWidth());
        vcp_tableView->setSizePolicy(sizePolicy4);
        vcp_tableView->setMinimumSize(QSize(561, 0));
        vcp_tableView->setSelectionMode(QAbstractItemView::NoSelection);
        vcp_tableView->setSortingEnabled(false);

        horizontalLayout->addWidget(vcp_tableView);

        views_stackedWidget->addWidget(page_table_view);
        page_list_view = new QWidget();
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
        vcp_listView = new QListView(page_list_view);
        vcp_listView->setObjectName(QString::fromUtf8("vcp_listView"));
        vcp_listView->setGeometry(QRect(6, 6, 256, 192));
        sizePolicy5.setHeightForWidth(vcp_listView->sizePolicy().hasHeightForWidth());
        vcp_listView->setSizePolicy(sizePolicy5);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Monospace"));
        font1.setPointSize(10);
        vcp_listView->setFont(font1);
        label = new QLabel(page_list_view);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(268, 6, 342, 17));
        views_stackedWidget->addWidget(page_list_view);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 30));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuDisplays = new QMenu(menuBar);
        menuDisplays->setObjectName(QString::fromUtf8("menuDisplays"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuDisplays->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuView->addAction(actionMonitorSummary);
        menuView->addAction(actionCapabilities);
        menuView->addAction(actionFeaturesListView);
        menuView->addAction(actionFeaturesTableView);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAboutQt);
        menuOptions->addAction(actionFeatureSelectionDialog);

        this->retranslateUi();

        views_stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void Ui_MainWindow::retranslateUi()
    {
        QMainWindow * MainWindow = this;
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ddcutil - mainWindow", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "&About ddcui", 0));
        actionAboutQt->setText(QApplication::translate("MainWindow", "About &Qt", 0));
        actionMonitorSummary->setText(QApplication::translate("MainWindow", "&Monitor Summary", 0));
        actionCapabilities->setText(QApplication::translate("MainWindow", "&Capabilities", 0));
        actionFeaturesListView->setText(QApplication::translate("MainWindow", "&Features - ListView", 0));
        actionFeatureSelection->setText(QApplication::translate("MainWindow", "Feature Selection", 0));
        actionShowUnsupportedFeatures->setText(QApplication::translate("MainWindow", "Show Unsupported Features", 0));
        actionKnown->setText(QApplication::translate("MainWindow", "Known", 0));
        actionScan->setText(QApplication::translate("MainWindow", "Scan", 0));
        actionColor->setText(QApplication::translate("MainWindow", "Color", 0));
        actionProfile->setText(QApplication::translate("MainWindow", "Profile", 0));
        actionManufacturer->setText(QApplication::translate("MainWindow", "Manufacturer", 0));
        actionFeatureSelectionDialog->setText(QApplication::translate("MainWindow", "&Feature Selection", 0));
        actionFeaturesTableView->setText(QApplication::translate("MainWindow", "F&eatures - TableView", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Code", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Name", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Type", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "RW", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Value", 0));
        label->setText(QApplication::translate("MainWindow", "Header line for page_list_view containing vcp_listView", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "&View", 0));
        menuDisplays->setTitle(QApplication::translate("MainWindow", "Disp&lays", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Optio&ns", 0));
    } // retranslateUi



