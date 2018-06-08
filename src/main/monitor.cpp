/* monitor.cpp */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#include "base/monitor.h"

#include <assert.h>
#include <string.h>

#include <QtCore/QVector>

#include <ddcutil_c_api.h>

#include "base/ddcui_globals.h"
#include "nongui/vcpthread.h"    // includes vcprequest.h
#include "nongui/feature_value.h"


Monitor::Monitor(DDCA_Display_Info * display_info, int monitorNumber)
    : _monitorNumber(monitorNumber)
    , _displayInfo(display_info)

    ,  _baseModel(NULL)
 //   ,  _cls(metaObject()->className()) // -Wreorder
    ,  _requestQueue(NULL)
// #ifdef ALT_FEATURES
    ,  _listModel(NULL)
    ,  _tableModel(NULL)
// #endif

{
   _cls = metaObject()->className();
   _page_moninfo     = _page_capabilities     = NULL;
   _pageno_moninfo   = _pageno_capabilities   = 0;
   _moninfoPlainText = _capabilitiesPlainText = NULL;
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
    // PRINTFCM("feature_list_id=%d-%s",feature_list_id, ddca_feature_list_id_name(feature_list_id));

    bool include_table_features = false;    // TODO get from feature selection dialog
    DDCA_Status ddcrc = 0;
    DDCA_Feature_List result;
    if (_features.contains(feature_list_id)) {
        result = _features.value(feature_list_id);
    }
    else {
        ddcrc = ddca_get_feature_list_by_dref(
                   feature_list_id, _displayInfo->dref, include_table_features, &result);
        if (ddcrc == 0)
           _features.insert(feature_list_id, result);
    }
   return result;
}


bool Monitor::supportsDdc() {
   bool result = (_displayInfo->dispno != -1);   // dispno -1 if API found display invalid
   if (result)
      result = (_baseModel->_caps_status == 0 && _baseModel->_parsed_caps);  // got capabilities?
   return result;
}

// #ifdef ALT_WIDGETS
void Monitor::setFeatureItemModel(FeatureItemModel * model) {
   _listModel = model;
}

void Monitor::setFeatureTableModel(FeatureTableModel * tableModel) {
    this->_tableModel = tableModel;
}
// #endif

void Monitor::putVcpRequest(VcpRequest * rqst) {
    // PRINTFCM("rqst->type=%d. Adding request to monitor's request queue", rqst->_type);
    _requestQueue->put(rqst);
}
