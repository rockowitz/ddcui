/* monitor_desc_actions.cpp
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

#include <stdlib.h>

#include "ddcutil_c_api.h"

#include "base/monitor.h"
#include "nongui/feature_base_model.h"

#include "monitor_desc_actions.h"

char *
capture_display_info_report(
      DDCA_Display_Info * dinfo)
{
    ddca_start_capture(DDCA_CAPTURE_NOOPTS);
    DDCA_Output_Level saved_ol = ddca_get_output_level();
    ddca_set_output_level(DDCA_OL_VERBOSE);
    ddca_dbgrpt_display_info(dinfo, 0);
    ddca_set_output_level(saved_ol);
    char * s = ddca_end_capture();
    return s;
}


DDCA_Status
capture_capabilities_report(
      Monitor *         monitor,
      DDCA_Display_Ref  dref,
      char **           caps_report_loc)
{
   *caps_report_loc = NULL;

   char * caps = NULL;
   char * caps_report = NULL;
   DDCA_Capabilities * parsed_caps;
   DDCA_Status rc = 0;

#ifdef OLD
   DDCA_Display_Handle dh = NULL;
   rc = ddca_open_display(dref,&dh);
   if (rc == 0) {
       rc = ddca_get_capabilities_string(dh, &caps);
   }
   if (rc == 0) {
       rc = ddca_parse_capabilities_string(caps, &parsed_caps);
   }
#endif

   // Hacky.  Need to handle async, wait if caps not yet fetched
   rc = monitor->_baseModel->_caps_status;
   if (rc == 0) {
      caps = monitor->_baseModel->_caps_string;
      parsed_caps = monitor->_baseModel->_parsed_caps;
   }



   if (rc == 0) {
       DDCA_Monitor_Model_Key mmid = ddca_monitor_model_key_from_dref(dref);
       // wrap in collector
       DDCA_Output_Level saved_ol = ddca_get_output_level();
       ddca_set_output_level(DDCA_OL_VERBOSE);
       ddca_start_capture(DDCA_CAPTURE_NOOPTS);
       ddca_report_parsed_capabilities(parsed_caps, &mmid, 0);
       caps_report = ddca_end_capture();
       ddca_set_output_level(saved_ol);
       *caps_report_loc = caps_report;
       ddca_free_parsed_capabilities(parsed_caps);
   }

   if (caps)
      free(caps);

#ifdef OLD
   if (dh) {
      rc = ddca_close_display(dh);
   }
#endif

   return rc;
  }
