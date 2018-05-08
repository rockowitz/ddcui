/* feature_list_widget.h - List widget containing FeatureListWidgets */

#ifndef FEATURE_LIST_WIDGET_H
#define FEATURE_LIST_WIDGET_H

#include "alt/list_widget/feature_widget_extended.h"

#include <QtWidgets/QWidget>

#include <ddcutil_c_api.h>

#include "nongui/feature_base_model.h"
#ifdef FEATURE_CHANGE
#include "nongui/feature_change_observer.h"
#endif


class FeatureListWidget : public QListWidget
#ifdef FEATURE_LIST_WIDGET
                        , public FeatureChangeObserver    // no longer needed
#endif
{
    Q_OBJECT

public:
    FeatureListWidget(QWidget *parent = Q_NULLPTR);

    void setModel(FeatureBaseModel * baseModel);

    const char * _cls;    // className

    void dbgrpt() const;

public slots:

#ifdef NO
    void  modelVcpValueSet(
                       uint8_t                              feature_code,
                       DDCA_MCCS_Version_Spec               vspec,
                       DDCA_Simplified_Version_Feature_Info feature_flags,
                       DDCA_Non_Table_Value *               feature_value);

    void  modelVcpValueUpdate(
                       uint8_t                              feature_code,
                       uint8_t                              sh,
                       uint8_t                              sl);
#endif
    // void modelMccsVersionSet(DDCA_MCCS_Version_Spec    vspec);
    // DDCA_MCCS_Version_Spec mccsVersionSpec();

    void            startInitialLoad(void);
    void            endInitialLoad(void);

    void            featureAdded(FeatureValue fv);
    void            featureUpdated(char feature_code);

    void            featureChanged(uint8_t feature_code); //  override;    // virtual

protected:
    void            paintEvent(QPaintEvent *event);

private:
    int             findFeatureItem(uint8_t feature_code);
    FeatureWidgetExtended * getFeatureItem(uint8_t feature_code) ;
    void            addFeature(FeatureValue * fv);
    void            updateFeature(FeatureValue * fv);

    FeatureBaseModel * _baseModel;
};

#endif // FEATURE_LIST_WIDGET_H
