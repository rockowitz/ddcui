/** @file main_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "main/callback_manager.h"
#include "base/monitor.h"
#include "main/msgbox_thread.h"
#include "main/mainwindow_ui.h"
#include "main/mainwindow.h"

#include "main/main_services.h"

void init_main_services() {
   bool debug = true;
   DBGF(debug, "Starting");
   init_callback_manager();
   init_monitor();
   init_msgbox_thread();
   init_mainwindow_ui();
   init_mainwindow();
   DBGF(debug, "Done");
}
