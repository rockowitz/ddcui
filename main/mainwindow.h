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

// #include "base/monitor.h"
#include "feature_scrollarea/page_change_observer.h"
// #include "feature_selection/feature_selector.h"

class VcpThread;
class FeatureSelector;
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
        MonitorView,
        CapabilitiesView,
        FeaturesView
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void reportDdcApiError(QString funcname, int rc) const;

    DDCA_Feature_Subset_Id feature_list_id() const;
    void set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id);
    FeatureSelector * feature_selector;

    QVector<Monitor*> monitors;

    QComboBox * _toolbarDisplayCB;

    QStackedWidget * _views_StackedWidget;
#ifdef OLD
    QPlainTextEdit * _moninfoPlainText = nullptr;
    QTableView *     _vcp_tableview = nullptr;
#endif
    void pageChanged(int pageno) override;
    void pageChangedByWidget(QWidget * widget) override;

    const char * _cls;

signals:
    void featureSelectionChanged();

public slots:
    void longRunningTaskStart();
    void longRunningTaskEnd();

    void setStatusMsg(QString msg);

private:
    DDCA_Feature_Subset_Id _feature_list_id = DDCA_SUBSET_KNOWN;
    QVector<VcpThread*> vcp_threads;

    WaitingSpinnerWidget* _spinner;
    QMessageBox*          _loadingMsgBox;


    QWidget * initFeaturesScrollArea(
          Monitor *         curMonitor,
          FeatureBaseModel* baseModel,
          QStackedWidget *  stackedWidget);

    void featureSelectionDone();


private slots:
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

    // void on_actionFeatures_triggered();

    void on_actionMonitorSummary_triggered();
    void on_actionCapabilities_triggered();

    void on_actionFeatureSelectionDialog_triggered();
    void on_actionFeatureSelectionDialog_accepted();
    void actionFeatureSelectionDialog_destroyed(QObject * obj);

    void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);

    void on_actionFeatureSelection_triggered();

    void on_actionFeaturesTableView_triggered();
    void on_actionFeaturesListView_triggered();
    void on_actionFeaturesListWidget_triggered();
    // void on_actionFeaturesScrollArea_triggered_old();
    void on_actionFeaturesScrollAreaMock_triggered();
    void on_actionFeaturesScrollArea_triggered();

    void on_vcpTableView_clicked(const QModelIndex &index);
    void on_vcpTableView_doubleClicked(const QModelIndex &index);

    void showCentralWidgetPage(int pageno);
    void showCentralWidgetByWidget(QWidget * pageWidget);



private:
    Ui::MainWindow *ui;

    void initDisplaySelector();
    void loadMonitorFeatures(Monitor * monitor) ;

    DDCA_Display_Info_List * _dlist ;
    int                      _curDisplayIndex = 0;
    View                     _curView = MonitorView;
};

#endif // MAINWINDOW_H
