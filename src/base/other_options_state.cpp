/* other_options_source.cpp
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

#include "other_options_state.h"

char *
ncValuesSourceName(NcValuesSource source)
{
     char * result = NULL;

     switch(source) {
     case NcValuesFromMccs:         result = (char*) "NcValuesSourceFromMccs";   break;
     case NcValuesFromCapabilities: result = (char*) "NcValuesFromCapabilities"; break;
     case NcValuesFromBoth:         result = (char*) "NcValuesFromBoth";         break;
     case NcValuesSourceUnset:      result = (char*) "NcValuesSourceUnset";      break;
     }
     return result;
}
