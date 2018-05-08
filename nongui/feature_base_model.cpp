/* feature_base_model.cpp - UI independent portion of the data model */

#include <assert.h>
#include <iostream>

#include <QtCore/QVector>
#include <QtCore/QByteArray>

#include <ddcutil_c_api.h>

#include "base/ddcui_globals.h"
#include "base/monitor.h"
#include "base/ddca_utils.h"

#include "nongui/feature_value.h"

#include "nongui/feature_base_model.h"

using namespace std;

static bool debugModel = false;

FeatureBaseModel::FeatureBaseModel(Monitor * monitor)
{
    _cls                    = metaObject()->className();
    _monitor                = monitor;
    _featureValues          = new QVector<FeatureValue*>();
#ifdef FEATURE_CHANGE_OBSERVER
    _featureChangeObservers = new QVector<FeatureChangeObserver*>;
#endif
    ddca_feature_list_clear(&_featuresChecked);
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
    for (int ndx = 0; ndx < _featureValues->count(); ndx++) {
        FeatureValue * cur = _featureValues->at(ndx);
        if (cur->_feature_code == feature_code) {
            result = cur;
            break;
        }
    }
    return result;
}


FeatureValue * FeatureBaseModel::modelVcpValueFilteredFind(uint8_t feature_code) {
   FeatureValue * result = NULL;
   if (ddca_feature_list_contains(&_featuresToShow, feature_code)) {
      result = modelVcpValueFind(feature_code);
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
                   DDCA_Display_Ref                     dref,
                   DDCA_Feature_Metadata                metadata,
                   DDCA_Non_Table_Vcp_Value *           feature_value)
{
    if (debugModel)
        PRINTFCM("feature_code=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x",
                 feature_code, feature_value->mh, feature_value->ml, feature_value->sh, feature_value->sl);

    int ndx = modelVcpValueIndex(feature_code);
    // printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
    // FIXME: HACK AROUND LOGIC ERROR
    // MAY DO A FULL LOAD MULTIPLE TIMES, E.G if switch table type
    // assert(ndx < 0);
    if (ndx < 0) {
        if (debugModel)
            PRINTFCM("Creating new FeatureValue");

        DDCA_Cap_Vcp * cap_vcp = NULL;
        if (_parsed_caps)
           cap_vcp = ddcutil_find_cap_vcp(_parsed_caps, feature_code);

        FeatureValue * fv = new FeatureValue(
                                   feature_code,
                                   dref,
                                   metadata,
                                   cap_vcp,
                                   *feature_value);
        _featureValues->append(fv);

        // Not needed, only thing that matters is end initial load
        // if (debugSignals)
        //     printf("(%s::%s) Emitting signalFeatureAdded()\n", _cls, __func__); fflush(stdout);
        // emit signalFeatureAdded(*fv);
        // notifyFeatureChangeObservers(feature_code);   // alternative
    }
    else {
        // PRINTFCM("Modifying existing FeatureValue");

        FeatureValue * fv =  _featureValues->at(ndx);
        fv->_value.sh = feature_value->sh;
        fv->_value.sl = feature_value->sl;

        PRINTFCM("Emitting signalFeatureUpdated3(), feature code: 0x%02x, sl: 0x%02x",
                 fv->_feature_code, feature_value->sl);
        emit signalFeatureUpdated3(__func__, fv->_feature_code, feature_value->sh, feature_value->sl);
    }
}


void
FeatureBaseModel::modelVcpValueUpdate(
        uint8_t   feature_code,
        uint8_t   sh,
        uint8_t   sl)
{
    PRINTFCMF(debugModel, "feature_code=0x%02x, sh=0x%02x, sl=0x%02x", feature_code, sh, sl);

    int ndx = modelVcpValueIndex(feature_code);
    // printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
    assert (ndx >= 0);
    // PRINTFCMF(debugModel, "Modifying existing FeatureValue");

    FeatureValue * fv =  _featureValues->at(ndx);
    fv->_value.sh = sh;
    fv->_value.sl = sl;

    // PRINTFCMF(debugModel, "Emitting signalFeatureUpdated()");
    // emit signalFeatureUpdated(feature_code);
    PRINTFCMF(debugModel, "Emitting signalFeatureUpdated3()");
    emit signalFeatureUpdated3(__func__, feature_code, sh, sl);
}


// This really belongs in Monitor
void
FeatureBaseModel::setCapabilities(
      DDCA_Status          ddcrc,
      char *               capabilities_string,
      DDCA_Capabilities *  parsed_capabilities)
{
   PRINTFCM("capabilities_string=|%s|", capabilities_string);

   _caps_status = ddcrc;
   _caps_string = capabilities_string;
   _parsed_caps = parsed_capabilities;
}


void
FeatureBaseModel::onDdcError(
      DdcError& erec)
{
   // PRINTFCM("erec=%s", erec.srepr() );
   // emit signalModelError(featureCode, msg);
   emit  signalDdcError(erec);
}


void
FeatureBaseModel::modelMccsVersionSet(
     DDCA_MCCS_Version_Spec    vspec)
{
    _vspec = vspec;
}


DDCA_MCCS_Version_Spec
FeatureBaseModel::mccsVersionSpec()
{
    return _vspec;
}


void
FeatureBaseModel::setFeatureList(
      DDCA_Feature_List featureList)
{
   _featuresToShow = featureList;

   DDCA_Feature_List unchecked_features =
         ddca_feature_list_and_not(&_featuresToShow, &_featuresChecked);

   if (debugFeatureLists) {
       char * s = ddca_feature_list_string(&unchecked_features, NULL, (char*) " ");
       PRINTFCM("Unchecked features: %s", s);
   }

#ifdef OLD
   cout << "Unchecked features: " << endl;
   for (int ndx = 0; ndx <= 255; ndx++) {
       if ( ddca_feature_list_contains(&unchecked_features, (uint8_t) ndx))
           printf("%02x ", ndx);
   }
   cout << endl;
#endif

   _monitor->_requestQueue->put(new VcpStartInitialLoadRequest);

   for (int ndx = 0; ndx <= 255; ndx++) {
       uint8_t vcp_code = (uint8_t) ndx;
       if ( ddca_feature_list_contains(&unchecked_features, vcp_code)) {
           _monitor->_requestQueue->put( new VcpGetRequest(vcp_code));
       }
   }
   _monitor->_requestQueue->put(new VcpEndInitialLoadRequest);
}


void FeatureBaseModel::setFeatureChecked(uint8_t featureCode) {
   ddca_feature_list_add(&_featuresChecked, featureCode);
}



/** Debugging function to report the contents of the current 
 *  #FeatureBaseModel instance. 
 */
// TODO: report feature_info, feature_name
void FeatureBaseModel::dbgrpt() {
    printf("(FeatureBaseModel::report)\n");
    int ct = _featureValues->count();
    for (int ndx = 0; ndx < ct; ndx++) {
        FeatureValue* fv = _featureValues->at(ndx);
        // printf("   feature_code: 0x%02x\n", fv->_feature_code);
        printf("   code=0x%02x, mh=0x%02x, ml-0x%02x, sh=0x%02x, sl=0x%02x\n",
               fv->_feature_code,
               fv->_value.mh,
               fv->_value.ml,
               fv->_value.sh,
               fv->_value.sl);
    }
}


void  FeatureBaseModel::modelStartInitialLoad(void) {
    PRINTFCM("Emitting signalStartInitialLoad");
    emit signalStartInitialLoad();
}

void  FeatureBaseModel::modelEndInitialLoad(void) {
    PRINTFCM("Emitting signalEndInitialLoad");
    signalEndInitialLoad();
}

void  FeatureBaseModel::setStatusMsg(QString msg) {
   // printf("(%s::%s) msg=%s\n", _cls, __func__, msg.toLatin1().data());  fflush(stdout);
   emit signalStatusMsg(msg);
}


#ifdef FEATURE_CHANGE_OBSERVER
void  FeatureBaseModel::addFeatureChangeObserver(FeatureChangeObserver* observer) {
    _featureChangeObservers->append(observer);
}


void FeatureBaseModel::notifyFeatureChangeObservers(uint8_t feature_code) {
    if (debugSignals)
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
#endif
