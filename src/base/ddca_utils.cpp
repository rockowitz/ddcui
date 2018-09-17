// ddca_utils.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <glib-2.0/glib.h>

#include <ddcutil_c_api.h>

#include "ddca_utils.h"


bool cap_vcp_contains(DDCA_Cap_Vcp* cfr, uint8_t feature_code) {
   // printf("(%s) cfr=%p, feature_code=0x%02x\n", __func__, cfr, feature_code); fflush(stdout);
   bool result = false;
   for (int ndx = 0; ndx < cfr->value_ct; ndx++) {
      if (cfr->values[ndx] == feature_code) {
         result = true;
         break;
      }
   }
   // printf("(%s) Returning %d\n", __func__, result);
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
      DDCA_Feature_Value_Table   mccsTable,
      Nc_Values_Merge_Mode       mergeMode)
{
   assert(cfr);
   assert(mccsTable);
   bool debug = false;
   if (debug) {
      printf("(%s) cfr=%p, mccsTable=%p, mergeMode=%d\n",
             __func__, cfr, mccsTable, mergeMode);
      printf("(%s) cfr->feature_code=0x%02x, cfr->value_ct=%d\n",
             __func__, cfr->feature_code, cfr->value_ct);
      fflush(stdout);
   }
   DDCA_Feature_Value_Table result = NULL;
   // int max_entries = (merge_mode == CapsOnly)
   //                        ? cfr->value_ct + 1
   //                        : int_max( cfr->value_ct, feature_value_entry_ct(mccs_table)) + 1;
   int maxEntries = 0;
   switch(mergeMode) {
   case CapsOnly:
      maxEntries = cfr->value_ct + 1;
      break;
   case CapsPlusMccs:
      maxEntries = int_max( cfr->value_ct, feature_value_entry_ct(mccsTable)) + 1;
      break;
   case MccsOnly:
      maxEntries =  feature_value_entry_ct(mccsTable) + 1;
      break;
   }
   result = (DDCA_Feature_Value_Table) calloc(maxEntries, sizeof(DDCA_Feature_Value_Entry));
   int resultCt = 0;
   for (int featureCode = 0; featureCode < 256; featureCode++) {
      // printf("(%s) feature_code=0x%02x\n", __func__, feature_code);
      bool emit = false;
      bool featureInCaps = cfr && cap_vcp_contains(cfr, featureCode);
      DDCA_Feature_Value_Entry * slEntry = NULL;
      switch(mergeMode) {
      case CapsOnly:
         if (featureInCaps) {
            slEntry = find_feature_value_entry(mccsTable, featureCode);
            emit = true;
         }
         break;
      case CapsPlusMccs:
         slEntry = find_feature_value_entry(mccsTable, featureCode);
         if (featureInCaps || slEntry)
            emit = true;
         break;
      case MccsOnly:
         slEntry = find_feature_value_entry(mccsTable, featureCode);
         if (slEntry)
            emit = true;
         break;
      }
      if (emit) {
         result[resultCt].value_code = featureCode;
         if (slEntry)
            result[resultCt].value_name = strdup(slEntry->value_name);
         else
            result[resultCt].value_name = g_strdup_printf("Value 0x%02x", featureCode);
         resultCt++;
      }
   }
   result[resultCt].value_code = 0x00;
   result[resultCt].value_name = NULL;

   // Don't assume the values are ordered.
   // Results in a O(n**2) algorithm, but the lists are short.

   if (debug) {
      printf("(%s) Returning: %p\n", __func__, result); fflush(stdout);
   }
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

