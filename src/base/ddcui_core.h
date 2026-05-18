/** @file ddcui_core.h */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_CORE_H
#define DDCUI_CORE_H

#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>

#include "config.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "ddcui_parms.h"

const QString ddcui_version = DDCUI_VSTRING;

// Qt

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc
const int FeatureWidgetType = QListWidgetItem::UserType+1;

extern const int FeatureRowHeight;
extern const int FeatureHeaderHeight;

extern const QFont FeatureValueTextFont;
extern const QFont FeatureValueMonoFont;
extern const QFont FeatureValueButtonFont;
extern const QFont FeatureValueNumberEntryFont;
extern const QFont FeatureValueComboBoxFont;
extern const QFont FeatureValueHeaderFont;
extern QFont MainMenuFont2;

void init_core();

//
// Tracing
//

bool     enable_trace_show_time(bool onoff);
bool     enable_trace_show_thread_id(bool onoff);
intmax_t get_thread_id();
intmax_t get_process_id();
void     create_timestamp(char* buf, int bufsz);

typedef uint16_t Printftcmf_Options;
#define TRACE_OPTIONS_NONE      0x00
//#define TRACE_OPTIONS_SYSLOG    0x01
// #define TRACE_OPTIONS_SEVERE    0x02
#define TRACE_OPTIONS_STARTING  0x08
#define TRACE_OPTIONS_DONE      0x10
#define TRACE_OPTIONS_NOPREFIX  0x20


bool     printftcmf(
            bool debug,
            Printftcmf_Options opts,
            const char * metaclass_classname,
            const char * cls,
            const char * funcname,
            int lineno,
            const char * filename,
            const char * format, ...);

// Naming conventions:
//   C    use _cls
//     F  flag controls whether shown
//  M     use metaObject->className()
//  MC

#define TRACEC( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_NONE, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEC_STARTING( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_STARTING, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEC_DONE( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_DONE, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACECF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_NONE, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACECF_STARTING(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_STARTING, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACECF_DONE(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_DONE, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEM( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_NONE, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEM_STARTING( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_STARTING, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEM_DONE( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_DONE, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_NONE, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMF_STARTING(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_STARTING, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMF_DONE(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_DONE, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMC( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_NONE, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMC_STARTING( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_STARTING, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMC_DONE( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_DONE, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMCF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_NONE, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMCF_STARTING(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_STARTING, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMCF_DONE(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_DONE, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEC_NOPREFIX( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_NOPREFIX, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACECF_NOPREFIX(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_NOPREFIX, nullptr, _cls, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEM_NOPREFIX( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_NOPREFIX, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMF_NOPREFIX(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_NOPREFIX, metaObject()->className(), nullptr,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMC_NOPREFIX( _FMT, ...) \
do { \
   printftcmf(true, TRACE_OPTIONS_NOPREFIX, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#define TRACEMCF_NOPREFIX(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, TRACE_OPTIONS_NOPREFIX, metaObject()->className(), _cls,  __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)


// Inline functions

// inline const char * sbool(bool val) { return (val) ? "true" : "false"; }
// wrong.  on function end the underlying QByteArray is free'd, so the
// returned pointer is invalid
// inline const char * qs2s(QString qstr) {return qstr.toLatin1().data(); }

// A macro alternative to sbool()
#define SBOOL(val) ( (val) ? "true" : "false" )
#define QS2S(qstr)  qstr.toLatin1().data()

#endif // DDCUI_CORE_H
