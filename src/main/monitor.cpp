/* monitor.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
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
    : _monitorNumber(monitorNumber)
    , _displayInfo(display_info)

    ,  _baseModel(NULL)
 //   ,  _cls(metaObject()->className()) // -Wreorder
    ,  _requestQueue(NULL)
{
   _cls = metaObject()->className();
   _page_moninfo     = _page_capabilities     = NULL;
   _pageno_moninfo   = _pageno_capabilities   = 0;
   _moninfoPlainText = _capabilitiesPlainText = NULL;

   // PRINTFCM("End of constructor");
   // ddca_report_display_info(_displayInfo, 3);
}


Monitor::~Monitor() {
}


void Monitor::dbgrpt() {
   printf("_monitorNumber:      %d\n",    _monitorNumber);
   printf("_displayInfo->dref:  %s\n",     ddca_dref_repr(_displayInfo->dref));
   fflush(stdout);
}


DDCA_Feature_List
Monitor::getFeatureList(DDCA_Feature_Subset_Id feature_list_id) {
    bool debugFunc = debugFeatureLists;
    // debugFunc = true;
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

bool Monitor::capabilities_check_complete() {
   // considered complete if invalid display
   bool result = (_displayInfo->dispno != -1);   // dispno -1 if API found display invalid
   if (result)
      result = (_baseModel->_caps_check_complete);
   return result;
}


bool Monitor::supportsDdc() {
   bool result = (_displayInfo->dispno != -1);   // dispno -1 if API found display invalid
   if (result)
      result = (_baseModel->_caps_status == 0 && _baseModel->_parsed_caps);  // got capabilities?
   return result;
}


void Monitor::putVcpRequest(VcpRequest * rqst) {
    // PRINTFCM("-> rqst->type=%d. Adding request to monitor's request queue", rqst->_type);
    _requestQueue->put(rqst);
}
