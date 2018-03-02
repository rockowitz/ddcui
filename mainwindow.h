#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ddcutil_types.h>
#include "misc.h"
#include "monitor.h"
#include <QComboBox>
#include <QPlainTextEdit>
#include <QTableView>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

    DDCA_Feature_List_Id feature_list_id() const;
    void set_feature_list_id(DDCA_Feature_List_Id feature_list_id);
    FeatureSelector * feature_selector;

    QVector<Monitor*> monitors;

    QComboBox * _toolbarDisplayCB;

    QStackedWidget *  _views_StackedWidget;
    QPlainTextEdit * _moninfoPlainText = nullptr;
    QTableView *     _vcp_tableview = nullptr;


private:
    DDCA_Feature_List_Id _feature_list_id = DDCA_FEATURE_LIST_KNOWN;
    QVector<VcpThread*> vcp_threads;

private slots:
    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    // void on_actionFeatures_triggered();

    void on_actionCapabilities_triggered();

    void on_actionMonitor_Summary_triggered();

    void on_actionFeature_Selection_triggered();

    void on_actionFeature_Selection_Dialog_triggered();

    void on_actionFeatures_TableView_triggered();

    void on_actionFeaturesListView_triggered();

    void on_vcp_tableView_clicked(const QModelIndex &index);

    void on_vcp_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    void initDisplaySelector();
    void loadMonitorFeatures(Monitor * monitor) ;

    DDCA_Display_Info_List * _dlist ;
    int                      _curDisplayIndex = 0;
    View                     _curView = MonitorView;
};

#endif // MAINWINDOW_H
