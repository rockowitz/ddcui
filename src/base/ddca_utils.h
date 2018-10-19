// ddca_utils.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCA_UTILS_H_
#define DDCA_UTILS_H_

#include <ddcutil_types.h>

typedef enum {
   CapsOnly,
   CapsPlusMccs,
   MccsOnly
} Nc_Values_Merge_Mode;

// #define LOCAL_FEATURE_VALUE_TABLE_MARKER0 "LFVT"
static const char * LOCAL_FEATURE_VALUE_TABLE_MARKER = "LFVT";
typedef struct {
   char                       marker[4];
   unsigned char              flags;       // for future use, no flags yet defined
   DDCA_Feature_Value_Entry   values[0];
} Local_Feature_Value_Table;

Local_Feature_Value_Table *
ddcutil_merge_feature_values(
      DDCA_Cap_Vcp *             capabilities_feature_record,
      DDCA_Feature_Value_Entry * mccs_table,
      Nc_Values_Merge_Mode       merge_mode);

void
ddcutil_free_local_feature_value_table(
      Local_Feature_Value_Table *  table);

DDCA_Cap_Vcp *
ddcutil_find_cap_vcp(
      DDCA_Capabilities *        parsed_caps,
      uint8_t                    feature_code);

#ifdef UNUSED
void
free_ddca_feature_value_table(
      DDCA_Feature_Value_Table * table);
#endif

#endif /* DDCA_UTILS_H_ */
