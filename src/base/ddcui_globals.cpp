// ddcui_globals.cpp

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <glib-2.0/glib.h>

#include <assert.h>
#include <glib-2.0/glib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ddcui_globals.h"


// from ddcutil glib_util.c

/** Handles the boilerplate of obtaining a thread specific fixed size buffer.
 *  The first call to this function in a thread with a given key address
 *  allocates the buffer.  Subsequent calls in the thread for the same key
 *  address return the same buffer.
 *
 *  \param  buf_key_ptr  address of a **GPrivate** used as the identifier
 *                       for the buffer
 *  \param  buffer_size  size of buffer to allocate
 *  \return pointer to thread specific buffer
 *
 *  \remark
 *  When the buffer is first allocated, byte 0 is set to '\0'
 */
gchar *
get_thread_fixed_buffer(
      GPrivate * buf_key_ptr,
      guint16    buffer_size)
{
   // printf("(%s) buf_key_ptr=%p, buffer_size=%d\n", __func__, buf_key_ptr, buffer_size);
   assert(buffer_size > 0);

   char * buf = (char *) g_private_get(buf_key_ptr);

   // GThread * this_thread = g_thread_self();
   // printf("(%s) this_thread=%p, buf=%p\n", __func__, this_thread, buf);

   if (!buf) {
      buf = (char *) g_new(char, buffer_size);
      buf[0] = '\0';     // (sort of) mark buffer as unused
      g_private_set(buf_key_ptr, buf);
   }

   // printf("(%s) Returning: %p\n", __func__, buf);
   return buf;
}



// ddcutil timestamp.c

uint64_t cur_realtime_nanosec() {
   // on Pi, __time_t resolves to long int
   struct timespec tvNow;
   clock_gettime(CLOCK_REALTIME, &tvNow);

   uint64_t result = tvNow.tv_sec * (uint64_t)(1000*1000*1000);
   result += tvNow.tv_nsec;
   // printf("(%s) result=%"PRIu64"\n", __func__, result);

   // printf("(%s) tv_sec=%ld, tv_nsec=%10ld, Returning: %"PRIu64"\n",
   //        __func__, tvNow.tv_sec, tvNow.tv_nsec, result);
return result;
}


static uint64_t initial_timestamp_nanos = 0;


/** Returns the elapsed time in nanoseconds since the start of
 *  program execution.
 *
 *  The first call to this function marks the start of program
 *  execution and returns 0.
 *
 *  @return nanoseconds since start of program execution
 */
uint64_t elapsed_time_nanosec() {
   // printf("(%s) initial_timestamp_nanos=%"PRIu64"\n", __func__, initial_timestamp_nanos);
   uint64_t cur_nanos = cur_realtime_nanosec();
   if (initial_timestamp_nanos == 0)
      initial_timestamp_nanos = cur_nanos;
   uint64_t result = cur_nanos - initial_timestamp_nanos;
   // printf("(%s) Returning: %"PRIu64"\n", __func__, result);
   return result;
}


/** Returns the elapsed time since start of program execution
 *  as a formatted, printable string.
 *
 *  The string is built in a thread specific private buffer.  The returned
 *  string is valid until the next call of this function in the same thread.
 *
 *  @return formatted elapsed time
 */
char * formatted_elapsed_time() {
   static GPrivate  formatted_elapsed_time_key = G_PRIVATE_INIT(g_free);
   char * elapsed_buf = get_thread_fixed_buffer(&formatted_elapsed_time_key, 40);

   uint64_t et_nanos = elapsed_time_nanosec();
   uint64_t isecs    = et_nanos/ (1000 * 1000 * 1000);
   uint64_t imillis  = et_nanos/ (1000 * 1000);
   // printf("(%s) et_nanos=%"PRIu64", isecs=%"PRIu64", imillis=%"PRIu64"\n", __func__,  et_nanos, isecs, imillis);
   snprintf(elapsed_buf, 40, "%3" PRIu64 ".%03" PRIu64 "", isecs, imillis - (isecs*1000) );

   // printf("(%s) |%s|\n", __func__, elapsed_buf);
   return elapsed_buf;
}



#ifdef REF
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

