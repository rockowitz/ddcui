/** @file core_widgets_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "core_widgets/number_entry_widget.h"
#include "core_widgets/spin_slider.h"

#include "core_widgets/core_widgets_services.h"

void init_core_widgets_services() {
   bool debug = false;
   DBGF(debug, "Starting");
   init_number_entry_widget();
   init_spin_slider();
   DBGF(debug, "Done");
}
