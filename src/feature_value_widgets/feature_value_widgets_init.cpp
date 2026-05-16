/** @file feature_value_widgets_init.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "feature_value_widgets/value_2button_widget.h"
#include "feature_value_widgets/value_base_widget.h"
#include "feature_value_widgets/value_bytes_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_ncplus_widget.h"
#include "feature_value_widgets/value_new_cont_widget.h"
#include "feature_value_widgets/value_reset_widget.h"
#include "feature_value_widgets/value_simple_cont_widget.h"
#include "feature_value_widgets/value_special_widget_x62.h"
#include "feature_value_widgets/value_stacked_widget.h"
#include "feature_value_widgets/value_std_widget.h"

#include "feature_value_widgets/feature_value_widgets_init.h"

void init_feature_value_widgets() {
   bool debug = true;
   DBGF(debug, "Starting");
   init_value_base_widget();
   init_value_2button_widget();
   init_value_bytes_widget();
   init_value_nc_widget();
   init_value_ncplus_widget();
   init_value_new_cont_widget();
   init_value_reset_widget();
   init_value_simple_cont_widget();
   init_value_special_widget_x62();
   init_value_stacked_widget();
   init_value_std_widget();
   DBGF(debug, "Done");
}
