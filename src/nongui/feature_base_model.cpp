/* feature_base_model.cpp - UI independent portion of the data model */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "nongui/feature_base_model.h"

#include <assert.h>
#include <iostream>
#include <typeinfo>

#include <QtCore/QVector>
#include <QtCore/QByteArray>

#include <ddcutil_types.h>
#include <ddcutil_c_api.h>
#include <ddcutil_status_codes.h>

#include "base/global_state.h"
#include "base/ddcui_globals.h"
#include "base/monitor.h"
#include "base/ddca_utils.h"

#include "nongui/feature_value.h"


using namespace std;

static bool debugModel = false;

FeatureBaseModel::FeatureBaseModel(Monitor * monitor)
{
    _cls                    = strdup(metaObject()->className());
    _monitor                = monitor;
    _featureValues          = new QVector<FeatureValue*>();
#ifdef FEATURE_CHANGE_OBSERVER
    _featureChangeObservers = new QVector<FeatureChangeObserver*>;
#endif
    ddca_feature_list_clear(&_featuresChecked);
}


FeatureBaseModel::~FeatureBaseModel() {
   delete _featureValues;
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
        if (cur->featureCode() == feature_code) {
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
        if (cur->featureCode() == feature_code) {
            result = cur;
            break;
        }
    }
    return result;
}


