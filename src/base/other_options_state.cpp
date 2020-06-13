// other_options_state.cpp

// Controls which features are shown, as specified by in the
// Options->Feature Selection dialog.

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>

#include "other_options_state.h"


// For Other-Options dialog
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


OtherOptionsState::OtherOptionsState() {}

OtherOptionsState::OtherOptionsState(Parsed_Cmd * parsed_cmd) {
   if (parsed_cmd->nc_values_source != NC_VALUES_SOURCE_UNSET)
      _ncValuesSource = parsedNcValuesSource_to_NcValuesSource(parsed_cmd->nc_values_source);
}


OtherOptionsState::OtherOptionsState(const OtherOptionsState &other) : QObject() {
   _ncValuesSource = other._ncValuesSource;
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


