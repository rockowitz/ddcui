/* ui_mainwindow2.h */

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <QtCore/QVariant>
#include <QtWidgets/QtWidgets>
#ifdef OLD
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#endif
#include <iostream>

#include "feature_list_widget.h"
#include "feature_value_widgets/value_std_widget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionAboutQt;

    QAction *actionMonitorSummary;
    QAction *actionCapabilities;

    QAction *actionFeaturesListWidget;        // for ListWidget
    QAction *actionFeaturesListView;
    QAction *actionFeaturesScrollArea;
    QAction *actionFeaturesTableView;

    QAction *actionFeatureSelection;
    QAction *actionShowUnsupportedFeatures;
    QAction *actionKnown;
    QAction *actionScan;
    QAction *actionColor;
    QAction *actionProfile;
    QAction *actionManufacturer;
    QAction *actionFeatureSelectionDialog;

    QWidget *centralWidget;   
    QStackedWidget *views_stackedWidget;

    QWidget *page_moninfo;
    QPlainTextEdit *moninfoPlainText;
    int     _pageno_moninfo;

    QWidget *page_capabilities;
    QPlainTextEdit *capabilities_plainText;
    int _pageno_capabilities;

    QWidget *page_vcp;
    QListWidget *vcpListWidget;
    int _pageno_vcp;

    QWidget *page_table_item;
    QTableWidget *tableWidget;
    int _pageno_table_item;

    QWidget *page_table_view;
    int _pageno_table_view;
    QTableView *vcp_tableView;

    QWidget *page_list_view;
    QListView *vcp_listView;
    int _pageno_list_view;

    QWidget *page_list_widget;
    // QListWidget *feature_listWidget;
    FeatureListWidget * feature_listWidget;
    int _pageno_list_widget;

    // or should this be per monitor?

    QWidget *     featuresScrollAreaContents;
    // QScrollArea * featuresScrollArea;
    QWidget *     page_features_scrollarea;
    int           _pageno_scrollarea;

    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuDisplays;
    QMenu *menuHelp;
    QMenu *menuOptions;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

private:
    void initActions(QMainWindow * MainWindow){

        // Help Menu Actions

        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));

        actionAboutQt = new QAction(MainWindow);
        actionAboutQt->setObjectName(QString::fromUtf8("actionAbout_Qt"));


        // View Menu Actions
        // n. QMetaObject::connectSlotsByName() depends on object name,
        // must match action_<objname>_triggered()

        actionMonitorSummary = new QAction(MainWindow);
        actionMonitorSummary->setObjectName(QString::fromUtf8("actionMonitorSummary"));

        actionCapabilities = new QAction(MainWindow);
        actionCapabilities->setObjectName(QString::fromUtf8("actionCapabilities"));

        actionFeaturesListWidget = new QAction(MainWindow);
        actionFeaturesListWidget->setObjectName(QString::fromUtf8("actionFeaturesListWidget"));

        actionFeaturesListView = new QAction(MainWindow);
        actionFeaturesListView->setObjectName(QString::fromUtf8("actionFeaturesListView"));

        actionFeatureSelection = new QAction(MainWindow);
        actionFeatureSelection->setObjectName(QString::fromUtf8("actionFeatureSelection"));

        actionFeaturesTableView = new QAction(MainWindow);
        actionFeaturesTableView->setObjectName(QString::fromUtf8("actionFeaturesTableView"));

        actionFeaturesScrollArea = new QAction(MainWindow);
        actionFeaturesScrollArea->setObjectName(QString::fromUtf8("actionFeaturesScrollArea"));


        // Options->Feature Selection Dialog Actions

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
        actionFeatureSelectionDialog->setObjectName(QString::fromUtf8("actionFeatureSelectionDialog"));
    }

