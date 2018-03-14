/* monitor.cpp */

#include <assert.h>
#include <string.h>

#include <QtCore/QVector>

#include "feature_value.h"
#include "monitor.h"


Monitor::Monitor(DDCA_Display_Info * display_info)
{
    _displayInfo = display_info;
}


Monitor::~Monitor() {
}


DDCA_Feature_List
Monitor::getFeatureList(DDCA_Feature_Subset_Id feature_list_id) {
    bool include_table_features = false;    // TODO get from feature selection dialog
    DDCA_Feature_List result;
    if (_features.contains(feature_list_id)) {
        result = _features.value(feature_list_id);
    }
    else {
        DDCA_MCCS_Version_Spec vspec = _displayInfo->vcp_version;
        result = ddca_get_feature_list(feature_list_id, vspec, include_table_features);
        _features.insert(feature_list_id, result);
    }
   return result;
}


void Monitor::setFeatureItemModel(FeatureItemModel * model) {
    this->_listModel = model;
}


void Monitor::setFeatureTableModel(FeatureTableModel * tableModel) {
    this->_tableModel = tableModel;
}

void Monitor::putVcpRequest(VcpRequest * rqst) {
    printf("(Monitor::putVcpRequest)\n");  fflush(stdout);
    _requestQueue->put(rqst);
}