bool dbgtrc(
        DDCA_Trace_Group  trace_group,
        const char *      funcname,
        const int         lineno,
        const char *      filename,
        char *            format,
        ...)
{
   bool msg_emitted = false;
   if ( is_tracing(trace_group, filename, funcname) ) {
      va_list(args);
      va_start(args, format);
      char * buffer = g_strdup_vprintf(format, args);
      va_end(args);
      int    bufsz = strlen(buffer) + 1;
      char * buf2  = calloc(bufsz+60, sizeof(char));

      // *** TEMP ***
      ddcui_trace_show_time = true;
      ddcui_trace_show_thread_id = true;

      char  elapsed_prefix[15] = "";
      if (ddcui_trace_show_time)
         g_snprintf(elapsed_prefix, 15, "[%s]", formatted_elapsed_time());

      char thread_prefix[15] = "";
      if (ddcui_trace_show_thread_id) {
         pid_t tid = syscall(SYS_gettid);
         snprintf(thread_prefix, 15, "[%7jd]", (intmax_t) tid);  // is this proper format for pid_t
      }

      g_snprintf(buf2, bufsz+60, "%s%s(%-30s) %s\n",
                               thread_prefix,
                               elapsed_prefix,
                               funcname, buffer);

      f0puts(buf2, fout());    // no automatic terminating null
      fflush(fout());

      free(buffer);
      free(buf2);
      msg_emitted = true;
   }

   return msg_emitted;
}

#endif

static bool ddcui_trace_show_time;
static bool ddcui_trace_show_thread_id;

bool enable_trace_show_time(bool onoff) {
   // printf("(%s) Setting %s\n", __func__, sbool(onoff));
   bool old = ddcui_trace_show_time;
   ddcui_trace_show_time = onoff;
   return old;
}

bool enable_trace_show_thread_id(bool onoff) {
   // printf("(%s) Setting %s\n", __func__, sbool(onoff));
   bool old = ddcui_trace_show_thread_id;
   ddcui_trace_show_thread_id = onoff;
   return old;
}

bool printftcmf(
      bool debug,
      const char * metaclass_classname,
      const char * cls,     // _cls
      const char * funcname,
      int lineno,
      const char * filename,
      const char * format, ...)
{
   bool msg_emitted = false;
   if (debug) {

      //if ( is_tracing(trace_group, filename, funcname) ) {
         va_list args;
         va_start(args, format);
         char * buffer = g_strdup_vprintf(format, args);
         va_end(args);
         int    bufsz = strlen(buffer) + 1;
         char * buf2  = (char *) calloc(bufsz+60, sizeof(char));

         char funcbuf[160];
         if (metaclass_classname && cls) {
            if (strcmp(metaclass_classname, cls) == 0)
               g_snprintf(funcbuf, sizeof(funcbuf), "%s::%s", metaclass_classname, funcname);
            else
               g_snprintf(funcbuf, sizeof(funcbuf), "%s/%s::%s", metaclass_classname, cls, funcname);
         }
         else if (metaclass_classname && !cls)
            g_snprintf(funcbuf, sizeof(funcbuf), "%s::%s", metaclass_classname, funcname);
         else if (!metaclass_classname && cls)
            g_snprintf(funcbuf, sizeof(funcbuf), "%s::%s", cls, funcname);
         else
            strcpy(funcbuf, funcname);

         char  elapsed_prefix[15] = "";
         if (ddcui_trace_show_time)
            g_snprintf(elapsed_prefix, 15, "[%s]", formatted_elapsed_time());

         char thread_prefix[15] = "";
         if (ddcui_trace_show_thread_id) {
            pid_t tid = syscall(SYS_gettid);
            snprintf(thread_prefix, 15, "[%7jd]", (intmax_t) tid);  // is this proper format for pid_t
         }

         g_snprintf(buf2, bufsz+100, "%s%s(%-30s) %s\n",
                                  thread_prefix,
                                  elapsed_prefix,
                                  funcbuf,
                                  buffer);

         fputs(buf2, stdout);    // no automatic terminating null
         fflush(stdout);

         free(buffer);
         free(buf2);
         msg_emitted = true;
      //}


   }
   return msg_emitted;


}