private:
    void setupMenus(QMainWindow *MainWindow)
    {
       // Menu Bar
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


       // Menu Bar actions
       menuBar->addAction(menuView->menuAction());
       menuBar->addAction(menuDisplays->menuAction());
       menuBar->addAction(menuHelp->menuAction());
       menuBar->addAction(menuOptions->menuAction());

       menuView->addAction(actionMonitorSummary);
       menuView->addAction(actionCapabilities);
       menuView->addAction(actionFeaturesListWidget);
       menuView->addAction(actionFeaturesListView);
       menuView->addAction(actionFeaturesTableView);
       menuView->addAction(actionFeaturesScrollArea);

       menuHelp->addAction(actionAbout);
       menuHelp->addAction(actionAboutQt);

       menuOptions->addAction(actionFeatureSelectionDialog);


       // Tool Bar
       mainToolBar = new QToolBar(MainWindow);
       mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
       MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

       // Status Bar
       statusBar = new QStatusBar(MainWindow);
       statusBar->setObjectName(QString::fromUtf8("statusBar"));
       MainWindow->setStatusBar(statusBar);
    }


    void layoutCentralWidget(QMainWindow *MainWindow)
    {     //       std::cout << "(setupUi) Wolf 1" << std::endl;

       centralWidget = new QWidget(MainWindow);
       centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

       QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
       sizePolicy1.setHorizontalStretch(1);
       sizePolicy1.setVerticalStretch(1);
       // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
       sizePolicy1.setHeightForWidth(false);
       centralWidget->setSizePolicy(sizePolicy1);

       centralWidget->setMinimumSize(QSize(600, 200));            // was 800,0
       centralWidget->setMaximumSize(QSize(2000, 16777215));
       centralWidget->setSizeIncrement(QSize(10, 10));

       views_stackedWidget = new QStackedWidget(centralWidget);
       views_stackedWidget->setObjectName(QString::fromUtf8("views_stackedWidget"));

       views_stackedWidget->setGeometry(QRect(2, 42, 780, 300));
       // sizePolicy1.setHeightForWidth(views_stackedWidget->sizePolicy().hasHeightForWidth());
       views_stackedWidget->setSizePolicy(sizePolicy1);
       views_stackedWidget->setFrameShape(QFrame::Panel);
       views_stackedWidget->setFrameShadow(QFrame::Sunken);

       QHBoxLayout *
       centralWidgetLayout = new QHBoxLayout(centralWidget);
       centralWidgetLayout->setSpacing(6);
       // centralWidgetLayout->setContentsMargins(11, 11, 11, 11);
       centralWidgetLayout->setObjectName(QString::fromUtf8("centralWidgetLayout"));
       centralWidgetLayout->addWidget(views_stackedWidget);


       int pagectr = 0;

       // Layout stacked widget page: page_moninfo, contains moninfoPlainText
       page_moninfo = new QWidget();
       page_moninfo->setObjectName(QString::fromUtf8("page_moninfo"));

       // sizePolicy1.setHeightForWidth(page_moninfo->sizePolicy().hasHeightForWidth());
       page_moninfo->setSizePolicy(sizePolicy1);

       moninfoPlainText = new QPlainTextEdit(page_moninfo);
       moninfoPlainText->setObjectName(QString::fromUtf8("moninfoPlainText"));

       moninfoPlainText->setGeometry(QRect(6, 6, 700, 191));   // was 574,191
       // sizePolicy1.setHeightForWidth(moninfoPlainText->sizePolicy().hasHeightForWidth());
       moninfoPlainText->setSizePolicy(sizePolicy1);
       moninfoPlainText->setMaximumSize(QSize(2000, 16777215));   // 574->2000
       moninfoPlainText->setLineWrapMode(QPlainTextEdit::NoWrap);
       moninfoPlainText->setReadOnly(true);

       views_stackedWidget->addWidget(page_moninfo);
       _pageno_moninfo = pagectr++;

         //             std::cout << "(setupUi) Wolf 5" << std::endl;
#ifdef NO
       QHBoxLayout *
       moninfoLayout = new QHBoxLayout(views_stackedWidget);  // views_StackedWidget already has a layout
       // moninfoLayout->setContentsMargins(11, 11, 11, 11);
       moninfoLayout->setObjectName(QString::fromUtf8("moninfoLayout"));
       moninfoLayout->addWidget(page_moninfo);
#endif
           //           std::cout << "(setupUi) Wolf 6" << std::endl;

       // AMEN!
       QHBoxLayout *
       pageMoninfoLayout = new QHBoxLayout(page_moninfo);
       pageMoninfoLayout->setSpacing(6);
       // pageMoninfoLayout->setContentsMargins(11, 11, 11, 11);
       pageMoninfoLayout->setObjectName(QString::fromUtf8("pageMoninfoLayout"));
       pageMoninfoLayout->addWidget(moninfoPlainText);


       // Layout stacked widget page page_capabilities, contains capabilities_plainText
       page_capabilities = new QWidget();
       page_capabilities->setObjectName(QString::fromUtf8("page_capabilities"));
       // sizePolicy1.setHeightForWidth(page_capabilities->sizePolicy().hasHeightForWidth());
       page_capabilities->setSizePolicy(sizePolicy1);
       capabilities_plainText = new QPlainTextEdit(page_capabilities);
       capabilities_plainText->setObjectName(QString::fromUtf8("capabilities_plainText"));
       capabilities_plainText->setGeometry(QRect(16, 6, 700, 231));   // was 574,231
       // QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
       // sizePolicy2.setHorizontalStretch(1);
       // sizePolicy2.setVerticalStretch(1);       // was 0
       // sizePolicy2.setHeightForWidth(capabilities_plainText->sizePolicy().hasHeightForWidth());
       capabilities_plainText->setSizePolicy(sizePolicy1);     // was sizePolicy1
       capabilities_plainText->setMaximumSize(QSize(2000, 16777215));  // was 574
       capabilities_plainText->setReadOnly(true);
       capabilities_plainText->setCenterOnScroll(false);
       views_stackedWidget->addWidget(page_capabilities);
       _pageno_capabilities = pagectr++;

       // AMEN!
       QHBoxLayout *
       pageCapabilitiesLayout = new QHBoxLayout(page_capabilities);
       pageCapabilitiesLayout->setSpacing(6);
       // pageCapabilitiesLayout->setContentsMargins(11, 11, 11, 11);
       pageCapabilitiesLayout->setObjectName(QString::fromUtf8("pageCapabilitiesLayout"));
       pageCapabilitiesLayout->addWidget(capabilities_plainText);


       // Layout stacked widget page page_vcp, contains vcp_listWidget
       page_vcp = new QWidget();
       page_vcp->setObjectName(QString::fromUtf8("page_vcp"));
       QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
       sizePolicy3.setHorizontalStretch(1);
       sizePolicy3.setVerticalStretch(1);
       // sizePolicy3.setHeightForWidth(page_vcp->sizePolicy().hasHeightForWidth());
       sizePolicy3.setHeightForWidth(false);
       page_vcp->setSizePolicy(sizePolicy3);
       vcpListWidget = new QListWidget(page_vcp);
       vcpListWidget->setObjectName(QString::fromUtf8("vcpListWidget"));
       vcpListWidget->setGeometry(QRect(5, 1, 771, 251));
       // sizePolicy3.setHeightForWidth(vcp_listWidget->sizePolicy().hasHeightForWidth());
       vcpListWidget->setSizePolicy(sizePolicy3);
       views_stackedWidget->addWidget(page_vcp);
       _pageno_vcp = pagectr++;


       // Layout stacked widget page page_table_item, contains tableWidget
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
       // sizePolicy3.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
       tableWidget->setSizePolicy(sizePolicy3);
       tableWidget->setMinimumSize(QSize(581, 0));
       tableWidget->setColumnCount(5);
       views_stackedWidget->addWidget(page_table_item);
       _pageno_table_item = pagectr++;


       // Layout stacked widget page page_table_view, contains vcp_tableView
       page_table_view = new QWidget();
       page_table_view->setObjectName(QString::fromUtf8("page_table_view"));

       // sizePolicy1.setHeightForWidth(page_table_view->sizePolicy().hasHeightForWidth());
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
       // sizePolicy4.setHeightForWidth(vcp_tableView->sizePolicy().hasHeightForWidth());
       sizePolicy4.setHeightForWidth(false);
       vcp_tableView->setSizePolicy(sizePolicy4);
       vcp_tableView->setMinimumSize(QSize(700, 0));     // was 561,0
       vcp_tableView->setSelectionMode(QAbstractItemView::NoSelection);
       vcp_tableView->setSortingEnabled(false);

       horizontalLayout->addWidget(vcp_tableView);
       views_stackedWidget->addWidget(page_table_view);
       _pageno_table_view = pagectr++;


       // Layout stacked widget page page_list_view, contains vcp_listview
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
       _pageno_list_view = pagectr++;


       // Layout stacked widget page page_list_widget, contains vcp_feature_listwidget
#ifdef REF
       QWidget *page_list_widget;
       QListWidget *feature_listWidget;
       int _pageno_list_widget;
#endif
       page_list_widget = new QWidget();
       page_list_widget->setObjectName(QString::fromUtf8("page_list_widget"));

       // TODO: size, font, etc

       // feature_listWidget = new QListWidget(page_list_widget);
       feature_listWidget = new FeatureListWidget(page_list_widget);
       feature_listWidget->setObjectName(QString::fromUtf8("feature_listWidget"));

       views_stackedWidget->addWidget(page_list_widget);
       _pageno_list_widget = pagectr++;


#ifdef REF
       // or should this be per monitor?
       QWidget *page_features_scrollarea;
       QWidget * featuresScrollAreaContents;
       int _pageno_scrollarea;
#endif

       page_features_scrollarea = new QWidget();
       page_features_scrollarea->setObjectName(QString::fromUtf8("page_features_scrollarea"));

       // featuresScrollAreaContents = new QWidget(page_features_scrollarea);
       // featuresScrollAreaContents->setObjectName(QString::fromUtf8("featuresScrollAreaContents"));

       // try eliminating wrapping the "real" Widget in a page widget
       featuresScrollAreaContents = page_features_scrollarea;   // works

       // featuresScrollArea = new QScrollArea(page_features_scrollarea);
       // featuresScrollArea->setObjectName(QString::fromUtf8("featuresScrollArea"));
       // featuresScrollArea->setWidget(featuresScrollAreaContents);

       // views_stackedWidget->addWidget(page_features_scrollarea_contents);

       views_stackedWidget->addWidget(page_features_scrollarea);
       _pageno_scrollarea = pagectr++;
       printf("(%s) _pageno_scrollarea=%d\n", __func__, _pageno_scrollarea); fflush(stdout);


       MainWindow->setCentralWidget(centralWidget);
    }


