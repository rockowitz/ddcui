// monitor_desc_ui.h

// Copyright (C) 2018-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later/endcopyright>

#ifndef MONITOR_DESC_UI_H_
#define MONITOR_DESC_UI_H_

#include "base/monitor.h"
// class Monitor;

class QWidget;
class QStackedWidget;

QWidget * initMonitorInfoWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget);    // central widget of GUI

QWidget * initCapabilitiesWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget);    // central widget of GUI

#endif /* MONITOR_DESC_UI_H_ */
