// ddcui_globals.h

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_GLOBALS_H
#define DDCUI_GLOBALS_H

#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "config.h"

#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

#include "ddcui_parms.h"

const QString ddcui_version = DDCUI_VERSION;   // in ddcui_parms.h

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc

const int FeatureWidgetType = QListWidgetItem::UserType+1;

extern const bool debugSignals            ;
extern const bool debugValueWidgetSignals ;
extern const bool debugFeatureLists       ;
extern const bool debugFeatureSelection   ;
extern const bool debugNcValues           ;
extern const bool debugLayout             ;

bool enable_trace_show_time(bool onoff);
bool enable_trace_show_thread_id(bool onoff);

bool printftcmf(
      bool debug,
      const char * cls,
      const char * funcname,
      int lineno,
      const char * filename,
      const char * format, ...);

#define TRACE( _FMT, ...) \
do { \
   printftcmf(true, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACED( _FMT, ...) \
do { \
   printftcmf(true, metaObject()->className(), __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEDF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, metaObject()->className(), __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)



inline const char * sbool(bool val) { return (val) ? "true" : "false"; }
inline const char * qs2s(QString qstr) {return qstr.toLatin1().data(); }


// #define APPLY_CANCEL
// #define ALT_MOCK_FEATURES

#ifdef APPLY_CANCEL
const bool useApplyCancel    = true;
#endif


#endif // DDCUI_GLOBALS_H
