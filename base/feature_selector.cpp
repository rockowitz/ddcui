/* feature_selector.cpp
 *
 * <copyright>
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

#include <stdio.h>

#include <ddcutil_c_api.h>

#include "feature_selector.h"

bool FeatureSelector::operator==(const FeatureSelector &other) const {
   return (feature_list_id         == other.feature_list_id      &&
           includeTableFeatures    == other.includeTableFeatures &&
           showUnsupportedFeatures == other.showUnsupportedFeatures);
}

bool FeatureSelector::operator!=(const FeatureSelector &other) const {
   return !(*this == other);
}

void FeatureSelector::dbgrpt() {
   printf("   feature_list_id:          %d - %s\n", feature_list_id, ddca_feature_list_id_name(feature_list_id));
   printf("   includeTableFeatures:     %d\n", includeTableFeatures);
   printf("   showUnsupportedFeatures:  %d\n", showUnsupportedFeatures);
   fflush(stdout);
}
