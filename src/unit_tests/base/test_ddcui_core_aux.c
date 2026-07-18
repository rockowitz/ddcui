/** @file test_ddcui_core_aux.c
 *  Unit test for base/ddcui_core_aux.c
 *
 *  Standalone test, no framework required.  Build and run from the
 *  project root (build_cmake supplies the generated config.h):
 *
 *     gcc -Isrc -Ibuild_cmake $(pkg-config --cflags ddcutil glib-2.0) \
 *         src/unit_tests/base/test_ddcui_core_aux.c \
 *         src/base/ddcui_core_aux.c \
 *         -o test_ddcui_core_aux
 *     ./test_ddcui_core_aux
 *
 *  Exit status 0 if all tests pass, 1 if any fail.
 */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>
#include <stdio.h>

#include <ddcutil_types.h>

#include "base/ddcui_core_aux.h"

static int test_ct   = 0;
static int failed_ct = 0;

static void check(bool ok, const char * desc) {
   test_ct++;
   if (ok) {
      printf("ok:     %s\n", desc);
   }
   else {
      failed_ct++;
      printf("FAILED: %s\n", desc);
   }
}


static void test_default_level() {
   check(ddcui_syslog_level == DDCA_SYSLOG_INFO,
         "initial ddcui_syslog_level is DDCA_SYSLOG_INFO");
}


// At the default level (INFO), messages at or below INFO are emitted,
// more verbose ones are not.
static void test_level_info() {
   ddcui_syslog_level = DDCA_SYSLOG_INFO;
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR),
         "level INFO: ERROR message emitted");
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_WARNING),
         "level INFO: WARNING message emitted");
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_NOTICE),
         "level INFO: NOTICE message emitted");
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_INFO),
         "level INFO: INFO message emitted (boundary equality)");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_VERBOSE),
         "level INFO: VERBOSE message suppressed");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_DEBUG),
         "level INFO: DEBUG message suppressed");
}


// At the most restrictive normal level (ERROR), only ERROR messages emit.
static void test_level_error() {
   ddcui_syslog_level = DDCA_SYSLOG_ERROR;
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR),
         "level ERROR: ERROR message emitted");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_WARNING),
         "level ERROR: WARNING message suppressed");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_DEBUG),
         "level ERROR: DEBUG message suppressed");
}


// At the most verbose level (DEBUG), everything emits.
static void test_level_debug() {
   ddcui_syslog_level = DDCA_SYSLOG_DEBUG;
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR),
         "level DEBUG: ERROR message emitted");
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_VERBOSE),
         "level DEBUG: VERBOSE message emitted");
   check( test_emit_ddcui_syslog(DDCA_SYSLOG_DEBUG),
         "level DEBUG: DEBUG message emitted (boundary equality)");
}


// Level NEVER (0) suppresses all real message severities (all > 0).
static void test_level_never() {
   ddcui_syslog_level = DDCA_SYSLOG_NEVER;
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR),
         "level NEVER: ERROR message suppressed");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_DEBUG),
         "level NEVER: DEBUG message suppressed");
}


// Level NOT_SET suppresses everything, even though its value (-1)
// is numerically less than all message levels.
static void test_level_not_set() {
   ddcui_syslog_level = DDCA_SYSLOG_NOT_SET;
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_ERROR),
         "level NOT_SET: ERROR message suppressed");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_NEVER),
         "level NOT_SET: NEVER message suppressed");
   check(!test_emit_ddcui_syslog(DDCA_SYSLOG_DEBUG),
         "level NOT_SET: DEBUG message suppressed");
}


int main() {
   Ddcui_Syslog_Level saved_level = ddcui_syslog_level;

   test_default_level();
   test_level_info();
   test_level_error();
   test_level_debug();
   test_level_never();
   test_level_not_set();

   ddcui_syslog_level = saved_level;

   printf("%d tests, %d failed\n", test_ct, failed_ct);
   return (failed_ct == 0) ? 0 : 1;
}
