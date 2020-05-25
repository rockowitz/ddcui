// feature_selector.cpp

// Controls which features are shown, as specified in the
// Options->Feature Selection dialog.

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>

#include <ddcutil_c_api.h>

#include "base/core.h"
#include "base/feature_selector.h"
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
         case FS_CUSTOM:       fsid = DDCA_SUBSET_CUSTOM;       break;
         }
   return fsid;
}


bool defaultIncludeOnlyCapabilities(DDCA_Feature_Subset_Id fsid) {
   bool result = false;
   if (fsid == DDCA_SUBSET_KNOWN        ||
       fsid == DDCA_SUBSET_CAPABILITIES ||
       fsid == DDCA_SUBSET_MFG          ||
       fsid == DDCA_SUBSET_COLOR)
      result = true;
   return result;
}


bool defaultIncludeAllCapabilities(DDCA_Feature_Subset_Id fsid) {
   bool result = false;
   if (fsid == DDCA_SUBSET_CAPABILITIES)
      result = true;
   return result;
}


void FeatureSelector::applyParsedOptions(Parsed_Cmd * parsed_cmd) {
   bool debug = false;

   if (ddca_feature_list_count(parsed_cmd->custom_feature_list) > 0) {
      // printf("(applyParsedOptions) feature list count > 0\n");
      _customFeatureList = parsed_cmd->custom_feature_list;
      // printf("(applyParsedOptions) setting _featureListId = DDCA_SUBSET_CUSTOM\n");
      _featureSubsetId = DDCA_SUBSET_CUSTOM;
   }

   else if (parsed_cmd->feature_set) {
      // printf("(applyParsedOptions) feature set passed\n");
       _featureSubsetId =
             parsedFeatureSet_to_ddcaFeatureSubsetId(parsed_cmd->feature_set);
    }
   else
      _featureSubsetId = DDCA_SUBSET_KNOWN;      // default

    _showUnsupportedFeatures = parsed_cmd->flags & CMD_FLAG_SHOW_UNSUPPORTED;

    if (parsed_cmd->include_only_capabilities_features == TRIVAL_TRUE)
       _includeOnlyCapabilities = true;
    else if (parsed_cmd->include_only_capabilities_features == TRIVAL_FALSE)
      _includeOnlyCapabilities = false;
    else {
      _includeOnlyCapabilities = defaultIncludeOnlyCapabilities(_featureSubsetId);
    }

    if (parsed_cmd->include_all_capabilities_features == TRIVAL_TRUE)
       _includeAllCapabilities = true;
    else if (parsed_cmd->include_all_capabilities_features == TRIVAL_FALSE)
      _includeAllCapabilities = false;
    else {
      _includeAllCapabilities = defaultIncludeAllCapabilities(_featureSubsetId);
    }

    if (debug) {
       TRACEC("Done.");
       dbgrpt();
    }
}


FeatureSelector::FeatureSelector() {
}

FeatureSelector::FeatureSelector(Parsed_Cmd * parsedCmd) {
   applyParsedOptions(parsedCmd);
}

FeatureSelector::FeatureSelector(const FeatureSelector &other) {
   _featureSubsetId           = other._featureSubsetId;
   _includeTableFeatures    = other._includeTableFeatures     ;
   _showUnsupportedFeatures = other._showUnsupportedFeatures ;
   _includeOnlyCapabilities = other._includeOnlyCapabilities ;
   _includeAllCapabilities  = other._includeAllCapabilities;
}


bool FeatureSelector::operator==(const FeatureSelector &other) const {
   return (_featureSubsetId         == other._featureSubsetId             &&
           _includeTableFeatures    == other._includeTableFeatures    &&
           _showUnsupportedFeatures == other._showUnsupportedFeatures &&
           _includeOnlyCapabilities == other._includeOnlyCapabilities &&
           _includeAllCapabilities  == other._includeAllCapabilities);
}


bool FeatureSelector::operator!=(const FeatureSelector &other) const {
   return !(*this == other);
}


void FeatureSelector::dbgrpt() {
   printf("   feature_list_id:          %d - %s\n", _featureSubsetId, ddca_feature_list_id_name(_featureSubsetId));
   printf("   custom features:          %s\n", ddca_feature_list_string(_customFeatureList, "", ", "));
   printf("   includeTableFeatures:     %s\n", SBOOL(_includeTableFeatures));
   printf("   showUnsupportedFeatures:  %s\n", SBOOL(_showUnsupportedFeatures));
   printf("   includeOnlyCapabilities:  %s\n", SBOOL(_includeOnlyCapabilities));
   printf("   includeAllCapabilities:   %s\n", SBOOL(_includeAllCapabilities));
   fflush(stdout);
}
