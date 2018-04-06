/* monitor.h */

#ifndef MONITOR_H
#define MONITOR_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <ddcutil_c_api.h>

#include "feature_item_model.h"
#include "feature_table_model.h"
#include "feature_list_widget.h"
#include "misc.h"
#include "nongui/vcprequest.h"
#include "nongui/vcpthread.h"


// Represets a single display

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
    VcpRequestQueue*     _requestQueue;
    DDCA_Display_Info   *_displayInfo;
    DDCA_Display_Handle _dh;
    // VcpThread        _vcpThread;

    QWidget *           _page_listWidget;
    QWidget *           _page_scrollArea;
    int                 _pageno_listWidget;
    int                 _pageno_scrollArea;
    FeatureListWidget * _featureListWidget;


    const char * _cls;    // className

public slots:
    void putVcpRequest(VcpRequest * rqst);

private:
    QHash<DDCA_Feature_Subset_Id, DDCA_Feature_List> _features;
};

#endif // MONITOR_H
