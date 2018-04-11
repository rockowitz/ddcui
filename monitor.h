/* monitor.h */

#ifndef MONITOR_H
#define MONITOR_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <ddcutil_c_api.h>

#include "nongui/vcprequest.h"
#include "nongui/vcpthread.h"
#include "nongui/feature_base_model.h"

#include "table_model_view/feature_table_model.h"

#include "feature_scrollarea/features_scroll_area_contents.h"
#include "feature_selection/feature_selector.h"
#include "list_model_view/feature_item_model.h"
#include "list_widget/feature_list_widget.h"

class FeaturesScrollAreaView;

// Represents a single display

class Monitor : public QObject
{
    Q_OBJECT   // causes undefined reference to vtable

public:
    Monitor(DDCA_Display_Info * display_info);
    ~Monitor();

    DDCA_Feature_List getFeatureList(DDCA_Feature_Subset_Id);
    void setFeatureItemModel(FeatureItemModel * listModel);      // ALT
    void setFeatureTableModel(FeatureTableModel * tableModel);

    FeatureItemModel    *_listModel;
    FeatureTableModel   *_tableModel;
    FeatureBaseModel *   _baseModel;

    VcpRequestQueue*     _requestQueue;
    DDCA_Display_Info   *_displayInfo;
    DDCA_Display_Handle _dh;
    // VcpThread        _vcpThread;

    QWidget *           _page_listWidget;
    // QWidget *           _page_scrollArea;
    int                 _pageno_listWidget;
    // int                 _pageno_scrollArea;
    FeatureListWidget * _featureListWidget;

    // FeaturesScrollArea is per-monitor
    QScrollArea *                 _page_features_scrollarea;
    FeaturesScrollAreaContents *  _featuresScrollAreaContents;
    int                           _pageno_features_scrollarea;

    FeaturesScrollAreaView *     _featuresScrollAreaView;

    const char * _cls;    // className

public slots:
    void putVcpRequest(VcpRequest * rqst);

private:
    QHash<DDCA_Feature_Subset_Id, DDCA_Feature_List> _features;
};

#endif // MONITOR_H
