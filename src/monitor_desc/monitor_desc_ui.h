// monitor_desc_ui.h

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later/endcopyright>

#ifndef MONITOR_DESC_UI_H_
#define MONITOR_DESC_UI_H_

class QWidget;
class Monitor;
class QStackedWidget;

QWidget * initMonitorInfoWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget);    // central widget of GUI

QWidget * initCapabilitiesWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget);    // central widget of GUI

#endif /* MONITOR_DESC_UI_H_ */
