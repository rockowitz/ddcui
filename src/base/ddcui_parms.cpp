// ddcui_parms.cpp

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ddcui_parms.h"

const bool debugSignals            = false;
const bool debugValueWidgetSignals = false;
const bool debugFeatureLists       = false;
const bool debugFeatureSelection   = false;
const bool debugLayout             = false;
const bool debugNcValues           = false;
const bool debugFeatureDimensions  = false;


#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

#include "ddcui_parms.h"

const QString ddcui_version = DDCUI_VERSION;   // in ddcui_parms.h