FeatureValue * FeatureBaseModel::modelVcpValueFilteredFind(uint8_t feature_code) {
   bool debug = false;
   FeatureValue * result = NULL;
   if (ddca_feature_list_contains(&_featuresToShow, feature_code)) {
      result = modelVcpValueFind(feature_code);
      if (result) {
         DDCA_Status ddcrc = result->ddcrc();
         bool showUnsupported = GlobalState::instance()._mainWindow->_feature_selector->_showUnsupportedFeatures;
         if (!showUnsupported && (ddcrc == DDCRC_REPORTED_UNSUPPORTED || ddcrc == DDCRC_DETERMINED_UNSUPPORTED)) {
            TRACEF(debug, "Filtering out feature 0x%02x because UNSUPPORTED", result->featureCode());
            result = NULL;
         }
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
                   DDCA_Display_Ref                     dref,
                   DDCA_Feature_Metadata *              metadata,
                   DDCA_Non_Table_Vcp_Value *           feature_value,
                   DDCA_Status                          ddcrc)
{
    bool debugFunc = debugModel;
    debugFunc = false;
    if (debugFunc)
        TRACE("feature_code=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x, ddcrc = %s",
                 feature_code, feature_value->mh, feature_value->ml, feature_value->sh, feature_value->sl,
                 ddca_rc_name(ddcrc));

    _featureStatusCode[feature_code] = ddcrc;
    // if (ddcrc == 0) {
       int ndx = modelVcpValueIndex(feature_code);
       // printf("(%s) ndx=%d\n", __func__, ndx);  fflush(stdout);
       // FIXME: HACK AROUND LOGIC ERROR
       // MAY DO A FULL LOAD MULTIPLE TIMES, E.G if switch table type
       // assert(ndx < 0);
       if (ndx < 0) {
           TRACEF(debugFunc, "Creating new FeatureValue");

           DDCA_Cap_Vcp * cap_vcp = NULL;
           if (_parsed_caps)
              cap_vcp = ddcutil_find_cap_vcp(_parsed_caps, feature_code);

           FeatureValue * fv = new FeatureValue(
                                      feature_code,
                                      dref,
                                      metadata,
                                      cap_vcp,
                                      *feature_value,
                                      ddcrc);
           _featureValues->append(fv);
           if (metadata) {
             _featureMetadata[feature_code] = metadata;
           }

           // Not needed, only thing that matters is end initial load
           // if (debugSignals)
           //     printf("(%s::%s) Emitting signalFeatureAdded()\n", _cls, __func__); fflush(stdout);
           // emit signalFeatureAdded(*fv);
           // notifyFeatureChangeObservers(feature_code);   // alternative
       }
       else {
           // TRACE("Modifying existing FeatureValue");

           FeatureValue * fv =  _featureValues->at(ndx);
           // fv->_value.sh = feature_value->sh;
           // fv->_value.sl = feature_value->sl;

           // should never fail since it's previously succeeded, but just in case
           if (ddcrc ==0) {
              fv->setCurrentValue(feature_value->sh, feature_value->sl);

           TRACEF(debugFunc, "=> Emitting signalFeatureUpdated3(), feature code: 0x%02x, sl: 0x%02x",
                    fv->featureCode(), feature_value->sl);
           emit signalFeatureUpdated3(__func__, fv->featureCode(), feature_value->sh, feature_value->sl);
           }
           else{
              TRACE("Unexpected status code %s for previously read feature 0x%02x",
                   ddca_rc_name(ddcrc), feature_code);
       }
    }

}


void
FeatureBaseModel::modelVcpValueUpdate(
        uint8_t   feature_code,
        uint8_t   sh,
        uint8_t   sl)
{
    bool debugFunc = debugModel;
    debugFunc = true;
    TRACEF(debugFunc, "feature_code=0x%02x, sh=0x%02x, sl=0x%02x", feature_code, sh, sl);

    int ndx = modelVcpValueIndex(feature_code);
    assert (ndx >= 0);
    FeatureValue * fv =  _featureValues->at(ndx);
    fv->setCurrentValue(sh,sl);

    TRACEF(debugFunc || debugSignals, "Emitting signalFeatureUpdated3()");
    emit signalFeatureUpdated3(__func__, feature_code, sh, sl);
}


// This really belongs in Monitor
void
FeatureBaseModel::setCapabilities(
      DDCA_Status          ddcrc,
      char *               capabilities_string,
      DDCA_Capabilities *  parsed_capabilities)
{
   // TRACE("capabilities_string=|%s|", capabilities_string);

   _caps_status = ddcrc;
   _caps_string = capabilities_string;
   _parsed_caps = parsed_capabilities;
}


void
FeatureBaseModel::onDdcDetailedError(
      DdcDetailedError* perec)
{
   bool debugFunc = debugModel;
   // debugFunc = true;

   // TRACEF(debugFunc, "Starting");
   TRACEF(debugFunc, "perec=%p -> %s", perec, perec->srepr() );
   // TRACEF(debugFunc, "perec=%p -> %s", perec, perec->sexpl() );
   TRACEF(debugFunc, "=> emitting signalDdcDetailedError()");
   emit  signalDdcDetailedError(perec);
}


void
FeatureBaseModel::onDdcFeatureError(
      DdcFeatureError* perec)
{
   bool debugFunc = debugModel;
   // debugFunc = true;
   TRACEF(debugFunc, "perec=%p -> %s", perec, qs2s(perec->repr()) );
   // std::cout << "typeid(perec):  " << typeid(perec).name()  << std::endl;
   // std::cout << "typeid(*perec): " << typeid(*perec).name() << std::endl;
   // emit signalModelError(featureCode, msg);
   TRACEF(debugFunc, "=> emitting signalDdcFeatureError()");
   emit  signalDdcFeatureError(perec);
}


#ifdef UNUSED
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
#endif


void
FeatureBaseModel::setFeatureList(
      DDCA_Feature_List featureList,
      bool              reportUnsupported)
{
   bool debugFunc = debugFeatureLists;
   debugFunc = true;
   TRACEF(debugFunc, "Starting. %d features: %s, reportUnsupported: %s",
                     ddca_feature_list_count(&featureList),
         ddca_feature_list_string(&featureList, NULL, (char*) " "),
         sbool(reportUnsupported) );
   _featuresToShow = featureList;

   DDCA_Feature_List unchecked_features =
         ddca_feature_list_and_not(&_featuresToShow, &_featuresChecked);

   if (debugFeatureLists) {
       TRACEF(debugFunc, "Unchecked features: %s",
                ddca_feature_list_string(&unchecked_features, NULL, (char*) " "));
   }

   //bool showUnsupported = _monitor->_curFeatureSelector._showUnsupportedFeatures;
   // TRACEF(debugFunc, "_monitor->curFeatureSelector._showUnsupportedFeatures = %s", sbool(showUnsupported));
   _monitor->_requestQueue->put(new VcpStartInitialLoadRequest);
   for (int ndx = 0; ndx <= 255; ndx++) {
       uint8_t vcp_code = (uint8_t) ndx;
       if ( ddca_feature_list_contains(&unchecked_features, vcp_code)) {
           bool needMetadata = true;
           _monitor->_requestQueue->put( new VcpGetRequest(vcp_code, needMetadata) );
       }
   }
   _monitor->_requestQueue->put(new VcpEndInitialLoadRequest);

   TRACEF(debugFunc, "Done");
}

// reload specific features, but only if they were already loadeed
// used when setting one feature requires rereading others

void
FeatureBaseModel::reloadSpecificFeatures(int ct, uint8_t* features) {
   bool debugFunc = true;
   for (int ndx = 0; ndx < ct; ndx++) {
      DDCA_Vcp_Feature_Code vcp_code = features[ndx];
      if (ddca_feature_list_contains(& _featuresChecked, vcp_code)) {
         TRACEF(debugFunc, "vcp_code = 0x%02x, in _features_checked", vcp_code);
         FeatureValue *  fv = modelVcpValueFind(vcp_code);
         // should always exist, but just in case
         if (fv) {
            bool needMetadata = false;
            TRACE("Putting VcpGetRequest(0x%02x) on _requestQueue", vcp_code);
            _monitor->_requestQueue->put( new VcpGetRequest(vcp_code, needMetadata));
         }
         else
            TRACE("FeatureValue for 0x%02x not found", vcp_code);
      }
      else {
         TRACEF(debugFunc, "vcp_code = 0x%02x not in _features_checked", vcp_code);
      }

   }

}

void FeatureBaseModel::setFeatureChecked(uint8_t featureCode) {
   ddca_feature_list_add(&_featuresChecked, featureCode);
}


void FeatureBaseModel::reloadFeatures() {
   bool debugFunc = debugFeatureLists;
   debugFunc = false;
   TRACEF(debugFunc, "Starting.");

   _monitor->_requestQueue->put(new VcpStartInitialLoadRequest);
   int ct = modelVcpValueCount();
   for (int ndx = 0; ndx < ct; ndx++) {
      FeatureValue* fv =  modelVcpValueAt(ndx);
      uint8_t featureCode = fv->featureCode();
      DDCA_Feature_Flags flags = fv->flags();
      if (flags & DDCA_RW) {
         bool needMetadata = true;  // a rare possibility
         _monitor->_requestQueue->put( new VcpGetRequest(featureCode, needMetadata));
      }
   }
   _monitor->_requestQueue->put(new VcpEndInitialLoadRequest);

   TRACEF(debugFunc, "Done");
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
               fv->featureCode(),
               fv->val().mh,
               fv->val().ml,
               fv->val().sh,
               fv->val().sl);
    }
}


void  FeatureBaseModel::modelStartInitialLoad(void) {
    // TRACE("=> Emitting signalStartInitialLoad");
    emit signalStartInitialLoad();
}


void  FeatureBaseModel::modelEndInitialLoad(void) {
    // TRACE("=> Emitting signalEndInitialLoad");
    signalEndInitialLoad();
}


void  FeatureBaseModel::setStatusMsg(QString msg) {
   // printf("(%s::%s) => msg=%s\n", _cls, __func__, msg.toLatin1().data());  fflush(stdout);
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
