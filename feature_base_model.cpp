/* feature_base_model.cpp - UI independent portion of the data model */

#include <assert.h>
#include <iostream>

#include <QVector>
#include <QByteArray>

#include "ddcutil_types.h"
#include "feature_value.h"
#include "feature_base_model.h"


using namespace std;

FeatureBaseModel::FeatureBaseModel()
{
     _featureValues = new QVector<FeatureValue*>();

}

// void setMonitor(Monitor * monitor) {
//         _monitor = monitor;
// }


/** Returns the index of the entry for the specified feature in _feature_values
 *
 * \param feature_code
 * \return index of entry, -1 if not found
 */
int FeatureBaseModel::modelVcpValueIndex(uint8_t feature_code) {
    int result = -1;
    for (int ndx = 0; ndx < _featureValues->count(); ndx++) {
        FeatureValue * cur = _featureValues->at(ndx);
        if (cur->_feature_code == feature_code) {
            result = ndx;
            break;
        }
    }
    return result;
}


/** Returns the #FeatureValue instance for the specified feature.
 *
 * \param feature_code
 * \return pointer to FeatureValue instance, NULL if not found
 */
FeatureValue * FeatureBaseModel::modelVcpValueFind(uint8_t feature_code) {
    FeatureValue * result = NULL;
    for (int ndx = 0; ndx < _featureValues->count(); ndx++) {
        FeatureValue * cur = _featureValues->at(ndx);
        if (cur->_feature_code == feature_code) {
            result = cur;
            break;
        }
    }
    return result;
}

/** Returns a #FeatureValue instance based on its location in
 * #_featureValues
 *
 * \param  ndx   index in _featureValues vector
 * \return pointer to FeatureValue instance, NULL if invalid index
 */
FeatureValue * FeatureBaseModel::modelVcpValueAt(int ndx) const {
    assert(ndx >= 0);
    FeatureValue * result = NULL;
    if (ndx < _featureValues->count() )
        result = _featureValues->at(ndx);
    return result;
}


/** Returns the number of #FeatureValue instances. 
 */
int FeatureBaseModel::modelVcpValueCount(void) const {
    return _featureValues->count();
}


// IDEA: Separate function for update?
//      modelVcpValueAdd() 
//      modelVcpValueUpdate() 

/** Sets the current VCP value in the model.  
 *  This function is invoked from the ddcutil api side
 *  the note that actual VCP value. 
 * 
 *  @param  feature_code    VCP feature code
 *  @param  vspec           MCCS version 
 *  @param  feature_info    feature information 
 *  @param  feature_value   feature value 
 */
void   FeatureBaseModel::modelVcpValueSet(
                   uint8_t                              feature_code,
                   DDCA_MCCS_Version_Spec               vspec,
                   DDCA_Simplified_Version_Feature_Info feature_info,
                   DDCA_Non_Table_Value *               feature_value)
{
    printf("(modelVcpValueSet) feature_code=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
           feature_code, feature_value->mh, feature_value->ml, feature_value->sh, feature_value->sl);
    int ndx = modelVcpValueIndex(feature_code);
    printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
    assert(ndx < 0);
    if (ndx < 0) {
        printf("(%s) Creating new FeatureValue\n", __func__); fflush(stdout);
        FeatureValue * fv = new FeatureValue();
        fv->_feature_code    = feature_code;
        fv->_vspec          = vspec;
        fv->_feature_flags  = feature_info.feature_flags,
        fv->_mh             = feature_value->mh;
        fv->_ml             = feature_value->ml;
        fv->_sh             = feature_value->sh;
        fv->_sl             = feature_value->sl;
        fv->_value          = *feature_value;

        _featureValues->append(fv);
    }
    else {
#ifdef OLD
        printf("(%s) Modifying existing FeatureValue\n", __func__); fflush(stdout);
        FeatureValue * fv =  _featureValues->at(ndx);
       // FeatureValue * fv = modelVcpValueAt(ndx);
        // need to test _mh = 255, _ml = 0 ?
        // fv->_mh             = feature_value->mh;
        // fv->_ml             = feature_value->ml;
        fv->_sh             = feature_value->sh;
        fv->_sl             = feature_value->sl;

        // TODO: free old values

        fv->_value.sh = feature_value->sh;
        fv->_value.sl = feature_value->sl;
#endif
    }
}


void   FeatureBaseModel::modelVcpValueUpdate(
                   uint8_t                              feature_code,
                   uint8_t                              sh,
                   uint8_t                              sl)
{
    printf("(modelVcpValueUpdate) feature_code=0x%02x, sh=0x%02x, sl=0x%02x\n",
           feature_code, sh, sl); fflush(stdout);
    int ndx = modelVcpValueIndex(feature_code);
    printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
    assert (ndx >= 0);
    printf("(%s) Modifying existing FeatureValue\n", __func__); fflush(stdout);
    FeatureValue * fv =  _featureValues->at(ndx);
    fv->_sh             = sh;
    fv->_sl             = sl;

    fv->_value.sh = sh;
    fv->_value.sl = sl;
}



void FeatureBaseModel::modelMccsVersionSet(
     DDCA_MCCS_Version_Spec    vspec)
{
    _vspec = vspec;
}

DDCA_MCCS_Version_Spec FeatureBaseModel::mccsVersionSpec() {
    return _vspec;
}


/** Debugging function to report the contents of the current 
 *  #FeatureBaseModel instance. 
 */
// TODO: report feature_info, feature_name
void FeatureBaseModel::report() {
    printf("(FeatureBaseModel::report)\n");
    int ct = _featureValues->count();
    for (int ndx = 0; ndx < ct; ndx++) {
        FeatureValue* fv = _featureValues->at(ndx);
        // printf("   feature_code: 0x%02x\n", fv->_feature_code);
        printf("   code=0x%02x, mh=0x%02x, ml-0x%02x, sh=0x%02x, sl=0x%02x\n",
               fv->_feature_code,
               fv->_mh,
               fv->_ml,
               fv->_sh,
               fv->_sl);
    }
}


void  FeatureBaseModel::modelStartInitialLoad(void) {
    cout << "(FeatureBaseModel::modelStartInitialLoad()" << endl;
    signalStartInitialLoad();
}

void  FeatureBaseModel::modelEndInitialLoad(void) {
    cout << "(FeatureBaseModel::modelEndInitialLoad()" << endl;
    signalEndInitialLoad();
}
