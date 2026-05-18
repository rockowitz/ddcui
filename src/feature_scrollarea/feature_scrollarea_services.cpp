/** @file feature_scrollarea_services.cpp */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "c_util/debug_util.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/features_scrollarea_contents.h"
#include "feature_scrollarea/features_scrollarea_view.h"

#include "feature_scrollarea/feature_scrollarea_services.h"

void init_feature_scrollarea_services() {
   bool debug = false;
   DBGF(debug, "Starting");
   init_feature_widget();
   init_features_scrollarea_contents();
   init_features_scrollarea_view();
   DBGF(debug, "Done");
}
