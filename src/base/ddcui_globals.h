// ddcui_globals.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_GLOBALS_H
#define DDCUI_GLOBALS_H

#include "config.h"

#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc

const int FeatureWidgetType = QListWidgetItem::UserType+1;

extern const bool debugSignals            ;
extern const bool debugValueWidgetSignals ;
extern const bool debugFeatureLists       ;
extern const bool debugFeatureSelection   ;
extern const bool debugNcValues;
extern const bool debugLayout             ;

#define PRINTFCM(__FMT__, ...) \
   do { \
     printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
     fflush(stdout); \
   } while(0)

#define PRINTFCMF(__FLAG__, __FMT__, ...) \
   if (__FLAG__) { \
      printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
      fflush(stdout); \
   }

inline const char * sbool(bool val) { return (val) ? "true" : "false"; }

#define SBOOL(__v) ( (__v) ? "true" : "false")

// #define APPLY_CANCEL
// #define ALT_MOCK_FEATURES

#ifdef APPLY_CANCEL
const bool useApplyCancel    = true;
#endif
#ifdef ALT_FEATURES
const bool enableAltFeatures = true;
#endif

#endif // DDCUI_GLOBALS_H
