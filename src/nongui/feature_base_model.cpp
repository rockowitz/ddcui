/* feature_base_model.cpp
 *
 * This was the UI independent portion of the data model when trying to use
 * QListView and QTableView.
 */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <iostream>
#include <typeinfo>

#include <QVector>
#include <QByteArray>

#include "ddcutil_types.h"
#include "ddcutil_c_api.h"
#include "ddcutil_status_codes.h"

#include "c_util/data_structures.h"

#include "base/ddcui_core.h"
#include "c_util/debug_util.h"
#include "base/ddcui_rtti.h"
#include "base/global_state.h"
#include "base/monitor.h"
#include "base/ddca_utils.h"

#include "nongui/feature_value.h"

#include "nongui/feature_base_model.h"

static bool debugModel = false;

FeatureBaseModel::FeatureBaseModel(Monitor * monitor)
      : _cls(strdup(metaObject()->className()))
      , _monitor(monitor)
{
    bool debug = false;
    TRACECF_STARTING(debug, "dref=%s", ddca_dref_repr(_monitor->_displayInfo->dref));

    setObjectName(QString::asprintf("baseModel-%s",ddca_dref_repr(_monitor->_displayInfo->dref)));
    _featureValues          = new QVector<FeatureValue*>();
#ifdef FEATURE_CHANGE_OBSERVER
    _featureChangeObservers = new QVector<FeatureChangeObserver*>;
#endif
    ddca_feature_list_clear(&_featuresChecked);

    // Features that need to be reread if feature x14 is changed
    // There ought to be a cleaner way
    _featuresTouchedByX14 = DDCA_EMPTY_FEATURE_LIST;
    ddca_feature_list_add(&_featuresTouchedByX14, 0x16);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x18);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x1a);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x6c);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x6e);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x70);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x0b);
    ddca_feature_list_add(&_featuresTouchedByX14, 0x0c);

    TRACECF_NOPREFIX(debug, "Constructor complete. _monitor -> dispno=%d, model=%s, dref=%p",
                   _monitor->_displayInfo->dispno,
                   _monitor->_displayInfo->model_name,
                   _monitor->_displayInfo->dref);
    TRACECF_DONE(debug, "");
}

