/* feature_base_model.h */

#ifndef FEATURE_BASE_MODEL_H
#define FEATURE_BASE_MODEL_H

#include <QObject>

#include "ddcutil_types.h"

#include "feature_value.h"
// #include "monitor.h"   // errors because of order of includes
#include "vcprequest.h"


/** The UI independent portion of the QT feature data model.
 *  Records all VCP values for a single monitor.
 */
class FeatureBaseModel : public QObject
{
    Q_OBJECT

public:
    FeatureBaseModel();

    // void setMonitor(Monitor * monitor);

    FeatureValue * modelVcpValueFind(uint8_t feature_code);
    FeatureValue * modelVcpValueAt(int ndx) const;
    int            modelVcpValueCount(void) const;

    void           modelVcpValueSet(
                       uint8_t                              feature_code,
                       DDCA_MCCS_Version_Spec               vspec,
                       DDCA_Simplified_Version_Feature_Info feature_flags,
                       DDCA_Non_Table_Value *               feature_value);

    void           modelVcpValueUpdate(
                       uint8_t                              feature_code,
                       uint8_t                              sh,
                       uint8_t                              sl);

    void modelMccsVersionSet(DDCA_MCCS_Version_Spec    vspec);
    DDCA_MCCS_Version_Spec mccsVersionSpec();

    void          modelStartInitialLoad(void);
    void          modelEndInitialLoad(void);

    void          report();

signals:
    void signalStartInitialLoad(void);
    void signalEndInitialLoad(void);
    void signalVcpRequest(VcpRequest * rqst);  // used to call into monitor

private:
    int modelVcpValueIndex(uint8_t feature_code);

    QVector<FeatureValue*> * _featureValues;
    DDCA_MCCS_Version_Spec   _vspec;
    //  Monitor * _monitor;
};

#endif // FEATURE_BASE_MODEL_H
