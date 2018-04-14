/* ui_mainwindow2.h */

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <QtCore/QVariant>
// #include <QtWidgets/QtWidgets>
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

#include "base/debug_utils.h"

// #include "feature_value_widgets/value_std_widget.h"

// #include "list_widget/feature_list_widget.h"
// #include "feature_scrollarea/features_scroll_area_contents.h"


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
    QAction *actionFeaturesTableView;
    QAction *actionFeaturesScrollArea;
    QAction *actionFeaturesScrollAreaMock;

    QAction *actionFeatureSelection;
    QAction *actionShowUnsupportedFeatures;
    QAction *actionKnown;
    QAction *actionScan;
    QAction *actionColor;
    QAction *actionProfile;
    QAction *actionManufacturer;
    QAction *actionFeatureSelectionDialog;

#define COMBINED_CENTRAL_WIDGET
#ifndef COMBINED_CENTRAL_WIDGET
    QWidget *centralWidget;
#else
    QStackedWidget * centralWidget;
#endif
    QStackedWidget *viewsStackedWidget;


    QHBoxLayout *horizontalLayout;
    // QLabel *label;
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

        actionFeaturesScrollAreaMock = new QAction(MainWindow);
        actionFeaturesScrollAreaMock->setObjectName(QString::fromUtf8("actionFeaturesScrollAreaMock"));


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


    void retranslateFeatureSelectionDialog()
    {
       actionFeatureSelection->setText(       QApplication::translate("MainWindow", "Feature Selection", 0));
       actionShowUnsupportedFeatures->setText(QApplication::translate("MainWindow", "Show Unsupported Features", 0));
       actionKnown->setText(                  QApplication::translate("MainWindow", "Known", 0));
       actionScan->setText(                   QApplication::translate("MainWindow", "Scan", 0));
       actionColor->setText(                  QApplication::translate("MainWindow", "Color", 0));
       actionProfile->setText(                QApplication::translate("MainWindow", "Profile", 0));
       actionManufacturer->setText(           QApplication::translate("MainWindow", "Manufacturer", 0));
       actionFeatureSelectionDialog->setText( QApplication::translate("MainWindow", "&Feature Selection", 0));
    }


    void setupMenus(QMainWindow *MainWindow)
    {
       // Menu Bar
       menuBar = new QMenuBar(MainWindow);
       menuBar->setObjectName(QString::fromUtf8("menuBar"));
       menuBar->setGeometry(QRect(0, 0, 800, 30));

       menuView = new QMenu(menuBar);
       menuView->setObjectName(QString::fromUtf8("menuView"));
       menuView->setTitle(    QApplication::translate("MainWindow", "&View", 0));

       menuDisplays = new QMenu(menuBar);
       menuDisplays->setObjectName(QString::fromUtf8("menuDisplays"));
       menuDisplays->setTitle(QApplication::translate("MainWindow", "Disp&lays", 0));

       menuHelp = new QMenu(menuBar);
       menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
       menuHelp->setTitle(    QApplication::translate("MainWindow", "Help", 0));

       menuOptions = new QMenu(menuBar);
       menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
       menuOptions->setTitle( QApplication::translate("MainWindow", "Optio&ns", 0));
       menuOptions->addAction(actionFeatureSelectionDialog);

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
       menuView->addAction(actionFeaturesScrollAreaMock);

       actionMonitorSummary->setText(    QApplication::translate("MainWindow", "&Monitor Summary", 0));
       actionCapabilities->setText(      QApplication::translate("MainWindow", "&Capabilities", 0));
       actionFeaturesListWidget->setText(QApplication::translate("MainWindow", "&Features - ListWidget", 0));
       actionFeaturesListView->setText(  QApplication::translate("MainWindow", "&Features - ListView",   0));
       actionFeaturesTableView->setText( QApplication::translate("MainWindow", "F&eatures - TableView",  0));
       actionFeaturesScrollArea->setText(QApplication::translate("MainWindow", "Features - &ScrollArea", 0));
       actionFeaturesScrollAreaMock->setText(
                                         QApplication::translate("MainWindow", "Features - &MockScrollArea", 0));
       menuHelp->addAction(actionAbout);
       menuHelp->addAction(actionAboutQt);

       actionAbout->setText(QApplication::translate(  "MainWindow", "&About ddcui", 0));
       actionAboutQt->setText(QApplication::translate("MainWindow", "About &Qt", 0));


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

#ifndef COMBINED_CENTRAL_WIDGET
       centralWidget = new QWidget(MainWindow);
       centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
       viewsStackedWidget = new QStackedWidget(centralWidget);
       viewsStackedWidget->setObjectName(QString::fromUtf8("views_stackedWidget"));
#else
       centralWidget = new QStackedWidget(MainWindow);
       centralWidget->setObjectName(QString::fromUtf8("centralWidget/views_stackedWidget"));
       viewsStackedWidget = centralWidget;
#endif

       QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
       sizePolicy1.setHorizontalStretch(1);
       sizePolicy1.setVerticalStretch(1);
       // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
       sizePolicy1.setHeightForWidth(false);
       centralWidget->setSizePolicy(sizePolicy1);

       centralWidget->setMinimumSize(QSize(600, 200));            // was 800,0
       centralWidget->setMaximumSize(QSize(2000, 16777215));
       centralWidget->setSizeIncrement(QSize(10, 10));

       viewsStackedWidget->setFrameShape(QFrame::Panel);
       viewsStackedWidget->setFrameShadow(QFrame::Sunken);
#ifndef COMBINED_CENTRAL_WIDGET
       viewsStackedWidget->setGeometry(QRect(2, 42, 780, 300));
       // sizePolicy1.setHeightForWidth(views_stackedWidget->sizePolicy().hasHeightForWidth());
       viewsStackedWidget->setSizePolicy(sizePolicy1);

       QHBoxLayout *
       centralWidgetLayout = new QHBoxLayout(centralWidget);
       centralWidgetLayout->setSpacing(6);
       // centralWidgetLayout->setContentsMargins(11, 11, 11, 11);
       centralWidgetLayout->setObjectName(QString::fromUtf8("centralWidgetLayout"));
       centralWidgetLayout->addWidget(viewsStackedWidget);
#endif

       MainWindow->setCentralWidget(centralWidget);
       // reportWidgetChildren(centralWidget, "Children of centralWidget:");
       // reportWidgetChildren(viewsStackedWidget, "Children of viewsStackedWidget:");
       // printf("(%s) Number of widgets contained by viewsStackedWidget: %d\n",
       //        __func__, viewsStackedWidget->count() );  fflush(stdout);
    }


public:
    void setupUi(QMainWindow *MainWindow)
    {
        std::cout << "(setupUi) Starting" << std::endl;
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 400);

        // set MainWindow size policy
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        // sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        sizePolicy.setHeightForWidth(false);
        MainWindow->setSizePolicy(sizePolicy);

        initActions(MainWindow);

        layoutCentralWidget(MainWindow);

        setupMenus(MainWindow);

        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ddcutil - mainWindow", 0));
        retranslateFeatureSelectionDialog();

        viewsStackedWidget->setCurrentIndex(0);

        QMetaObject::connectSlotsByName(MainWindow);

        std::cout << "(setupUi) Done" << std::endl;
    } // setupUi



};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
