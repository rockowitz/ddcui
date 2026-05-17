/** @file nongui_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "nongui/msgbox_queue.h"
#include "nongui/feature_base_model.h"
#include "nongui/vcpthread.h"

#include "nongui/nongui_services.h"

void init_nongui_services() {
   bool debug = true;
   DBGF(debug, "Starting");
   init_msgbox_queue();
   init_feature_base_model();
   init_vcpthread();
   DBGF(debug, "Done");
}
