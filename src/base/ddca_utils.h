// ddca_utils.h
//
// Wrap ddcutil API functionality

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCA_UTILS_H_
#define DDCA_UTILS_H_

#include <ddcutil_types.h>
#include <QtCore/QString>

#include "c_util/data_structures.h"

Bit_Set_256 bs256_from_cfr(DDCA_Cap_Vcp * cfr);
Bit_Set_256 bs256_from_sl_values(DDCA_Feature_Value_Entry * sl_values);

// NC Feature Values


#ifdef UNUSED
// #define LOCAL_FEATURE_VALUE_TABLE_MARKER0 "LFVT"
extern  const char * LOCAL_FEATURE_VALUE_TABLE_MARKER;
typedef struct {
   char                       marker[4];
   unsigned char              flags;       // for future use, no flags yet defined
   DDCA_Feature_Value_Entry   values[0];
} Local_Feature_Value_Table;
#endif

#ifdef UNUSED
void
ddcutil_free_local_feature_value_table(
      Local_Feature_Value_Table *  table);

void
ddcutil_dbgrpt_local_feature_value_table(
      Local_Feature_Value_Table * table);
#endif

// Capabilities

DDCA_Cap_Vcp *
ddcutil_find_cap_vcp(
      DDCA_Capabilities *        parsed_caps,
      uint8_t                    feature_code);

#ifdef UNUSED
void
free_ddca_feature_value_table(
      DDCA_Feature_Value_Table * table);
#endif


// Error Reporting

QString
ddcutil_format_error_detail(
      DDCA_Error_Detail * erec,
      QString             causesTitle,
      int                 indentation_per_depth);

#endif /* DDCA_UTILS_H_ */
