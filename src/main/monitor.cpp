/* monitor.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "base/monitor.h"

#include <assert.h>
#include <string.h>

#include <QtCore/QVector>

#include <ddcutil_c_api.h>

#include "base/core.h"
#include "nongui/vcpthread.h"    // includes vcprequest.h
#include "nongui/feature_value.h"


Monitor::Monitor(DDCA_Display_Info * display_info, int monitorNumber)
    : _cls( strdup(metaObject()->className()) )
    , _monitorNumber(monitorNumber)
    , _displayInfo(display_info)
    , _baseModel(NULL)
    , _requestQueue(NULL)
{
   bool debug = false;
   TRACECF(debug, "Starting. monitorNumber=%d, dispno=%d, dref=%s",
                  monitorNumber, display_info->dispno, ddca_dref_repr(display_info->dref));
   _page_moninfo     = _page_capabilities     = NULL;
   _pageno_moninfo   = _pageno_capabilities   = 0;
   _moninfoPlainText = _capabilitiesPlainText = NULL;
   _vcpThread = NULL;

   // ddca_report_display_info(_displayInfo, 3);
   if (supportsDdc()) {
      _requestQueue = new VcpRequestQueue();
      _baseModel = new FeatureBaseModel(this);
      // baseModel->setObjectName(QString::asprintf("baseModel-%s",ddca_dref_repr(_displayInfo->dref));

      _vcpThread = new VcpThread(NULL, _displayInfo,  _requestQueue, _baseModel);

      // never triggered ??
      QObject::connect(_vcpThread, &VcpThread::finished,
                       this,       &Monitor::vcpThreadFinished);

      _vcpThread->start();
      // _vcp_threads.append(curThread);

      QObject::connect(_baseModel,  SIGNAL(signalVcpRequest(VcpRequest*)),
                       this, SLOT(  putVcpRequest(VcpRequest*)));
   }

   TRACECF(debug, "Done.     _monitorNumber=%d, dref: %s", _monitorNumber, ddca_dref_repr(_displayInfo->dref));
   // if (debug)
   //   ddca_report_display_info(_displayInfo, 3);
}


Monitor::~Monitor() {
   bool debug = false;
   TRACECF(debug, "Starting. monitor=%p, _monitor_number=%d, _displayInfo->dispno=%d, _baseModel=%p",
         this, _monitorNumber, _displayInfo->dispno, _baseModel);

   if (supportsDdc()) {
      _requestQueue->put(new HaltRequest());

      // wait for halt
      while (!_vcpThread->isFinished()) {
         TRACECF(debug, "Waiting for _vcpThread to finish");
         QThread::msleep(100);
      }
      TRACECF(debug, "_vcpThread finished");

      QObject::disconnect(_baseModel, SIGNAL(signalVcpRequest(VcpRequest*)),
                          this,       SLOT(  putVcpRequest(VcpRequest*)));
      delete _requestQueue;
      delete _vcpThread;
      delete _baseModel;
   }
   ddca_free_display_info(_displayInfo);
   TRACECF(debug, "Done");
   free((void*) _cls);
}


void Monitor::dbgrpt() {
   printf("_monitorNumber:      %d\n",    _monitorNumber);
   printf("_displayInfo->dref:  %p -> %s\n",     _displayInfo->dref, ddca_dref_repr(_displayInfo->dref));
   fflush(stdout);
}


DDCA_Display_Ref Monitor::getDref() {
   return _displayInfo->dref;
}


QString Monitor::dref_repr() {
   if (!_displayInfo->dref)
      return QString("No dref");
   else
      return   QString( ddca_dref_repr(_displayInfo->dref) );
}


DDCA_Feature_List
Monitor::getFeatureList(DDCA_Feature_Subset_Id feature_list_id) {
    bool debugFunc = debugFeatureLists;
    debugFunc = false;
    TRACECF(debugFunc,
          "feature_list_id=%d-%s",feature_list_id, ddca_feature_list_id_name(feature_list_id));

    bool include_table_features = false;    // TODO get from feature selection dialog
    DDCA_Status ddcrc = 0;
    DDCA_Feature_List result;
    if (_features.contains(feature_list_id)) {
        result = _features.value(feature_list_id);
    }
    else {
        // ddca_report_display_info(_displayInfo, 3);

        ddcrc = ddca_get_feature_list_by_dref(
                   feature_list_id, _displayInfo->dref, include_table_features, &result);
        TRACECF(debugFunc, "ddca_get_feature_list_by_dref() returned %d", ddcrc);
        if (ddcrc == 0) {
           // hack
           if (feature_list_id == DDCA_SUBSET_KNOWN) {
              DDCA_Feature_List mfgFeatureList;
              ddcrc = ddca_get_feature_list_by_dref(
                         DDCA_SUBSET_MFG, _displayInfo->dref, include_table_features, &mfgFeatureList);
              TRACECF(debugFunc, "ddca_get_feature_list_by_dref(DDCA_SUBSET_MFG) returned %d", ddcrc);
              if (ddcrc == 0) {
                 result = ddca_feature_list_or(result, mfgFeatureList);
              }
           }
           _features.insert(feature_list_id, result);
        }
    }
    TRACECF(debugFunc,
         "Returning: %d features: %s", ddca_feature_list_count(result),
                                       ddca_feature_list_string(result, NULL, (char*)" "));
    return result;
}


// consider replacing 2 booleans with an enum with 3 states

bool Monitor::capabilitiesCheckComplete() {
   bool debug = false;
   // considered complete if invalid display
   bool result = (supportsDdc());
   if (result)
      result = (_baseModel->_caps_check_complete);
   TRACECF(debug, "dref=%s, returning %s", QS2S(dref_repr()), SBOOL(result));
   return result;
}


bool Monitor::capabilitiesCheckSuccessful() {
   bool debug = false;
   bool result = (_displayInfo->dispno >  0);   // dispno -1 if API found display invalid, -2 if phantom
   if (result)
      result = (_baseModel->_caps_status == 0 && _baseModel->_parsed_caps);  // got capabilities?
   TRACECF(debug, "dref=%s, returning %s", QS2S(dref_repr()), SBOOL(result));
   return result;
}


bool Monitor::supportsDdc() {
   bool debug = false;
   bool result = (_displayInfo->dispno >  0);   // dispno -1 if API found display invalid, -2 if phantom
   TRACECF(debug, "dref=%s, returning %s", QS2S(dref_repr()), SBOOL(result));
   return result;
}


 // called by initFeaturesScrollAreaView
void Monitor::putVcpRequest(VcpRequest * rqst) {
    bool debug = false;
    TRACECF(debug, "-> rqst->type=%d. Adding request to monitor's request queue", rqst->_type);
    _requestQueue->put(rqst);
}


// never called!
void Monitor::vcpThreadFinished() {
   bool debug = true;
   TRACECF(debug, "vcp thread finished");
}



