/* monitor_desc_actions.h
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

#ifndef MONITOR_DESC_ACTIONS_H_
#define MONITOR_DESC_ACTIONS_H_

// class MonitorDescActions {
// };

#include "ddcutil_c_api.h"

char * capture_display_info_report(DDCA_Display_Info * dinfo);

DDCA_Status capture_capabilities_report(DDCA_Display_Ref dref, char ** caps_report_loc);

#endif /* MONITOR_DESC_ACTIONS_H_ */
