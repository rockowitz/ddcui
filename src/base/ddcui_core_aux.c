/** @file ddcui_core_aux.c */

// Copyright (C) 2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>

#include <ddcutil_types.h>

#include "c_util/string_util.h"

#include "ddcui_core_aux.h"

Ddcui_Syslog_Level  ddcui_syslog_level = DDCA_SYSLOG_INFO;

/** Given a message severity level, test whether it should be
 *  written to the system log.
 *
 *  @param  msg_level  severity of message
 *  @return true if msg should be written to system log, false if not
 */
bool test_emit_ddcui_syslog(Ddcui_Syslog_Level msg_level) {
   bool result =  (ddcui_syslog_level != DDCA_SYSLOG_NOT_SET &&
         msg_level <= ddcui_syslog_level);
   return result;
}

Ddcui_Syslog_Level ddcui_syslog_level_name_to_value(const char * name) {
   Ddcui_Syslog_Level ddcui_syslog_level = DDCA_SYSLOG_NOT_SET;
        if (streq(name, "DEBUG"))   ddcui_syslog_level = DDCA_SYSLOG_DEBUG;
   else if (streq(name, "INFO"))    ddcui_syslog_level = DDCA_SYSLOG_INFO;
   else if (streq(name, "WARNING")) ddcui_syslog_level = DDCA_SYSLOG_WARNING;
   else if (streq(name, "ERROR"))   ddcui_syslog_level = DDCA_SYSLOG_ERROR;
   else if (streq(name, "NEVER"))   ddcui_syslog_level = DDCA_SYSLOG_NEVER;
   return ddcui_syslog_level;
}

#ifdef OUT
const char * syslog_level_id_name(DDCA_Syslog_Level level) {
   const char * name = NULL;
   switch (level) {
   case DDCA_SYSLOG_DEBUG:   name = "DDCA_SYSLOG_DEBUG";   break;
   case DDCA_SYSLOG_INFO:    name = "DDCA_SYSLOG_INFO";    break;
   case DDCA_SYSLOG_WARNING: name = "DDCA_SYSLOG_WARNING"; break;
   case DDCA_SYSLOG_ERROR:   name = "DDCA_SYSLOG_ERROR";   break;
   case DDCA_SYSLOG_NONE:    name = "DDCA_SYSLOG_NONE";    break;
   case DDCA_SYSLOG_NOT_SET: name = "DDCA_SYSLOG_NOT_SET"; break;
   }
   return name;
}
#endif
