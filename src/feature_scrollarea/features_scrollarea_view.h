/** \file features_scrollarea_view.h
 *
 * Manages the scroll area that provides a view onto the current feature settings
 * of a monitor.
 *
 * There is one instance of this class for each detected monitor.
 */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURES_SCROLLAREA_VIEW_H
#define FEATURES_SCROLLAREA_VIEW_H

#include <QtCore/QObject>

#include "nongui/ddc_error.h"
#include "nongui/feature_base_model.h"

#include "feature_scrollarea/feature_widget.h"

class QStackedWidget;
class Monitor;
class FeaturesScrollAreaContents;
class MsgBoxQueue;


class FeaturesScrollAreaView : public QObject
{
    Q_OBJECT

public:
    FeaturesScrollAreaView(
            Monitor *          monitor,    // needed?  complicates dependency order?
            FeatureBaseModel * model,
            QStackedWidget *   centralStackedWidget,
            MsgBoxQueue *      msgboxQueue,
            QObject *          parent = nullptr);

    void freeContents(void);    // unimplemented

    QSize maxRowSize();

// signals:
//     void signalVcpRequest(VcpRequest * rqst);  // used to call into monitor

public slots:
    // void startInitialLoad(void);
    void onEndInitialLoad(void);
    void onUIValueChanged(uint8_t feature_code, bool writeOnly, uint8_t sh, uint8_t sl);
    void onModelValueChanged(const char * caller, uint8_t featureCode, uint8_t sh, uint8_t sl);
#ifdef PASS_NC_PARMS
    void onNcValuesSourceChanged(NcValuesSource newsrc, bool useLatestNcValueNames);
#endif
    void onModelDdcDetailedError(DdcDetailedError* perec);
    void onModelDdcFeatureError(DdcFeatureError* perec);

// *** Member variables ***

protected:
    static int nextId;

private:        // member variables
    const char *       _cls;
    const int          _id;
    Monitor *          _monitor;
    FeatureBaseModel * _baseModel;
    QStackedWidget *   _centralStackedWidget;
    MsgBoxQueue *      _msgboxQueue;

#ifdef PASS_NC_PARMS
    NcValuesSource     _curNcValuesSource;    // how to properly initialize?
    bool               _curUseLatestNcValueNames;
#endif
    bool               _controlKeyRequired;
    FeaturesScrollAreaContents * _scrollAreaContents = NULL;

    // quick and dirty for now, eventually replace by hash
    FeatureWidget *    _widgets[256] = {0};
};
#endif // FEATURES_SCROLLAREA_VIEW_H
