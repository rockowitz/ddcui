/** @file option_dialogs_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "option_dialogs/feature_selection_dialog.h"
#include "option_dialogs/nc_values_dialog.h"
#include "option_dialogs/user_interface_options_dialog.h"

#include "option_dialogs/option_dialogs_services.h"

void init_option_dialogs_services() {
   bool debug = false;
   DBGF(debug, "Starting");
   init_feature_selection_dialog();
   init_nc_values_dialog();
   init_user_interface_options_dialog();
   DBGF(debug, "Done");
}