public:
    void setupUi(QMainWindow *MainWindow)
    {
        std::cout << "(setupUi) Starting" << std::endl;
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 400);

        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        // sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        sizePolicy.setHeightForWidth(false);
        MainWindow->setSizePolicy(sizePolicy);

        initActions(MainWindow);

        layoutCentralWidget(MainWindow);

        setupMenus(MainWindow);

        retranslateUi(MainWindow);

        views_stackedWidget->setCurrentIndex(0);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi


    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ddcutil - mainWindow", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "&About ddcui", 0));
        actionAboutQt->setText(QApplication::translate("MainWindow", "About &Qt", 0));

        actionMonitorSummary->setText(QApplication::translate("MainWindow", "&Monitor Summary", 0));
        actionCapabilities->setText(QApplication::translate("MainWindow", "&Capabilities", 0));

        actionFeaturesListWidget->setText(QApplication::translate("MainWindow", "&Features - ListWidget", 0));
        actionFeaturesScrollArea->setText(QApplication::translate("MainWindow", "Features - &ScrollArea", 0));
        actionFeaturesListView->setText(  QApplication::translate("MainWindow", "&Features - ListView",   0));
        actionFeaturesTableView->setText( QApplication::translate("MainWindow", "F&eatures - TableView",  0));

        actionFeatureSelection->setText(QApplication::translate("MainWindow", "Feature Selection", 0));
        actionShowUnsupportedFeatures->setText(QApplication::translate("MainWindow", "Show Unsupported Features", 0));
        actionKnown->setText(QApplication::translate("MainWindow", "Known", 0));
        actionScan->setText(QApplication::translate("MainWindow", "Scan", 0));
        actionColor->setText(QApplication::translate("MainWindow", "Color", 0));
        actionProfile->setText(QApplication::translate("MainWindow", "Profile", 0));
        actionManufacturer->setText(QApplication::translate("MainWindow", "Manufacturer", 0));
        actionFeatureSelectionDialog->setText(QApplication::translate("MainWindow", "&Feature Selection", 0));



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

        menuView->setTitle(    QApplication::translate("MainWindow", "&View", 0));
        menuDisplays->setTitle(QApplication::translate("MainWindow", "Disp&lays", 0));
        menuHelp->setTitle(    QApplication::translate("MainWindow", "Help", 0));
        menuOptions->setTitle( QApplication::translate("MainWindow", "Optio&ns", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
