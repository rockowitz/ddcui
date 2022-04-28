// ddca_utils.cpp

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ddcutil_types.h>

#include <QtCore/QString>

extern "C" {
#include "c_util/data_structures.h"
}
#include "ddca_utils.h"


// Parsed capabilities and feature values

Bit_Set_256
bs256_from_cfr(DDCA_Cap_Vcp * cfr) {
   Bit_Set_256 result = EMPTY_BIT_SET_256;
   for (int ndx = 0; ndx < cfr->value_ct; ndx++) {
      result = bs256_insert(result, cfr->values[ndx]);
    }
   return result;
}

Bit_Set_256
bs256_from_sl_values(DDCA_Feature_Value_Entry * sl_values) {
   Bit_Set_256 result = EMPTY_BIT_SET_256;
   if (sl_values) {
      DDCA_Feature_Value_Entry * cur = sl_values;
      while(cur->value_name) {
         result = bs256_insert(result, cur->value_code);
         cur++;
      }
   }
   return result;
}


#ifdef UNUSED
static bool
cap_vcp_contains(DDCA_Cap_Vcp* cfr, uint8_t feature_code) {
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

static DDCA_Feature_Value_Entry *
find_feature_value_entry(DDCA_Feature_Value_Entry * sl_values, uint8_t feature_code) {
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


static int
feature_value_entry_ct(DDCA_Feature_Value_Entry * sl_values) {
   int ct = 0;
   if (sl_values) {
      while( sl_values[ct].value_name )
         ct++;
   }
   return ct;
}
#endif

#ifdef OLD
static inline int
int_max(int v1, int v2) {
   return (( v1 > v2) ? v1 : v2);
}
#endif


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


// Error Reporting

// declaration for recursive call
static void
format_error_detail_aux(
      DDCA_Error_Detail * erec,
      QString             causesTitle,
      int                 indentation_per_depth,
      int                 cur_depth,
      QString&            accum);


static void
format_error_detail_aux(
      DDCA_Error_Detail * erec,
      QString             causesTitle,
      int                 indentation_per_depth,
      int                 cur_depth,
      QString&            accum)
{
   int indentct = indentation_per_depth * cur_depth;
   QString istring = "";
   for (int ndx = 0; ndx < indentct; ndx++) {
      istring.append(" ");
   }
   accum.append(istring);
   accum.append(erec->detail);
   accum.append("\n");
   if (erec->cause_ct > 0) {
      if (causesTitle.length() > 0) {
         accum.append(causesTitle);
         accum.append("\n");
      }
      for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
         format_error_detail_aux(
               erec->causes[ndx],
               causesTitle,
               indentation_per_depth,
               cur_depth+1,
               accum);
      }
   }
}


QString
ddcutil_format_error_detail(
      DDCA_Error_Detail * erec,
      QString             causesTitle,
      int                 indentation_per_depth)
{
   QString accum = QString("");
   format_error_detail_aux(erec, causesTitle, indentation_per_depth, 0, accum);
   return accum;
}


#ifdef OLD   // use api function ddca_dbgrpt_feature_metadata()
void
ddcui_dbgrpt_ddca_feature_metadata(
      DDCA_Feature_Metadata * meta)
{
   assert(meta);

   printf("DDCA_Feature_Metadata at %p:", meta);
   printf("   VCP code %02X: %s\n", meta->feature_code, meta->feature_name);

   // rpt_vstring(d1, "Version spec: %d.%d", meta->vspec.major, meta->vspec.minor);

   printf("   Description:  %s\n", meta->feature_desc);
   DDCA_Version_Feature_Flags  vflags = meta->feature_flags;
   printf("   feature_flags:   0x%04x\n", vflags);
   // rpt_vstring(d1, "Global_flags: 0x%02x",  info->global_flags);  // TODO: interpretation function

   if(meta->sl_values) {
      DDCA_Feature_Value_Entry * sl_values = meta->sl_values;
      printf("Simple NC values:\n");
      while (sl_values->value_name != NULL) {
         printf("      0x%02x: %s\n", sl_values->value_code, sl_values->value_name);
         sl_values++;
      }
   }
   else
      printf("Simple NC values; No table specified\n");
}
#endif

