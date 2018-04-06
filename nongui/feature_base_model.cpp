/* feature_base_model.cpp - UI independent portion of the data model */

#include <assert.h>
#include <iostream>

#include <QtCore/QVector>
#include <QtCore/QByteArray>

#include "ddcutil_types.h"

#include "nongui/feature_value.h"

#include "nongui/feature_base_model.h"

using namespace std;

FeatureBaseModel::FeatureBaseModel()
{
    _cls                    = metaObject()->className();
    _featureValues          = new QVector<FeatureValue*>();
    _featureChangeObservers = new QVector<FeatureChangeObserver*>;
}


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
    // printf("(FeatureBaseModel::%s) this=%p\n", __func__, this); fflush(stdout);

    //    QString qobjname = objectName();
    //    std::string str_objname = qobjname.toStdString();
    //    const char * objname = str_objname.c_str();
    //    printf("(FeatureBaseModel::%s) objectName=%s\n", __func__, objname); fflush(stdout);

    // printf("(FeatureBaseModel::%s) _featureValues=%p\n", __func__, _featureValues); fflush(stdout);
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
 *  @param  metadata        feature metadata
 *  @param  feature_value   feature value 
 */
void   FeatureBaseModel::modelVcpValueSet(
                   uint8_t                              feature_code,
                   DDCA_Feature_Metadata                metadata,
                   DDCA_Non_Table_Vcp_Value *           feature_value)
{
    printf("(%s::%s) feature_code=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
           _cls, __func__, feature_code,
           feature_value->mh, feature_value->ml, feature_value->sh, feature_value->sl);
    int ndx = modelVcpValueIndex(feature_code);
    // printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
    // FIXME: HACK AROUND LOGIC ERROR
    // MAY DO A FULL LOAD MULTIPLE TIMES, E.G if switch table type
    // assert(ndx < 0);
    if (ndx < 0) {
        printf("(%s) Creating new FeatureValue\n", __func__); fflush(stdout);
        FeatureValue * fv = new FeatureValue();
        fv->_feature_code    = feature_code;
        fv->_vspec          = metadata.vspec;
        fv->_feature_flags  = metadata.feature_flags,
        fv->_mh             = feature_value->mh;
        fv->_ml             = feature_value->ml;
        fv->_sh             = feature_value->sh;
        fv->_sl             = feature_value->sl;
        fv->_value          = *feature_value;

        _featureValues->append(fv);
        printf("(%s::%s) Emitting signalFeatureAdded()\n", _cls, __func__); fflush(stdout);
        emit signalFeatureAdded(*fv);
        // notifyFeatureChangedObservers(feature_code);
        notifyFeatureChangeObservers(feature_code);
    }
    else {
// #ifdef OLD
        printf("(%s::%s) Modifying existing FeatureValue\n", _cls, __func__); fflush(stdout);
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

        printf("(%s) Emitting signalFeatureAdded()\n", __func__); fflush(stdout);
        emit signalFeatureAdded(*fv);
// #endif
    }
}


void   FeatureBaseModel::modelVcpValueUpdate(
                   uint8_t                              feature_code,
                   uint8_t                              sh,
                   uint8_t                              sl)
{
    printf("(%s::%s) feature_code=0x%02x, sh=0x%02x, sl=0x%02x\n",
           _cls, __func__, feature_code, sh, sl); fflush(stdout);
    int ndx = modelVcpValueIndex(feature_code);
    // printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
    assert (ndx >= 0);
    printf("(%s) Modifying existing FeatureValue\n", __func__); fflush(stdout);
    FeatureValue * fv =  _featureValues->at(ndx);
    fv->_sh             = sh;
    fv->_sl             = sl;

    fv->_value.sh = sh;
    fv->_value.sl = sl;

    emit signalFeatureUpdated(feature_code);
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


void  FeatureBaseModel::addFeatureChangeObserver(FeatureChangeObserver* observer) {
    _featureChangeObservers->append(observer);
}


void FeatureBaseModel::notifyFeatureChangeObservers(uint8_t feature_code) {
    printf("(%s::%s) Disabled\n", _cls, __func__);  fflush(stdout);
#ifdef DISABLED
    int ct = _featureChangeObservers->count();
    printf("(%s) Starting ct=%d\n", __func__, ct);  fflush(stdout);
    for (int ndx = 0; ndx < ct; ndx++) {
            printf("(%s) Notifying observer\n", __func__);  fflush(stdout);
            FeatureChangeObserver*  observer = _featureChangeObservers->at(ndx);
            observer->featureChanged(feature_code);
    }
#endif
}
