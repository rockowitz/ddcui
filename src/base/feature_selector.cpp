// feature_selector.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>

#include <ddcutil_c_api.h>

#include "feature_selector.h"

bool FeatureSelector::operator==(const FeatureSelector &other) const {
   return (_featureListId         == other._featureListId      &&
           _includeTableFeatures    == other._includeTableFeatures &&
           _showUnsupportedFeatures == other._showUnsupportedFeatures &&
           _respectCapabilities     == other._respectCapabilities);
}

bool FeatureSelector::operator!=(const FeatureSelector &other) const {
   return !(*this == other);
}

void FeatureSelector::dbgrpt() {
   printf("   feature_list_id:          %d - %s\n", _featureListId, ddca_feature_list_id_name(_featureListId));
   printf("   includeTableFeatures:     %d\n", _includeTableFeatures);
   printf("   showUnsupportedFeatures:  %d\n", _showUnsupportedFeatures);
   printf("   respectCapabilities:      %d\n", _respectCapabilities);
   fflush(stdout);
}
