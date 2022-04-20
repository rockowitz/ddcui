/* mainwindow.h */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QKeyEvent>
#include <QtWidgets/QtWidgets>

#include <ddcutil_types.h>

#include "cmdline/ddcui_parsed_cmd.h"
#include "base/core.h"
#include "base/user_interface_options_state.h"
#include "base/other_options_state.h"
#include "nongui/msgbox_queue.h"

class DebugActionsDialog;
class FeatureBaseModel;
class FeatureSelectionDialog;
class FeatureSelector;
class Monitor;
class MsgBoxQueue;
class MsgBoxThread;
class OtherOptionsDialog;
class OtherOptionsState;
class QMessageBox;
class UserInterfaceOptionsDialog;
class UserInterfaceOptionsState;
class VcpThread;

// namespace Ui {
// class MainWindow;
// }

class Ui_MainWindow;

class MainWindow : public QMainWindow
// , public PageChangeObserver
{
    Q_OBJECT

    // n. MainWindow is also a singleton, combine with GlobalState?
    // to do, make private, use accessor function
    // GlobalState & _globalState;


public:
    enum View {
        NoView,
        MonitorView,
        CapabilitiesView,
        FeaturesView
    };

//
// Constructor, Destructor, Initialization methods
//

public:
    // explicit
    MainWindow(Parsed_Ddcui_Cmd * parsed_cmd, QWidget *parent = 0);
    ~MainWindow();

    // *** public initialization methods
    void initSerialMsgbox();
    void start_msgBoxThread();    // was originally factored out for use as a slot

private:
    void connectBaseModel(Monitor * monitor);
    void disconnectBaseModel(Monitor * monitor);
    void freeMonitors();
    void initOneMonitor(DDCA_Display_Info * info, int curIndex);
    void setInitialDisplayIndex(Parsed_Ddcui_Cmd * parsed_cmd);
    void initMonitors(Parsed_Ddcui_Cmd * parsed_cmd);
    void loadMonitorFeatures(Monitor * monitor);

//
// *** Signals
//

public:
    signals:
    void featureSelectionChanged();
    void signalMonitorSummaryView();
    void signalCapabilitiesView();
    void signalFeaturesView();
    void userInterfaceOptionsChanged();
    void showStats();     // needs parm for which
    void resetStats();
    void reportApplicationEventLoopStarted();
    void signalControlKeyPressed(bool onoff);


//
// *** Slots ***
//

public slots:
    void longRunningTaskStart();
    void longRunningTaskEnd();
    void setStatusMsg(QString msg);
    void showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon);

    // or should these two be private?
    void showCentralWidgetPage(int pageno);
    void showCentralWidgetByWidget(QWidget * pageWidget);

    void forControlKeyRequired_changed(bool onoff);

protected slots:
    void keyPressEvent(QKeyEvent *   ev) override;
    void keyReleaseEvent(QKeyEvent * ev) override;

private slots:
    // View Menu
    void on_actionMonitorSummary_triggered();
    void on_actionCapabilities_triggered();

    void on_actionFeaturesScrollArea_triggered();

    // Actions Menu
    void on_actionRescan_triggered();
    void on_actionRedetect_triggered();
    void on_actionDebugActionsDialog_triggered();
    void for_resetStats_triggered();
    void for_reportStats_triggered(DDCA_Stats_Type stats_type, bool showTheadData);

    // Options Menu
    void on_actionFeatureSelectionDialog_triggered();
    void for_actionFeatureSelectionDialog_accepted();
    void on_actionOtherOptionsDialog_triggered();
    void on_actionUserInterfaceOptionsDialog_triggered();
    void for_actionUserInterfaceOptionsDialog_accept();

#ifdef UNUSED
    void actionFeatureSelectionDialog_destroyed(QObject * obj);
    void on_actionFeatureSelection_triggered();
    void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);
    void on_actionOtherOptionsDialog_accepted();
#endif

    // Help Menu
    void on_actionContentsHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

    // Tool Bar
    void displaySelectorCombobox_currentIndexChanged(int index);
    // void displaySelectorCombobox_activated(int index);

    // Shortcuts
    void quitShortcut();



//
// *** General methods ***
//

public:
    void reportDdcApiError(QString funcname, int rc) const;

private:
    // used only by slotfor_reportStats_triggered()
      void capture_stats(DDCA_Stats_Type stats_type, bool show_thread_data);

// *** Unused Methods, public and private, all types
 #ifdef UNUSED
      void on_vcpTableView_clicked(const QModelIndex &index);
      void on_vcpTableView_doubleClicked(const QModelIndex &index);
      void pageChanged(int pageno) override;
      void pageChangedByWidget(QWidget * widget) override;
     DDCA_Feature_Subset_Id feature_list_id() const;
     void set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id);
 #endif


//
// *** Member Variables ***
//

public:
    UserInterfaceOptionsDialog * _uid = nullptr;
    FeatureSelector *            _feature_selector = nullptr;
    MsgBoxQueue*                 _msgBoxQueue = nullptr;
    MsgBoxThread *               _msgBoxThread = nullptr;
    QMessageBox*                 _serialMsgBox = nullptr;
    bool                         _ctrl_key_is_pressed = false;

private:
    const char *               _cls;
    // Ui::MainWindow*         _ui;
    Ui_MainWindow *            _ui;
    // DDCA_Display_Info_List *   _dlist ;
    DDCA_Display_Ref *        _drefs;
    int                        _drefs_ct = 0;
    int                        _curDisplayIndex = -1;
    View                       _curView = NoView;
    QComboBox *                _toolbarDisplayCB;
    OtherOptionsState *        _otherOptionsState = nullptr;
    UserInterfaceOptionsState* _uiOptionsState = nullptr;
    QVector<Monitor*>          _monitors;
    DDCA_Feature_Subset_Id     _feature_list_id = DDCA_SUBSET_KNOWN;
    QVector<VcpThread*>        _vcp_threads;

    QMessageBox*               _loadingMsgBox;

    // Accumulates messages that will be shown in the SerialMsgBox once
    // initialization is sufficiently complete.  Not needed.
    // QList<MsgBoxQueueEntry*>   _deferredMsgs;

    FeatureSelectionDialog*    _fsd = nullptr;
    OtherOptionsDialog*        _ood = nullptr;       // for future use
    bool                       _widgetChangesAllowed = true;

    QShortcut * _quit_shortcut = nullptr;
};

#endif // MAINWINDOW_H
