// core.h

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CORE_H
#define CORE_H

#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

#include "config.h"

#include "ddcui_parms.h"

const QString ddcui_version = DDCUI_VERSION;

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc
const int FeatureWidgetType = QListWidgetItem::UserType+1;

const int  FeatureRowHeight = 22;

// Tracing

bool enable_trace_show_time(bool onoff);
bool enable_trace_show_thread_id(bool onoff);

bool printftcmf(
      bool debug,
      const char * metaclass_classname,
      const char * cls,
      const char * funcname,
      int lineno,
      const char * filename,
      const char * format, ...);

#define TRACEC( _FMT, ...) \
do { \
   printftcmf(true, NULL, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACECF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, NULL, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEM( _FMT, ...) \
do { \
   printftcmf(true, metaObject()->className(), NULL,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, metaObject()->className(), NULL,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMC( _FMT, ...) \
do { \
   printftcmf(true, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMCF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)


// inline functions

inline const char * sbool(bool val) { return (val) ? "true" : "false"; }
inline const char * qs2s(QString qstr) {return qstr.toLatin1().data(); }


// contrik whether every widget has Accept/Cancel buttons

// #define APPLY_CANCEL
// #define ALT_MOCK_FEATURES

#ifdef APPLY_CANCEL
const bool useApplyCancel    = true;
#endif


#endif // CORE_H
