/* mainwindow_ui.h - Extracted from uic generated ui code */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <iostream>

#include <QtGui/QFont>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include "../base/core.h"
#include "../base/widget_debug.h"


// QT_BEGIN_NAMESPACE

class Ui_MainWindow
{

public:
   Ui_MainWindow(QMainWindow* mainWindow);

private:
   QMainWindow * _mainWindow;

    // Menu Bar
    QMenuBar    *menuBar     = NULL;
    QMenu       *menuView    = NULL;
    QMenu       *menuOptions = NULL;
    QMenu       *menuActions = NULL;
    QMenu       *menuHelp    = NULL;

public:
    // View Menu
    QAction     *actionMonitorSummary= NULL;
    QAction     *actionCapabilities= NULL;

    // Actions Menu
    QAction     *actionRescan = NULL;
    QAction     *actionDebugActionsDialog = NULL;

    QAction     *actionFeaturesScrollArea = NULL;
    QAction     *actionUserInterfaceOptionsDialog = NULL;
private:
    // Options Menu
#ifdef IS_THIS_NEEDED
    QAction     *actionFeatureSelection= NULL;

    QAction     *actionShowUnsupportedFeatures= NULL;
    QAction     *actionKnown= NULL;
    QAction     *actionScan= NULL;
    QAction     *actionColor= NULL;
    QAction     *actionProfile = NULL;
    QAction     *actionManufacturer = NULL;
#endif
    QAction     *actionFeatureSelectionDialog = NULL;
    QAction     *actionOtherOptionsDialog = NULL;


    // Actions Menu

    QAction     *actionRedetect = NULL;

    // Help Menu
    QAction     *actionContentsHelp;
    QAction     *actionAbout= NULL;
    QAction     *actionAboutQt= NULL;

public:
    // Tool Bar
    QToolBar    *mainToolBar= NULL;

    // Status Bar
    QStatusBar  *statusBar= NULL;

    // Other
    QStackedWidget *centralWidget = NULL;
    QFont mainMenuFont;


private:
    void initActions(QMainWindow * mainWindow);
#ifdef UNUSED
    void retranslateFeatureSelectionDialog();
#endif
    void setupMenus(QMainWindow *MainWindow);
    void layoutCentralWidget(QMainWindow *MainWindow);



// public:
    void setupUi(QMainWindow *MainWindow);

};

// namespace Ui {
//     class MainWindow: public Ui_MainWindow {};
// } // namespace Ui

// QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
