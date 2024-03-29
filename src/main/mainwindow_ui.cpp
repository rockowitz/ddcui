/** \file mainwindow_ui.cpp  */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Note:  This file cannot be named ui_mainwindow.cpp.
// The cmake autouic directive regards the "ui_" prefix as special.
// If a cpp file starts with "ui_", cmake looks for a corresponding
// file with extension .ui to process.


#include "mainwindow_ui.h"
#include "../base/ddcui_core.h"

#ifdef DEBUG
static
void dbgrptQKeyEvent(QKeyEvent * event) {
   printf("  key:  %d\n", event->key());
   printf("  modifiers: 0x%x\n", (unsigned int) event->modifiers());
   printf("  nativeModifiers: 0x%08x\n", event->nativeModifiers());
   printf("  nativeScanCode:  0x%08x\n", event->nativeScanCode());
}
#endif


Ui_MainWindow::Ui_MainWindow(QMainWindow* mainWindow)
   : _mainWindow(mainWindow)
{
   bool debug = false;
   TRACECF(debug, "Starting");
   setupUi(mainWindow);
   TRACECF(debug, "Done");
}


void Ui_MainWindow::initActions(QMainWindow * MainWindow)
{
    // View Menu Actions
    // n. QMetaObject::connectSlotsByName() depends on object name,
    // must match action_<objname>_triggered()

    actionMonitorSummary = new QAction(MainWindow);
    actionMonitorSummary->setObjectName(QString::fromUtf8("actionMonitorSummary"));
    actionMonitorSummary->setCheckable(true);

    actionCapabilities = new QAction(MainWindow);
    actionCapabilities->setObjectName(QString::fromUtf8("actionCapabilities"));
    actionCapabilities->setCheckable(true);

    actionFeaturesScrollArea = new QAction(MainWindow);
    actionFeaturesScrollArea->setObjectName(QString::fromUtf8("actionFeaturesScrollArea"));
    actionFeaturesScrollArea->setCheckable(true);

    QActionGroup* viewGroup = new QActionGroup(MainWindow);
    viewGroup->addAction(actionMonitorSummary);
    viewGroup->addAction(actionCapabilities);
    viewGroup->addAction(actionFeaturesScrollArea);

    // actionMonitorSummary->setChecked(true);   // ???


    // Options->Feature Selection Dialog Actions

    // Feature Selection menu action
    actionFeatureSelectionDialog = new QAction(MainWindow);
    actionFeatureSelectionDialog->setObjectName(QString::fromUtf8("actionFeatureSelectionDialog"));
    actionFeatureSelectionDialog->setText(
          QApplication::translate("MainWindow", "&Feature Selection...",        nullptr, -1));
    actionFeatureSelectionDialog->setFont(mainMenuFont);

    // Options menu actions
    actionOtherOptionsDialog = new QAction(MainWindow);
    actionOtherOptionsDialog->setObjectName(QString::fromUtf8("actionOtherOptionsDialog"));
    actionOtherOptionsDialog->setText(
          QApplication::translate("MainWindow", "&NC Feature Values...",            nullptr, -1));
          // was "&Other Options"
    actionOtherOptionsDialog->setFont(mainMenuFont);

    // User Interface menu action
    actionUserInterfaceOptionsDialog = new QAction(MainWindow);
    actionUserInterfaceOptionsDialog->setObjectName(QString::fromUtf8("actionUserInterfaceOptionsDialog"));
    actionUserInterfaceOptionsDialog->setText(
          QApplication::translate("MainWindow", "&User Interface...",   nullptr, -1));
    actionUserInterfaceOptionsDialog->setFont(mainMenuFont);

    actionDebugActionsDialog = new QAction(MainWindow);
    // n.b. objectName used by connectSlotsByName()
    actionDebugActionsDialog->setObjectName(QString::fromUtf8("actionDebugActionsDialog"));
    actionDebugActionsDialog->setText(
          QApplication::translate("MainWindow", "&Execution Statistics...",   nullptr, -1));
    actionDebugActionsDialog->setFont(mainMenuFont);

    // Actions Menu Actions
    actionRescan = new QAction(MainWindow);
    actionRescan->setObjectName(QString::fromUtf8("actionRescan"));
    actionRedetect = new QAction(MainWindow);
    actionRedetect->setObjectName(QString::fromUtf8("actionRedetect"));
    actionDebugLocks = new QAction(MainWindow);
    actionDebugLocks->setObjectName(QString::fromUtf8("actionDebugLocks"));
    actionDebugLocks->setText(
         QApplication::translate("MainWindow", "Debug Locks", nullptr, -1));

    // ??
    // QActionGroup* actionsGroup = new QActionGroup(MainWindow);
    // actionsGroup->addAction(actionRescan);
    // actionsGroup->addAction(actionRescan);


    // Help Menu Actions

    actionContentsHelp = new QAction(MainWindow);
    actionContentsHelp->setObjectName(QString("actionContentsHelp"));
    actionAbout = new QAction(MainWindow);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionAboutQt = new QAction(MainWindow);
    actionAboutQt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
}


