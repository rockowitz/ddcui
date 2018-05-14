/* feature_selector.h */

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

#ifndef FEATURE_SELECTOR_H
#define FEATURE_SELECTOR_H

#include "ddcutil_types.h"

class FeatureSelector {
public:
   //  FeatureSelector();

    DDCA_Feature_Subset_Id _featureListId           = DDCA_SUBSET_COLOR;
    bool                   _showUnsupportedFeatures = false;
    bool                   _includeTableFeatures    = false;
    bool                   _respectCapabilities     = true;

    bool operator==(const FeatureSelector& sel2) const;
    bool operator!=(const FeatureSelector& sel2) const;

    void dbgrpt();
};

#endif // FEATURE_SELECTOR_H
