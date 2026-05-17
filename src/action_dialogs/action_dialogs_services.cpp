/** @file action_dialogs_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "action_dialogs/debug_actions_dialog.h"

#include "action_dialogs/action_dialogs_services.h"

void init_action_dialogs_services() {
   bool debug = true;
   DBGF(debug, "Starting");
   init_debug_actions_dialog();
   DBGF(debug, "Done");
}
