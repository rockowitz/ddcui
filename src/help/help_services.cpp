/** @file help_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "help/help_dialog.h"

#include "help/help_services.h"

void init_help_services() {
   bool debug = false;
   DBGF(debug, "Starting");
   init_help_dialog();
   DBGF(debug, "Done");
}
