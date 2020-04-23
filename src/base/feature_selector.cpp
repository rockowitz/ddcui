// feature_selector.cpp

// Controls which features are shown, as specified in the
// Options->Feature Selection dialog.

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>

#include <ddcutil_c_api.h>

#include "feature_selector.h"
#include "core.h"
#include "cmdline/parsed_cmd.h"



// For feature selection dialog
DDCA_Feature_Subset_Id parsedFeatureSet_to_ddcaFeatureSubsetId(Parsed_Feature_Set fs) {
   DDCA_Feature_Subset_Id  fsid = DDCA_SUBSET_UNSET;   // dummy value
         switch(fs) {
         case FS_UNSET:        fsid = DDCA_SUBSET_UNSET;        break;
         case FS_MCCS:         fsid = DDCA_SUBSET_KNOWN;        break;
         case FS_CAPABILITIES: fsid = DDCA_SUBSET_CAPABILITIES; break;
         case FS_MANUFACTURER: fsid = DDCA_SUBSET_MFG;          break;
         case FS_COLOR:        fsid = DDCA_SUBSET_COLOR;        break;
         case FS_SCAN:         fsid = DDCA_SUBSET_SCAN;         break;
         }
   return fsid;
}



void FeatureSelector::applyParsedOptions(Parsed_Cmd * parsed_cmd) {
   if (parsed_cmd->feature_set) {
       _featureListId =
             parsedFeatureSet_to_ddcaFeatureSubsetId(parsed_cmd->feature_set);
    }

    _showUnsupportedFeatures = parsed_cmd->flags & CMD_FLAG_SHOW_UNSUPPORTED;

    if (parsed_cmd->nc_values_must_be_in_capabilities == TRIVAL_TRUE)
       _includeOnlyCapabilities = true;
    else if (parsed_cmd->nc_values_must_be_in_capabilities == TRIVAL_FALSE)
      _includeOnlyCapabilities = false;

    if (parsed_cmd->nc_values_all_in_capabilities == TRIVAL_TRUE)
       _includeAllCapabilities = true;
    else if (parsed_cmd->nc_values_all_in_capabilities == TRIVAL_FALSE)
       _includeAllCapabilities = false;

   //  _feature_selector->_includeOnlyCapabilities = parsed_cmd->flags & CMD_FLAG_NC_VALUES_MUST_BE_IN_CAPABILITIES;
   // _feature_selector->_includeAllCapabilities  = parsed_cmd->flags & CMD_FLAG_NC_VALUES_ALL_IN_CAPABILITIES;

}


FeatureSelector::FeatureSelector() {
}

FeatureSelector::FeatureSelector(Parsed_Cmd * parsedCmd) {
   applyParsedOptions(parsedCmd);
}

FeatureSelector::FeatureSelector(const FeatureSelector &other) {
   _featureListId           = other._featureListId;
   _includeTableFeatures    = other._includeTableFeatures     ;
   _showUnsupportedFeatures = other._showUnsupportedFeatures ;
   _includeOnlyCapabilities = other._includeOnlyCapabilities ;
   _includeAllCapabilities  = other._includeAllCapabilities;
}


bool FeatureSelector::operator==(const FeatureSelector &other) const {
   return (_featureListId         == other._featureListId             &&
           _includeTableFeatures    == other._includeTableFeatures    &&
           _showUnsupportedFeatures == other._showUnsupportedFeatures &&
           _includeOnlyCapabilities == other._includeOnlyCapabilities &&
           _includeAllCapabilities  == other._includeAllCapabilities);
}

bool FeatureSelector::operator!=(const FeatureSelector &other) const {
   return !(*this == other);
}




void FeatureSelector::dbgrpt() {
   printf("   feature_list_id:          %d - %s\n", _featureListId, ddca_feature_list_id_name(_featureListId));
   printf("   includeTableFeatures:     %s\n", SBOOL(_includeTableFeatures));
   printf("   showUnsupportedFeatures:  %s\n", SBOOL(_showUnsupportedFeatures));
   printf("   includeOnlyCapabilities:  %s\n", SBOOL(_includeOnlyCapabilities));
   printf("   includeAllCapabilities:   %s\n", SBOOL(_includeAllCapabilities));
   fflush(stdout);
}
