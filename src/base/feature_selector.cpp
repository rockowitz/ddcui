// feature_selector.cpp

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>

#include <ddcutil_c_api.h>

#include "base/ddcui_globals.h"

#include "feature_selector.h"

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
   printf("   includeTableFeatures:     %s\n", sbool(_includeTableFeatures));
   printf("   showUnsupportedFeatures:  %s\n", sbool(_showUnsupportedFeatures));
   printf("   includeOnlyCapabilities:  %s\n", sbool(_includeOnlyCapabilities));
   printf("   inncludeAllCapabilities:  %s\n", sbool(_includeAllCapabilities));
   fflush(stdout);
}