void Ui_MainWindow::setupMenus(QMainWindow *MainWindow)
{
    // Menu Bar
    menuBar = new QMenuBar(MainWindow);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setFont(mainMenuFont);
    // menuBar->setGeometry(QRect(0, 0, 800, 30));

    // character shortcuts in menubar don't work, are passed to the
    // display combo box

    menuView = new QMenu(menuBar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuView->setTitle(    QApplication::translate("MainWindow", "View", nullptr, -1));
    menuView->setFont(mainMenuFont);  // sets the font for menu entries

    // menuDisplays = new QMenu(menuBar);
    // menuDisplays->setObjectName(QString::fromUtf8("menuDisplays"));
    // menuDisplays->setTitle(QApplication::translate("MainWindow", "Displays", 0));

    menuActions = new QMenu(menuBar);
    menuActions->setObjectName(QString::fromUtf8("menuActions"));
    menuActions->setTitle(  QApplication::translate("MainWindow", "Actions", nullptr, -1));
    menuActions->setFont(mainMenuFont); // font for Actions menu entries

    menuOptions = new QMenu(menuBar);
    menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
    menuOptions->setTitle( QApplication::translate("MainWindow", "Options", nullptr, -1));
    menuOptions->setFont(mainMenuFont);
    menuOptions->addAction(actionFeatureSelectionDialog);
    menuOptions->addAction(actionOtherOptionsDialog);
    menuOptions->addAction(actionUserInterfaceOptionsDialog);

    menuHelp = new QMenu(menuBar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuHelp->setTitle(    QApplication::translate("MainWindow", "Help", nullptr, -1));
    menuHelp->setFont(mainMenuFont);

    MainWindow->setMenuBar(menuBar);

    // Menu Bar actions
    menuBar->addAction(menuView->menuAction());
    // menuBar->addAction(menuDisplays->menuAction());
    menuBar->addAction(menuActions->menuAction());
    menuBar->addAction(menuOptions->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    menuView->addAction(actionMonitorSummary);
    menuView->addAction(actionCapabilities);
    menuView->addAction(actionFeaturesScrollArea);

    actionMonitorSummary->setText(    QApplication::translate("MainWindow", "&Monitor Summary",       nullptr, -1));
    actionCapabilities->setText(      QApplication::translate("MainWindow", "&Capabilities",          nullptr, -1));
    actionFeaturesScrollArea->setText(QApplication::translate("MainWindow", "&Features",              nullptr, -1));
    actionRescan->setText(            QApplication::translate("MainWindow", "Rescan Feature &Values", nullptr, -1));
    actionMonitorSummary->setEnabled(false);
    actionCapabilities->setEnabled(false);
    actionFeaturesScrollArea->setEnabled(false);
    actionRescan->setFont(mainMenuFont);
    actionRescan->setEnabled(false);
    actionDebugLocks->setEnabled(true);

    actionRedetect->setText(  QApplication::translate("MainWindow", "Re&detect Displays", nullptr, -1));
    actionRedetect->setEnabled(true);
    actionRedetect->setFont(mainMenuFont);

    menuActions->addAction(actionRedetect);
    menuActions->addAction(actionRescan);
    menuActions->addAction(actionDebugActionsDialog);
    menuActions->addAction(actionDebugLocks);

    menuHelp->addAction(actionContentsHelp);
    menuHelp->addAction(actionAbout);
    menuHelp->addAction(actionAboutQt);

    actionContentsHelp->setText(QApplication::translate(  "MainWindow", "&Contents...", nullptr, -1));
    actionContentsHelp->setFont(mainMenuFont);
    actionAbout->setText(QApplication::translate(  "MainWindow", "About ddcui", nullptr, -1));
    actionAbout->setFont(mainMenuFont);
    actionAboutQt->setText(QApplication::translate("MainWindow", "About Qt", nullptr, -1));
    actionAboutQt->setFont(mainMenuFont);

    // Tool Bar
    mainToolBar = new QToolBar(MainWindow);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

    // Status Bar
    statusBar = new QStatusBar(MainWindow);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    statusBar->setFont(mainMenuFont);
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
   bool debug = false;
   TRACECF(debug, "Starting");

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

   // mainMenuFont = Styles::instance().mainMenuFont();
   mainMenuFont = MainMenuFont2;

   initActions(MainWindow);

   layoutCentralWidget(MainWindow);

   setupMenus(MainWindow);

   MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ddcui - Control Display Settings", nullptr, -1));
#ifdef UNUSED
      retranslateFeatureSelectionDialog();
#endif

   centralWidget->setCurrentIndex(0);

   // printf("(Ui_MainWindow::setupUi) Calling connectSlotsByName()...\n"); fflush(stdout);
   QMetaObject::connectSlotsByName(MainWindow);

   // std::cout << "(setupUi) Done" << std::endl;
   TRACECF(debug, "Done");
} // setupUi

#ifdef FAILS
void Ui_MainWindow::keyPressEvent(QKeyEvent * evt) {
   void dbgrptQKeyEvent(evt)
   switch (evt->key())
   {
   case Qt::Key_H:
      menuHelp->menuAction();  // no event
      // menuHelp->trigger();     // doesn't compile
      break;
   }
   evt->accept();
}
#endif


