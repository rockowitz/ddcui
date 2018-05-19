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
    : _monitorNumber(monitorNumber),
      _displayInfo(display_info)
     ,  _cls(metaObject()->className())
{
    // _cls = metaObject()->className();
}


Monitor::~Monitor() {
}

void Monitor::dbgrpt() {
   printf("_monitorNumber:      %d\n",    _monitorNumber);
   printf("_displayInfo->dref:  %s\n",     ddca_dref_repr(_displayInfo->dref));
}


DDCA_Feature_List
Monitor::getFeatureList(DDCA_Feature_Subset_Id feature_list_id) {
    // printf("(%s::%s) feature_list_id=%d-%s\n",
    //       _cls, __func__, feature_list_id, ddca_feature_list_id_name(feature_list_id));

    bool include_table_features = false;    // TODO get from feature selection dialog
    DDCA_Status ddcrc = 0;
    DDCA_Feature_List result;
    if (_features.contains(feature_list_id)) {
        result = _features.value(feature_list_id);
    }
    else {
        // DDCA_MCCS_Version_Spec vspec = _displayInfo->vcp_version;
        // ddcrc = ddca_get_feature_list(feature_list_id, vspec, include_table_features, &result);
        ddcrc = ddca_get_feature_list_by_dref(feature_list_id, _displayInfo->dref, include_table_features, &result);
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

void Monitor::setFeatureItemModel(FeatureItemModel * model) {
   _listModel = model;
}

void Monitor::setFeatureTableModel(FeatureTableModel * tableModel) {
    this->_tableModel = tableModel;
}

void Monitor::putVcpRequest(VcpRequest * rqst) {
    // PRINTFCM("rqst->type=%d. Adding request to monitor's request queue", rqst->_type);
    _requestQueue->put(rqst);
}