FeatureBaseModel::~FeatureBaseModel() {
   bool debug = false;
   TRACECF_STARTING(debug, "Executing. _monitor=%p, monitor number %d, dref: %p",
                  _monitor, _monitor->_displayInfo->dispno, _monitor->_displayInfo->dref);
   for (int ndx = 0; ndx < _featureValues->size(); ndx++) {
      FeatureValue * fv = _featureValues->at(ndx);
      fv->delete_finfo();
   }
   delete _featureValues;
   TRACECF_NOPREFIX(debug, "          _caps+string=%p->%s", _caps_string, _caps_string);
   free(_caps_string);   // raw capabilities
   ddca_free_parsed_capabilities(_parsed_caps);
   TRACECF_DONE(debug, "");
   free((void*) _cls);
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
   if (ddca_feature_list_contains(_featuresToShow, feature_code)) {
      result = modelVcpValueFind(feature_code);
      if (result) {
         DDCA_Status ddcrc = result->ddcrc();
         bool showUnsupported = GlobalState::instance()._mainWindow->_feature_selector->_showUnsupportedFeatures;
         if (!showUnsupported && (ddcrc == DDCRC_REPORTED_UNSUPPORTED || ddcrc == DDCRC_DETERMINED_UNSUPPORTED)) {
            TRACECF_EVENT(debug, "Filtering out feature 0x%02x because UNSUPPORTED", result->featureCode());
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
 *  This function is invoked from VcpThread::getvcp()
 *
 *  ddcrc != 0 indicates that getvcp failed.
 *  This method notes the error for reporting.
 * 
 *  @param  feature_code    VCP feature code
 *  @param  dref
 *  @param  metadata        feature metadata
 *  @param  feature_value   feature value
 *  @param  ddcrc
 */
void   FeatureBaseModel::modelVcpValueSet(
                   uint8_t                              feature_code,
                   DDCA_Display_Ref                     dref,
                   DDCA_Feature_Metadata *              metadata,
                   DDCA_Non_Table_Vcp_Value *           feature_value,
                   DDCA_Status                          ddcrc)
{
    bool debugFunc = false;  // || (feature_code == 0x14);
    debugFunc = debugFunc || debugModel;
    TRACEMCF_STARTING(debugFunc,
                 "Starting. feature_code=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x, ddcrc = %s, _initialLoadActive=%s",
                 feature_code, feature_value->mh, feature_value->ml, feature_value->sh, feature_value->sl,
                 ddca_rc_name(ddcrc), SBOOL(_initialLoadActive));

    int ndx = modelVcpValueIndex(feature_code);
    if (ndx < 0) {
        // TRACECF_NOPREFIX(debugFunc, "Creating new FeatureValue");
        DDCA_Cap_Vcp * cap_vcp = NULL;
        if (_parsed_caps)
           cap_vcp = ddcu_find_cap_vcp(_parsed_caps, feature_code);

        FeatureValue * fv = new FeatureValue(
                                   feature_code,
                                   dref,
                                   metadata,
                                   cap_vcp,
                                   *feature_value,
                                   ddcrc);
        _featureValues->append(fv);
        TRACECF_NOPREFIX(debugFunc, "Created new FeatureValue. id = %d, observedNcValues=%s",
                           fv->_id, bs256_to_string_t(fv->_observedNcValues, "", " "));

        // Not needed, only thing that matters is end initial load
        // if (debugSignals)
        //     printf("(%s::%s) Emitting signalFeatureAdded()\n", _cls, __func__); fflush(stdout);
        // emit signalFeatureAdded(*fv);
        // notifyFeatureChangeObservers(feature_code);   // alternative
    }
    else {
        FeatureValue * fv =  _featureValues->at(ndx);
        TRACECF_NOPREFIX(debugFunc, "Modifying existing FeatureValue, _id=%d, initial _observedNcValues=%s",
                           fv->_id, bs256_to_string_t(fv->observedNcValues(), ""," " ) );

        // fv->_value.sh = feature_value->sh;
        // fv->_value.sl = feature_value->sl;

        if ( ddcrc == fv->ddcrc() ) {
           fv->setCurrentValue(feature_value->sh, feature_value->sl);  // sets _observedNcValues
           TRACECF_NOPREFIX(debugFunc, "Updated FeatureValue _observedNcValues=%s",
                 bs256_to_string_t(fv->observedNcValues(), ""," " ) );

           TRACECF_NOPREFIX(debugFunc || debugSignals,
                   "Emitting signalFeatureUpdated3(), feature code: 0x%02x, sl: 0x%02x",
                   fv->featureCode(), feature_value->sl);
           emit signalFeatureUpdated3(__func__,
                                      fv->featureCode(),
                                      feature_value->sh, feature_value->sl);
        }

        else {
           TRACEMCF_NOPREFIX(debugFunc, "Unexpected status code %s for previously read feature 0x%02x",
                    ddca_rc_name(ddcrc), fv->featureCode() );
        }
    }
    TRACEMCF_DONE(debugFunc, "");
}

// called from VcpThread::setvcp()
void
FeatureBaseModel::modelVcpValueUpdate(
        uint8_t   feature_code,
        uint8_t   sh,
        uint8_t   sl)
{
    bool debugFunc = false;  //  || (feature_code == 0x14);
    debugFunc = debugFunc || debugModel;

    TRACECF_STARTING(debugFunc, "feature_code=0x%02x, sh=0x%02x, sl=0x%02x, _initialLoadActive=%s",
          feature_code, sh, sl, SBOOL(_initialLoadActive));

    int ndx = modelVcpValueIndex(feature_code);
    assert (ndx >= 0);
    FeatureValue * fv =  _featureValues->at(ndx);
    TRACECF_NOPREFIX(debugFunc, "Found FeatureValue instance,  _observedNcValues=%s",
                       bs256_to_string_t(fv->observedNcValues(), ""," " ) );

    // updates fv to the current value, sets _observedNcValues if appropriate
    fv->setCurrentValue(sh,sl);

    TRACECF_NOPREFIX(debugFunc, "Updated FeatureValue: _observedNcValues=%s",
                       bs256_to_string_t(fv->observedNcValues(), ""," " ) );
    TRACECF_NOPREFIX(debugFunc || debugSignals, "Emitting signalFeatureUpdated3()");
    // -> &FeaturesScrollAreaView::onModelValueChanged
    emit signalFeatureUpdated3(__func__, feature_code, sh, sl);

#ifdef FUTURE
    if (!_initialLoadActive) {
       if (feature_code == 0x14) {
          TRACECF(debugFunc, "_featuresTouchedByX14: %s",
                  ddca_feature_list_string(_featuresTouchedByX14, "x", ", "));
          TRACECF(debugFunc, "_featuresToShow: %s",
                  ddca_feature_list_string(_featuresToShow, "x", ", "));
          DDCA_Feature_List featuresTouched =
             ddca_feature_list_and(_featuresToShow, _featuresTouchedByX14 );
          TRACECF(debugFunc, "featuresTouched: %s",
                  ddca_feature_list_string(featuresTouched, "x", ", "));
          // there ought to be an iterator
          for (int vcp_code = 0; vcp_code < 256; vcp_code++) {
             if ( ddca_feature_list_contains(featuresTouched, vcp_code) ) {
                TRACECF(debugFunc, "putting VcpGetRequest for feature 0x%02x on queue",
                                   vcp_code);
                bool needMetadata = false;
                _monitor->_requestQueue->put( new VcpGetRequest(vcp_code, needMetadata) );
             }
          }
       }
    }
#endif
   TRACECF_DONE(debugFunc, "");
}

// This really belongs in Monitor
void
FeatureBaseModel::setCapabilities(
      DDCA_Status          ddcrc,
      char *               capabilities_string,
      DDCA_Capabilities *  parsed_capabilities)
{
   bool debug = false;
   TRACECF_STARTING(debug, "ddcrc = %s, capabilities_string=|%s|", ddca_rc_name(ddcrc), capabilities_string);

   _caps_check_complete = true;
   _caps_status = ddcrc;
   _caps_string = capabilities_string;
   _parsed_caps = parsed_capabilities;
   TRACECF_DONE(debug, "");
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
      DDCA_Feature_List featureList)
{
   bool debugFunc = false;
   debugFunc = debugFunc || debugFeatureLists;
   TRACECF_STARTING(debugFunc, "%d features: %s",
                      ddca_feature_list_count(featureList),
                      ddca_feature_list_string(featureList, NULL, (char*) " ") );

   _featuresToShow = featureList;
   DDCA_Feature_List unchecked_features =
         ddca_feature_list_and_not(_featuresToShow, _featuresChecked);

   if (debugFeatureLists) {
       TRACECF_NOPREFIX(debugFunc, "Unchecked features: %s",
                ddca_feature_list_string(unchecked_features, NULL, (char*) " "));
   }

   //bool showUnsupported = _monitor->_curFeatureSelector._showUnsupportedFeatures;
   // TRACEF(debugFunc, "_monitor->curFeatureSelector._showUnsupportedFeatures = %s", sbool(showUnsupported));

   _monitor->_requestQueue->put(new VcpStartInitialLoadRequest);
   for (int ndx = 0; ndx <= 255; ndx++) {
       uint8_t vcp_code = (uint8_t) ndx;
       if ( ddca_feature_list_contains(unchecked_features, vcp_code)) {
           bool needMetadata = true;
           _monitor->_requestQueue->put( new VcpGetRequest(vcp_code, needMetadata) );
       }
   }
   _monitor->_requestQueue->put(new VcpEndInitialLoadRequest);

   TRACECF_DONE(debugFunc, "");
}

// reload specific features, but only if they were already loaded
// used when setting one feature requires rereading others

void
FeatureBaseModel::reloadSpecificFeatures(int ct, uint8_t* features) {
   bool debugFunc =true;
   TRACECF_STARTING(debugFunc, "ct=%d", ct);

   for (int ndx = 0; ndx < ct; ndx++) {
      DDCA_Vcp_Feature_Code vcp_code = features[ndx];
      if (ddca_feature_list_contains(_featuresChecked, vcp_code)) {
         TRACECF_NOPREFIX(debugFunc, "vcp_code = 0x%02x, in _features_checked", vcp_code);
         FeatureValue *  fv = modelVcpValueFind(vcp_code);
         // should always exist, but just in case
         if (fv) {
            bool needMetadata = false;
            TRACECF_NOPREFIX(debugFunc, "Putting VcpGetRequest(0x%02x) on _requestQueue", vcp_code);
            _monitor->_requestQueue->put( new VcpGetRequest(vcp_code, needMetadata));
         }
         else
            TRACECF_NOPREFIX(debugFunc, "FeatureValue for 0x%02x not found", vcp_code);
      }
      else {
         TRACECF_NOPREFIX(debugFunc, "vcp_code = 0x%02x not in _features_checked", vcp_code);
      }
   }
   TRACECF_DONE(debugFunc, "");
}

void FeatureBaseModel::setFeatureChecked(uint8_t featureCode) {
   ddca_feature_list_add(&_featuresChecked, featureCode);
}

// called from MainWindow::on_actionRescan_triggered(),
// FeaturesScrollAreaView::onUIValueChanged() - reloading for features that affect multiple
void FeatureBaseModel::reloadFeatures() {
   bool debug = false;
   debug = debug || debugFeatureLists;

   TRACECF_STARTING(debug, "");

   _monitor->_requestQueue->put(new VcpStartInitialLoadRequest);
   int ct = modelVcpValueCount();
   for (int ndx = 0; ndx < ct; ndx++) {
      FeatureValue* fv =  modelVcpValueAt(ndx);
      // only rescan if previous scan was successful
      // the logic modelVcpValueSet is too convoluted if not
      if (fv->ddcrc() == 0) {
         DDCA_Feature_Flags flags = fv->flags();
         if (flags & DDCA_READABLE) {
            uint8_t featureCode = fv->featureCode();
            bool needMetadata = false;
            _monitor->_requestQueue->put( new VcpGetRequest(featureCode, needMetadata));
         }
      }
   }
   _monitor->_requestQueue->put(new VcpEndInitialLoadRequest);

   TRACECF_DONE(debug, "");
}

void FeatureBaseModel::markDisconnected(DDCA_Display_Ref dref) {
   bool debug = true;
   TRACECF_STARTING(debug, "DDCA_Display_Ref = %p", dref);
   _monitor->markDisconnected();   // or should this be emit signalDisconnected(dref) ?
   // GlobalState& _globalState = GlobalState::instance();
    TRACECF_NOPREFIX(debug, "emitting signalEndInitialLoad()");
   emit signalEndInitialLoad();
   TRACECF_DONE(debug, "");
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
        printf("   code=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
               fv->featureCode(),
               fv->val().mh,
               fv->val().ml,
               fv->val().sh,
               fv->val().sl);
    }
}

void  FeatureBaseModel::modelStartInitialLoad(void) {
   bool debug = false;
   _initialLoadActive = true;
    TRACECF_EVENT(debug, "=> Emitting signalStartInitialLoad");
    emit signalStartInitialLoad();
}

void  FeatureBaseModel::modelEndInitialLoad(void) {
   bool debug = false;
   _initialLoadActive = false;
    TRACECF_EVENT(debug, "=> Emitting signalEndInitialLoad");
    emit signalEndInitialLoad();   // claude added emit - is there a reason this was a simple call?
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

void init_feature_base_model() {
   bool debug = false;
   DBGF(debug, "Starting");
   RTTI_ADD_METHOD(FeatureBaseModel::FeatureBaseModel);
   RTTI_ADD_METHOD(FeatureBaseModel::~FeatureBaseModel);
   RTTI_ADD_METHOD(FeatureBaseModel::modelVcpValueUpdate);
   RTTI_ADD_METHOD(FeatureBaseModel::setCapabilities);
   RTTI_ADD_METHOD(FeatureBaseModel::setFeatureList);
   RTTI_ADD_METHOD(FeatureBaseModel::reloadFeatures);
   RTTI_ADD_METHOD(FeatureBaseModel::markDisconnected);
   DBGF(debug, "Done");
}
