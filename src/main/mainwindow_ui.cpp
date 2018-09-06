/* mainwindow_ui.cpp
 *
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

// Note:  This file cannot be named ui_mainwindow.cpp.
// The cmake autouic directive tries regards the "ui_" prefix as special.
// If a cpp file starts with "ui_", cmake looks for a corresponding
// file with extension .ui to process.


#include "mainwindow_ui.h"

   Ui_MainWindow::Ui_MainWindow(QMainWindow* mainWindow)
      : _mainWindow(mainWindow)
   {
      setupUi(mainWindow);
   }


   void Ui_MainWindow::initActions(QMainWindow * MainWindow){

       // View Menu Actions
       // n. QMetaObject::connectSlotsByName() depends on object name,
       // must match action_<objname>_triggered()

       actionMonitorSummary = new QAction(MainWindow);
       actionMonitorSummary->setObjectName(QString::fromUtf8("actionMonitorSummary"));
       actionMonitorSummary->setCheckable(true);

       actionCapabilities = new QAction(MainWindow);
       actionCapabilities->setObjectName(QString::fromUtf8("actionCapabilities"));
       actionCapabilities->setCheckable(true);

#ifdef ALT_FEATURES
       if (enableAltFeatures) {
          actionFeaturesListWidget = new QAction(MainWindow);
          actionFeaturesListWidget->setObjectName(QString::fromUtf8("actionFeaturesListWidget"));

          actionFeaturesListView = new QAction(MainWindow);
          actionFeaturesListView->setObjectName(QString::fromUtf8("actionFeaturesListView"));

          actionFeaturesTableView = new QAction(MainWindow);
          actionFeaturesTableView->setObjectName(QString::fromUtf8("actionFeaturesTableView"));

          actionFeaturesScrollAreaMock = new QAction(MainWindow);
          actionFeaturesScrollAreaMock->setObjectName(QString::fromUtf8("actionFeaturesScrollAreaMock"));
       }
#endif

       actionFeaturesScrollArea = new QAction(MainWindow);
       actionFeaturesScrollArea->setObjectName(QString::fromUtf8("actionFeaturesScrollArea"));
       actionFeaturesScrollArea->setCheckable(true);

       actionFeatureSelection = new QAction(MainWindow);
       actionFeatureSelection->setObjectName(QString::fromUtf8("actionFeatureSelection"));

       QActionGroup* viewGroup = new QActionGroup(MainWindow);
       viewGroup->addAction(actionMonitorSummary);
       viewGroup->addAction(actionCapabilities);
       viewGroup->addAction(actionFeaturesScrollArea);
#ifdef ALT_FEATURES
       if (enableAltFeatures) {
          // TODO
       }
#endif
       // actionMonitorSummary->setChecked(true);   // ???


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

       // Options Menu Actions
       actionFeatureSelectionDialog = new QAction(MainWindow);
       actionFeatureSelectionDialog->setObjectName(QString::fromUtf8("actionFeatureSelectionDialog"));

       actionOtherOptionsDialog = new QAction(MainWindow);
       actionOtherOptionsDialog->setObjectName(QString::fromUtf8("actionOtherOptionsDialog"));

       // Help Menu Actions

       actionAbout = new QAction(MainWindow);
       actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
       actionAboutQt = new QAction(MainWindow);
       actionAboutQt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
   }


   void Ui_MainWindow::retranslateFeatureSelectionDialog()
   {
      actionFeatureSelection->setText(
            QApplication::translate("MainWindow", "Feature Selection",         nullptr, -1));
      actionShowUnsupportedFeatures->setText(
            QApplication::translate("MainWindow", "Show Unsupported Features", nullptr, -1));
      actionKnown->setText(
            QApplication::translate("MainWindow", "Known",                     nullptr, -1));
      actionScan->setText(
            QApplication::translate("MainWindow", "Scan",                      nullptr, -1));
      actionColor->setText(
            QApplication::translate("MainWindow", "Color",                     nullptr, -1));
      actionProfile->setText(
            QApplication::translate("MainWindow", "Profile",                   nullptr, -1));
      actionManufacturer->setText(
            QApplication::translate("MainWindow", "Manufacturer",              nullptr, -1));
      actionFeatureSelectionDialog->setText(
            QApplication::translate("MainWindow", "&Feature Selection",        nullptr, -1));
      actionOtherOptionsDialog->setText(
            QApplication::translate("MainWindow", "&Other Options",            nullptr, -1));
   }

   void Ui_MainWindow::setupMenus(QMainWindow *MainWindow)
    {
       // Menu Bar
       menuBar = new QMenuBar(MainWindow);
       menuBar->setObjectName(QString::fromUtf8("menuBar"));
       // menuBar->setGeometry(QRect(0, 0, 800, 30));

       menuView = new QMenu(menuBar);
       menuView->setObjectName(QString::fromUtf8("menuView"));
       menuView->setTitle(    QApplication::translate("MainWindow", "&View", nullptr, -1));

       // menuDisplays = new QMenu(menuBar);
       // menuDisplays->setObjectName(QString::fromUtf8("menuDisplays"));
       // menuDisplays->setTitle(QApplication::translate("MainWindow", "Disp&lays", 0));

       menuOptions = new QMenu(menuBar);
       menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
       menuOptions->setTitle( QApplication::translate("MainWindow", "Optio&ns", nullptr, -1));
       menuOptions->addAction(actionFeatureSelectionDialog);
       menuOptions->addAction(actionOtherOptionsDialog);

       menuHelp = new QMenu(menuBar);
       menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
       menuHelp->setTitle(    QApplication::translate("MainWindow", "Help", nullptr, -1));

       MainWindow->setMenuBar(menuBar);

       // Menu Bar actions
       menuBar->addAction(menuView->menuAction());
       // menuBar->addAction(menuDisplays->menuAction());
       menuBar->addAction(menuOptions->menuAction());
       menuBar->addAction(menuHelp->menuAction());

       menuView->addAction(actionMonitorSummary);
       menuView->addAction(actionCapabilities);
#ifdef ALT_FEATURES
       if (enableAltFeatures) {
           menuView->addAction(actionFeaturesListWidget);
           menuView->addAction(actionFeaturesListView);
           menuView->addAction(actionFeaturesTableView);
           menuView->addAction(actionFeaturesScrollAreaMock);
       }
#endif
       menuView->addAction(actionFeaturesScrollArea);

       actionMonitorSummary->setText(    QApplication::translate("MainWindow", "&Monitor Summary", nullptr, -1));
       actionCapabilities->setText(      QApplication::translate("MainWindow", "&Capabilities", nullptr, -1));
#ifdef ALT_FEATURES
       if (enableAltFeatures) {
          actionFeaturesListWidget->setText(QApplication::translate("MainWindow", "&Features - ListWidget", 0));
          actionFeaturesListView->setText(  QApplication::translate("MainWindow", "&Features - ListView",   0));
          actionFeaturesTableView->setText( QApplication::translate("MainWindow", "F&eatures - TableView",  0));

          actionFeaturesScrollAreaMock->setText(
                                            QApplication::translate("MainWindow", "Features - &MockScrollArea", 0));
          actionFeaturesScrollArea->setText(QApplication::translate("MainWindow", "Features - &ScrollArea", 0));
       }
       else {
#endif
          actionFeaturesScrollArea->setText(QApplication::translate("MainWindow", "&Features", nullptr, -1));
#ifdef ALT_FEATURES
       }
#endif
       menuHelp->addAction(actionAbout);
       menuHelp->addAction(actionAboutQt);

       actionAbout->setText(QApplication::translate(  "MainWindow", "&About ddcui", nullptr, -1));
       actionAboutQt->setText(QApplication::translate("MainWindow", "About &Qt", nullptr, -1));


       // Tool Bar
       mainToolBar = new QToolBar(MainWindow);
       mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
       MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

       // Status Bar
       statusBar = new QStatusBar(MainWindow);
       statusBar->setObjectName(QString::fromUtf8("statusBar"));
       MainWindow->setStatusBar(statusBar);
    }

   void Ui_MainWindow::layoutCentralWidget(QMainWindow *MainWindow)
      {
         centralWidget = new QStackedWidget(MainWindow);
         centralWidget->setObjectName(QString::fromUtf8("centralWidget/views_stackedWidget"));

         QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
         sizePolicy1.setHorizontalStretch(1);
         sizePolicy1.setVerticalStretch(1);
         sizePolicy1.setHeightForWidth(false);
         centralWidget->setSizePolicy(sizePolicy1);

         centralWidget->setMinimumSize(QSize(740, 400));            // affected by contents width
         // centralWidget->setMaximumSize(QSize(2000, 16777215));
         centralWidget->setSizeIncrement(QSize(10, 10));

         centralWidget->setFrameShape(QFrame::Panel);
         centralWidget->setFrameShadow(QFrame::Sunken);

         MainWindow->setCentralWidget(centralWidget);
         // reportWidgetChildren(centralWidget, "Children of centralWidget:");
         // printf("(%s) Number of widgets contained by centralWidget: %d\n",
         //        __func__, centralWidget->count() );  fflush(stdout);
      }

   void Ui_MainWindow::setupUi(QMainWindow *MainWindow)
     {
         // std::cout << "(setupUi) Starting" << std::endl;
         if (MainWindow->objectName().isEmpty())
             MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
         // MainWindow->resize(800, 400);

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

         MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ddcui - Control Monitor Settings", nullptr, -1));
         retranslateFeatureSelectionDialog();

         centralWidget->setCurrentIndex(0);

         printf("(Ui_MainWindow::setupUi) Calling connectSlotsByName()...\n"); fflush(stdout);
         QMetaObject::connectSlotsByName(MainWindow);

         // std::cout << "(setupUi) Done" << std::endl;
     } // setupUi

