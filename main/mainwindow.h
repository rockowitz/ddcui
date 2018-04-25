/* mainwindow.h */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QtWidgets>

#include <ddcutil_types.h>

#include "base/ddcui_globals.h"
#include "base/other_options_state.h"

#include "feature_scrollarea/page_change_observer.h"

class VcpThread;
class FeatureSelector;
class OtherOptionsState;
class Monitor;
class FeatureBaseModel;
class WaitingSpinnerWidget;
class QMessageBox;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow, public PageChangeObserver
{
    Q_OBJECT

public:
    enum View {
        NoView,
        MonitorView,
        CapabilitiesView,
        FeaturesView
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void reportDdcApiError(QString funcname, int rc) const;
    void pageChanged(int pageno) override;
    void pageChangedByWidget(QWidget * widget) override;

#ifdef UNUSED
    DDCA_Feature_Subset_Id feature_list_id() const;
    void set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id);
#endif

signals:
    void featureSelectionChanged();
    void signalMonitorSummaryView();
    void signalCapabilitiesView();
    void signalFeaturesView();

public slots:
    void longRunningTaskStart();
    void longRunningTaskEnd();
    void setStatusMsg(QString msg);

private slots:
    // View Menu
    void on_actionMonitorSummary_triggered();
    void on_actionCapabilities_triggered();
    void on_actionFeaturesTableView_triggered();
    void on_actionFeaturesListView_triggered();
    void on_actionFeaturesListWidget_triggered();
    void on_actionFeaturesScrollAreaMock_triggered();
    void on_actionFeaturesScrollArea_triggered();

    // Options Menu
    void on_actionFeatureSelectionDialog_accepted();
    void on_actionFeatureSelectionDialog_triggered();
#ifdef UNUSED
    void actionFeatureSelectionDialog_destroyed(QObject * obj);
    void on_actionFeatureSelection_triggered();
    void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);
#endif

    void on_actionOtherOptionsDialog_triggered();
    void on_actionOtherOptionsDialog_ncValuesSourceChanged(NcValuesSource valuesSource);
#ifdef UNUSED
    void on_actionOtherOptionsDialog_accepted();
#endif

    // Help Menu
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

    // Tool Bar
    void displaySelectorCombobox_currentIndexChanged(int index);
    // void displaySelectorCombobox_activated(int index);

   // Other
#ifdef UNUSED
    void on_vcpTableView_clicked(const QModelIndex &index);
    void on_vcpTableView_doubleClicked(const QModelIndex &index);
#endif

    void showCentralWidgetPage(int pageno);
    void showCentralWidgetByWidget(QWidget * pageWidget);

private:
    void initMonitors();
    void loadMonitorFeatures(Monitor * monitor);
    void featureSelectionDone();

    const char *             _cls;
    Ui::MainWindow*          _ui;
    DDCA_Display_Info_List * _dlist ;
    int                      _curDisplayIndex = -1;
    View                     _curView = NoView;
    QComboBox *              _toolbarDisplayCB;
    FeatureSelector *        _feature_selector = NULL;
    OtherOptionsState *      _otherOptionsState = NULL;
    QVector<Monitor*>        _monitors;
    DDCA_Feature_Subset_Id   _feature_list_id = DDCA_SUBSET_KNOWN;
    QVector<VcpThread*>      _vcp_threads;
    WaitingSpinnerWidget*    _spinner;
    QMessageBox*             _loadingMsgBox;
};

#endif // MAINWINDOW_H
