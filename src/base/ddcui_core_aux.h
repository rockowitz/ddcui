/** @file ddcui_core_aux.h */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_CORE_AUX_H_
#define DDCUI_CORE_AUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "ddcutil_types.h"

#ifdef OLD
typedef enum  {
   DDCUI_SYSLOG_DEBUG    =   0,
   DDCUI_SYSLOG_INFO     =   3,
   DDCUI_SYSLOG_WARNING  =   5,
   DDCUI_SYSLOG_ERROR    =   7,
   DDCUI_SYSLOG_NEVER     =   9,
   DDCUI_SYSLOG_NOT_SET  = 255,
} Ddcui_Syslog_Level;
#endif

typedef DDCA_Syslog_Level Ddcui_Syslog_Level;

extern Ddcui_Syslog_Level  ddcui_syslog_level;

Ddcui_Syslog_Level ddcui_syslog_level_name_to_value(const char * name);

bool test_emit_ddcui_syslog(Ddcui_Syslog_Level msg_level);

#ifdef __cplusplus
}  // "C"
#endif

#endif /* DDCUI_CORE_AUX_H_ */
