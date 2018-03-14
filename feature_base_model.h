/* feature_base_model.h */

#ifndef FEATURE_BASE_MODEL_H
#define FEATURE_BASE_MODEL_H

#include <QtCore/QObject>

#include "ddcutil_types.h"

#include "feature_value.h"
// #include "monitor.h"   // errors because of order of includes
#include "vcprequest.h"
#include "feature_change_observer.h"

typedef void (*NotifyFeatureChanged)(uint8_t feature_code);


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

    // void  addFeatureChangedObserver(NotifyFeatureChanged func);
    void addFeatureChangeObserver(FeatureChangeObserver &observer);

signals:
    void signalStartInitialLoad(void);
    void signalEndInitialLoad(void);
    void signalFeatureAdded(FeatureValue& fv);
    void signalFeatureUpdated(char feature_code);
    void signalVcpRequest(VcpRequest * rqst);  // used to call into monitor


protected:
    // void notifyFeatureChangedObservers(uint8_t feature_code) ;
    void notifyFeatureChangeObservers(uint8_t feature_code) ;

private:
    int modelVcpValueIndex(uint8_t feature_code);

    QVector<FeatureValue*> * _featureValues;
    DDCA_MCCS_Version_Spec   _vspec;
    //  Monitor * _monitor;

    // QVector<NotifyFeatureChanged> *_featureChangedObservers;

    QVector<FeatureChangeObserver> * _featureChangeObservers;
};

#endif // FEATURE_BASE_MODEL_H
