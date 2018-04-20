#ifndef FEATURES_SCROLLAREA_VIEW_H
#define FEATURES_SCROLLAREA_VIEW_H

// Manages the scroll area that provides a view onto the current feature settings
// of a monitor

#include <QtCore/QObject>

#include "nongui/ddc_error.h"
#include "nongui/feature_base_model.h"
#include "feature_scrollarea/feature_widget_basic.h"

// #include "monitor.h"

class QStackedWidget;
class Monitor;

class FeaturesScrollAreaView : public QObject
{
    Q_OBJECT

public:
    explicit FeaturesScrollAreaView(
            Monitor *          monitor,    // needed?  complicates dependency order?
            FeatureBaseModel * model,
            QStackedWidget *   centralStackedWidget,
            QObject *          parent = nullptr);

    QStackedWidget *   _centralStackedWidget;
    FeatureBaseModel * _baseModel;
    Monitor *          _monitor;

private:
    const char * _cls;

    // quick and dirty for now, eventually replace by hash
    FeatureWidgetBasic * _widgets[256] = {0};

signals:
    void signalVcpRequest(VcpRequest * rqst);  // used to call into monitor

public slots:
    // void startInitialLoad(void);
    void onEndInitialLoad(void);
    // void featureAdded(FeatureValue fv);
    // void featureUpdated(char feature_code);

    void onUIValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

    void onModelValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl);

   void onModelDdcError(DdcError erec);
};
#endif // FEATURES_SCROLLAREA_VIEW_H
