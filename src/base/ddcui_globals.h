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
extern const bool debugNcValues;
extern const bool debugLayout             ;

bool enable_trace_show_time(bool onoff);
bool enable_trace_show_thread_id(bool onoff);

bool printftcmf(
      bool debug,
      const char * funcname,
      int lineno,
      const char * filename,
      const char * format, ...);


#ifdef OLD
#define PRINTFCM(__FMT__, ...) \
   do { \
     printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
     fflush(stdout); \
   } while(0)
#endif

#define PRINTFCM(_FMT, ...) \
do { \
   printftcmf(true, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)


#ifdef OLD
#define PRINTFCMF(__FLAG__, __FMT__, ...) \
   if (__FLAG__) { \
      printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
      fflush(stdout); \
   }
#endif

#define PRINTFCMF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)

#ifdef OLD
#define PRINTFTCM(__FMT__, ...) \
   do { \
      char thread_prefix[15] = ""; \
      pid_t tid = syscall(SYS_gettid); \
     snprintf(thread_prefix, 15, "[%5jd] ", (intmax_t) tid); \
     printf("%s(%s::%s) " __FMT__ "\n", thread_prefix,  _cls, __func__, ##__VA_ARGS__); \
     fflush(stdout); \
   } while(0)
// variant that shows QtThreadId as well as Linux thread id
// printf("(%p/%s %s::%s) " __FMT__ "\n",  QThread::currentThreadId(), thread_prefix, _cls, __func__, ##__VA_ARGS__); \

#endif

#define PRINTFTCM(_FMT, ...) \
   do { \
      printftcmf(true, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
   } while(0)

#ifdef OLD
#define PRINTFTCMF(__FLAG__, __FMT__, ...) \
   if (__FLAG__) { \
      bool dbgtrc_show_thread_id = true;\
      char thread_prefix[15] = ""; \
      if (dbgtrc_show_thread_id) { \
         pid_t tid = syscall(SYS_gettid); \
         snprintf(thread_prefix, 15, "[%5jd] ", (intmax_t) tid); \
      } \
      printf("%s(%s::%s) " __FMT__ "\n",  thread_prefix, _cls, __func__, ##__VA_ARGS__); \
      fflush(stdout); \
   }
#endif

#define PRINTFTCMF(_FLAG, _FMT, ...) \
do { \
   printftcmf(_FLAG, __func__, __LINE__, __FILE__, _FMT, ##__VA_ARGS__); \
} while(0)



inline const char * sbool(bool val) { return (val) ? "true" : "false"; }

// #define SBOOL(__v) ( (__v) ? "true" : "false")

// #define APPLY_CANCEL
// #define ALT_MOCK_FEATURES

#ifdef APPLY_CANCEL
const bool useApplyCancel    = true;
#endif



inline const char * qs2s(QString qstr) {return qstr.toLatin1().data(); }

#define QS2S(_qstr) ( _qstr.toLatin1().data() )

#endif // DDCUI_GLOBALS_H
