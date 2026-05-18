/** @file base_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "base/user_interface_options_state.h"

#include "base/base_services.h"

void init_base_services() {
   bool debug = false;
   DBGF(debug, "Starting");
   init_user_interface_options_state();
   DBGF(debug, "Done");
}
