/* mainwindow_ui.h - Extracted from uic generated ui code */

/* Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <iostream>

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"


// QT_BEGIN_NAMESPACE

class Ui_MainWindow
{

public:
   Ui_MainWindow(QMainWindow* mainWindow);

private:
   QMainWindow * _mainWindow;

    // Menu Bar
    QMenuBar    *menuBar= NULL;
    QMenu       *menuView= NULL;
    QMenu       *menuOptions= NULL;
    QMenu       *menuHelp= NULL;

public:
    // View Menu
    QAction     *actionMonitorSummary= NULL;
    QAction     *actionCapabilities= NULL;

#ifdef ALT_FEATURES
    QAction     *actionFeaturesListWidget;        // for ListWidget
    QAction     *actionFeaturesListView;
    QAction     *actionFeaturesTableView;
    QAction     *actionFeaturesScrollAreaMock;
#endif
    QAction     *actionFeaturesScrollArea = NULL;
private:
    // Options Menu
    QAction     *actionFeatureSelection= NULL;
    QAction     *actionShowUnsupportedFeatures= NULL;
    QAction     *actionKnown= NULL;
    QAction     *actionScan= NULL;
    QAction     *actionColor= NULL;
    QAction     *actionProfile = NULL;
    QAction     *actionManufacturer = NULL;
    QAction     *actionFeatureSelectionDialog = NULL;
    QAction     *actionOtherOptionsDialog = NULL;

    // Help Menu
    QAction     *actionAbout= NULL;
    QAction     *actionAboutQt= NULL;

public:
    // Tool Bar
    QToolBar    *mainToolBar= NULL;

    // Status Bar
    QStatusBar  *statusBar= NULL;

    // Other
    QStackedWidget *centralWidget = NULL;


private:
    void initActions(QMainWindow * mainWindow);
    void retranslateFeatureSelectionDialog();
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
