// feature_selector.cpp

// Controls which features are shown, as specified in the
// Options->Feature Selection dialog.

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdio.h>

#include <ddcutil_c_api.h>

#include "feature_selector.h"
#include "core.h"

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
