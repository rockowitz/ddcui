/* monitor.h */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </endcopyright>
 */

#ifndef MONITOR_H
#define MONITOR_H

#include <QtCore/QObject>
#include <QtCore/QHash>

#include <ddcutil_c_api.h>

#include "config.h"
#include "base/feature_selector.h"

class QListView;
class QListWidget;
class QPlainTextEdit;
class QTableView;
class QTableWidget;
class QWidget;
// class QHash;  // incomplete type, need full #include <QtCore/QHash>

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
    Monitor(DDCA_Display_Info * display_info, int monitorNumber);
    ~Monitor();

    DDCA_Feature_List getFeatureList(DDCA_Feature_Subset_Id);
    bool supportsDdc();
    void dbgrpt();

    const int            _monitorNumber = -1;    // 1 based
    DDCA_Display_Info *  _displayInfo;
    FeatureBaseModel *   _baseModel;
    VcpRequestQueue*     _requestQueue;

    QWidget *           _page_moninfo;
    int                 _pageno_moninfo;
    QPlainTextEdit *    _moninfoPlainText;

    QWidget *           _page_capabilities;
    int                 _pageno_capabilities;
    QPlainTextEdit *    _capabilitiesPlainText;

    FeatureSelector     _curFeatureSelector;

    // FEATURES_VIEW_SCROLLAREAVIEW
    // When using FeaturesScrollAreaView, do not allocate a permanent
    // QScrollArea and contents.  These must be created dynamically
    // each time features are loaded.
    FeaturesScrollAreaView * _featuresScrollAreaView = NULL;

public slots:
        void putVcpRequest(VcpRequest * rqst);

private:
    const char *  _cls;    // className
    QHash<DDCA_Feature_Subset_Id, DDCA_Feature_List> _features;

    // try removing from ifdef
public:
    FeatureItemModel *   _listModel;   // for central widget alternative
    FeatureTableModel *  _tableModel;
    void setFeatureItemModel(FeatureItemModel * listModel);      // ALT
    void setFeatureTableModel(FeatureTableModel * tableModel);

};

#endif // MONITOR_H
