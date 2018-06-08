/* ddca_utils.cpp
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <glib-2.0/glib.h>

#include <ddcutil_c_api.h>

#include "ddca_utils.h"


bool cap_vcp_contains(DDCA_Cap_Vcp* cfr, uint8_t feature_code) {
   bool result = false;
   for (int ndx = 0; ndx < cfr->value_ct; ndx++) {
      if (cfr->values[ndx] == feature_code) {
         result = true;
         break;
      }
   }
   return result;
}

DDCA_Feature_Value_Entry *
find_feature_value_entry(DDCA_Feature_Value_Table sl_values, uint8_t feature_code) {
   DDCA_Feature_Value_Entry * result = NULL;
   if (sl_values) {
      for (int ndx = 0; sl_values[ndx].value_name; ndx++) {
         if (sl_values[ndx].value_code == feature_code) {
            result = &sl_values[ndx];
            break;
         }
      }
   }

   return result;
}

int
feature_value_entry_ct(DDCA_Feature_Value_Table sl_values) {
   int ct = 0;
   if (sl_values) {
      while( sl_values[ct].value_name )
         ct++;
   }
   return ct;
}

inline int int_max(int v1, int v2) {
   return (( v1 > v2) ? v1 : v2);
}

DDCA_Feature_Value_Table
ddcutil_merge_feature_values(
      DDCA_Cap_Vcp *             cfr,
      DDCA_Feature_Value_Table   mccs_table,
      Nc_Values_Merge_Mode       merge_mode)
{
   DDCA_Feature_Value_Table result = NULL;
   // int max_entries = (merge_mode == CapsOnly)
   //                        ? cfr->value_ct + 1
   //                        : int_max( cfr->value_ct, feature_value_entry_ct(mccs_table)) + 1;
   int max_entries = 0;
   switch(merge_mode) {
   case CapsOnly:
      max_entries = cfr->value_ct + 1;
      break;
   case CapsPlusMccs:
      max_entries = int_max( cfr->value_ct, feature_value_entry_ct(mccs_table)) + 1;
      break;
   case MccsOnly:
      max_entries =  feature_value_entry_ct(mccs_table) + 1;
      break;
   }
   result = (DDCA_Feature_Value_Table) calloc(max_entries, sizeof(DDCA_Feature_Value_Entry));
   int result_ct = 0;
   for (int feature_code = 0; feature_code < 256; feature_code++) {
      bool emit = false;
      bool feature_in_caps = cap_vcp_contains(cfr, feature_code);
      DDCA_Feature_Value_Entry * sl_entry = NULL;
      switch(merge_mode) {
      case CapsOnly:
         if (feature_in_caps) {
            sl_entry = find_feature_value_entry(mccs_table, feature_code);
            emit = true;
         }
         break;
      case CapsPlusMccs:
         sl_entry = find_feature_value_entry(mccs_table, feature_code);
         if (feature_in_caps || sl_entry)
            emit = true;
         break;
      case MccsOnly:
         sl_entry = find_feature_value_entry(mccs_table, feature_code);
         if (sl_entry)
            emit = true;
         break;
      }
      if (emit) {
         result[result_ct].value_code = feature_code;
         if (sl_entry)
            result[result_ct].value_name = strdup(sl_entry->value_name);
         else
            result[result_ct].value_name = g_strdup_printf("Value 0x%02x", feature_code);
         result_ct++;
      }
   }
   result[result_ct].value_code = 0x00;
   result[result_ct].value_name = NULL;

   // Don't assume the values are ordered.
   // Results in a O(n**2) algorithm, but the lists are short.

   return result;
}

void
ddcutil_free_dynamic_feature_value_table(
      DDCA_Feature_Value_Table table)
{
   // TODO:
   // need a header on the table to indicate whether dynamic and ensure it's actually
   // a DDCA_Feature_Value_Table
   if (table) {
      DDCA_Feature_Value_Entry * cur = table;
      while (cur) {
         if (cur->value_name)
            free(cur->value_name);
         if (cur->value_code != 0 || cur->value_name)
            cur++;
         else
            cur = NULL;
      }
      free(table);
   }
}


DDCA_Cap_Vcp *
ddcutil_find_cap_vcp(DDCA_Capabilities * parsed_caps, uint8_t feature_code)
{
   DDCA_Cap_Vcp * result = NULL;
   if (parsed_caps) {
      for (int ndx = 0; ndx < parsed_caps->vcp_code_ct; ndx++) {
         DDCA_Cap_Vcp* cur = &parsed_caps->vcp_codes[ndx];
         if (cur->feature_code == feature_code) {
            result = cur;
            break;
         }
      }
   }

   return result;
}

#ifdef UNUSED
void free_ddca_feature_value_table(DDCA_Feature_Value_Table * table) {
   if (table) {
      for (int ndx = 0; table[ndx]->value_name; ndx++) {
         free(table[ndx]->value_name);
      }
      free(table);

   }
}
#endif

