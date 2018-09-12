// monitor_desc_actions.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MONITOR_DESC_ACTIONS_H_
#define MONITOR_DESC_ACTIONS_H_


#include "ddcutil_c_api.h"

#include "base/monitor.h"

namespace MonitorDescActions {

char *
capture_display_info_report(
      DDCA_Display_Info * dinfo);

DDCA_Status
capture_capabilities_report(
      Monitor *        monitor,
      DDCA_Display_Ref dref,
      char **          caps_report_loc);

}  // namespace MonitorDescActions

#endif /* MONITOR_DESC_ACTIONS_H_ */
