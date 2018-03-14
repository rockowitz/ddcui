#ifndef FEATURE_LIST_WIDGET_H
#define FEATURE_LIST_WIDGET_H

#include <QWidget>

#include "feature_base_model.h"
#include <ddcutil_c_api.h>
#include "feature_widget.h"
#include "feature_change_observer.h"


class FeatureListWidget : public QListWidget, public FeatureChangeObserver
{
    Q_OBJECT

public:
    FeatureListWidget(QWidget *parent = Q_NULLPTR);

    void setModel(FeatureBaseModel * baseModel);

public slots:
#ifdef NO
    void           modelVcpValueSet(
                       uint8_t                              feature_code,
                       DDCA_MCCS_Version_Spec               vspec,
                       DDCA_Simplified_Version_Feature_Info feature_flags,
                       DDCA_Non_Table_Value *               feature_value);

    void           modelVcpValueUpdate(
                       uint8_t                              feature_code,
                       uint8_t                              sh,
                       uint8_t                              sl);
#endif
    // void modelMccsVersionSet(DDCA_MCCS_Version_Spec    vspec);
    // DDCA_MCCS_Version_Spec mccsVersionSpec();

    void          startInitialLoad(void);
    void          endInitialLoad(void);

    void          featureAdded(FeatureValue& fv);
    void          featureUpdated(char feature_code);

    // void          featureChangedObserver(uint8_t feature_code);

    virtual void featureChanged(uint8_t feature_code) override;    // virtual


private:
    FeatureBaseModel * _baseModel;
    void addFeature(FeatureValue * fv);
    int findFeatureItem(uint8_t feature_code);
    FeatureWidget * getFeatureItem(uint8_t feature_code) ;
    void updateFeature(FeatureValue * fv);
};

#endif // FEATURE_LIST_WIDGET_H
