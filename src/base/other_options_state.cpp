// other_options_state.cpp

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "other_options_state.h"

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
