/** @file nc_values_state.cpp
 *  Maintains the current state of the NC Feature Values dialog, which
 *  controls which features are shown.
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>

#include "nc_values_state.h"


// For NcValues dialog
NcValuesSource parsedNcValuesSource_to_NcValuesSource(Parsed_NC_Values_Source val) {
   NcValuesSource ncvs = NcValuesFromBoth;      // put something
   // yes, the values in NcValuesSource and Parsed_NC_Values_Source match, but relying on that is brittle
   if (val != NC_VALUES_SOURCE_UNSET) {
      switch(val) {
      case NC_VALUES_SOURCE_MCCS:          ncvs = NcValuesFromMccs;          break;
      case NC_VALUES_SOURCE_CAPABILITIES:  ncvs = NcValuesFromCapabilities;  break;
      case NC_VALUES_SOURCE_BOTH:          ncvs = NcValuesFromBoth;          break;
      case NC_VALUES_SOURCE_UNSET:         assert(false);  // impossible case to avoid compiler warning
      };
   }
   else {
      ncvs = NcValuesFromCapabilities;   // default
   }
   return ncvs;
}


NcValuesState::NcValuesState() {}

NcValuesState::NcValuesState(Parsed_Ddcui_Cmd * parsed_cmd) {
   if (parsed_cmd->nc_values_source != NC_VALUES_SOURCE_UNSET)
      _ncValuesSource = parsedNcValuesSource_to_NcValuesSource(parsed_cmd->nc_values_source);
}


NcValuesState::NcValuesState(const NcValuesState &other) : QObject() {
   _ncValuesSource = other._ncValuesSource;
   // _useLatestNcValues = other._useLatestNcValues;
}


void NcValuesState::changeNcValuesSource(NcValuesSource mode, bool useLatestNcValues) {
   bool changed = (mode != _ncValuesSource || useLatestNcValues != _useLatestNcValues);
   _ncValuesSource = mode;
   // _useLatestNcValues = useLatestNcValues;
   if (changed)
      emit ncValuesSourceChanged(_ncValuesSource, _useLatestNcValues);

}



const char *
ncValuesSourceName(NcValuesSource source)
{
     char * result = NULL;

     switch(source) {
     case NcValuesFromMccs:         result = (char*) "NcValuesSourceFromMccs";   break;
     case NcValuesFromCapabilities: result = (char*) "NcValuesFromCapabilities"; break;
     case NcValuesFromBoth:         result = (char*) "NcValuesFromBoth";         break;
     }
     return result;
}


