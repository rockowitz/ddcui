/* monitor.h */

#ifndef MONITOR_H
#define MONITOR_H

#include <QtCore/QObject>
#include <QtCore/QHash>

#include <ddcutil_types.h>
#include <ddcutil_c_api.h>

#include "base/feature_selector.h"

class QListView;
class QListWidget;
class QPlainTextEdit;
class QTableView;
class QTableWidget;
class QWidget;
// class QHash;  // incomplete type

class FeatureBaseModel;
class FeatureItemModel;
class FeatureListWidget;
class FeaturesScrollAreaView;
class FeaturesScrollAreaView;
class FeatureTableModel;
class VcpRequest;
class VcpRequestQueue;


// Represents a single display

class Monitor : public QObject
{
    Q_OBJECT

public:

    enum FeaturesView {
       FEATURES_VIEW_SCROLLAREA_VIEW,
       FEATURES_VIEW_SCROLLAREA_MOCK,
       FEATURES_VIEW_TABLEVIEW,
       FEATURES_VIEW_TABLEWIDGET,
       FEATURES_VIEW_LISTVIEW,
       FEATURES_VIEW_LISTWIDGET,
       FEATURES_VIEW_UNSET
    };

    Monitor(DDCA_Display_Info * display_info, int monitorNumber);
    ~Monitor();

    DDCA_Feature_List getFeatureList(DDCA_Feature_Subset_Id);
    void setFeatureItemModel(FeatureItemModel * listModel);      // ALT
    void setFeatureTableModel(FeatureTableModel * tableModel);

    const int _monitorNumber = -1;    // 1 based

    FeatureItemModel *   _listModel;
    FeatureTableModel *  _tableModel;
    FeatureBaseModel *   _baseModel;

    VcpRequestQueue*     _requestQueue;
    DDCA_Display_Info *  _displayInfo;
    DDCA_Display_Handle  _dh;
    // DDCA_Display_Ref  _dref;    // use _displayInfo->dref
    // VcpThread         _vcpThread;

    QWidget *           _page_moninfo;
    int                 _pageno_moninfo;
    QPlainTextEdit *    _moninfoPlainText;

    QWidget *           _page_capabilities;
    int                 _pageno_capabilities;
    QPlainTextEdit *    _capabilitiesPlainText;

    FeaturesView       _curFeaturesView = FEATURES_VIEW_UNSET;
    FeatureSelector    _curFeatureSelector;

// *** Begin Alternative Central Widgets ***

#ifdef UNUSED
    QWidget *           page_vcp;
    QListWidget *       vcpListWidget;
    int                 _pageno_vcp;
#endif

    // FEATURES_VIEW_LISTWIDGET, initListWidget()
    QWidget *           _page_listWidget   = NULL;
    FeatureListWidget * _featureListWidget = NULL;
    int                 _pageno_listWidget = -1;

#ifdef UNUSED
    QWidget *page_list_widget;
    // QListWidget *feature_listWidget;
    FeatureListWidget * feature_listWidget;
    int _pageno_list_widget;
#endif

    // FEATURES_VIEW_LISTVIEW
    QWidget *           page_list_view    = NULL;
    QListView *         vcp_listView      = NULL;
    int                 _pageno_list_view = -1;

    // FEATURES_VIEW_TABLEWIDGET
    // init function defined, but no action
    QWidget *           page_table_item     = NULL;
    QTableWidget *      tableWidget         = NULL;
    int                 _pageno_table_item  = -1;

    // FEATURES_VIEW_TABLEVIEW
    QWidget *           _page_table_view   = NULL;
    QTableView *        _vcp_tableView     = NULL;
    int                 _pageno_table_view = -1;

    // *** End Alternative Central Widgets ***

    // QScrollArea *                 _page_features_scrollarea;
    // FeaturesScrollAreaContents *  _featuresScrollAreaContents;
    // int                           _pageno_features_scrollarea;

    // FEATURES_VIEW_SCROLLAREAVIEW
    // When using FeaturesScrollAreaView, do not allocate a permanent
    // QScrollArea and contents.  These must be created dynamically
    // each time features are loaded.
    FeaturesScrollAreaView * _featuresScrollAreaView = NULL;

    void dbgrpt();

public slots:
    void putVcpRequest(VcpRequest * rqst);

private:
    const char *  _cls;    // className
    QHash<DDCA_Feature_Subset_Id, DDCA_Feature_List> _features;
};

#endif // MONITOR_H
