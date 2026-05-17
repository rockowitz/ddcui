/** @file ddcui_trace_control.c
 *
 *  Manage whether tracing is performed for ddcui Qt C++ methods and files.
 */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

/** \cond */
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>
/** \endcond */

#include "c_util/glib_util.h"
#include "c_util/report_util.h"

#include "base/ddcui_trace_control.h"


// traced_method_table and traced_file_table are implemented using GPtrArray.
// These data structures are used only for testing, and
// there will be at most a handful of entries.

static GPtrArray * traced_method_table = NULL;
static GPtrArray * traced_file_table   = NULL;


/** Adds a method to the list of methods to be traced.
 *
 *  @param  methodname  method name
 *
 *  @remark
 *  If the **traced_method_table** does not already exist, it is created.
 */
void add_traced_method(const char * methodname) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting. methodname=|%s|\n", __func__, methodname);

   if (!traced_method_table)
      traced_method_table = g_ptr_array_new();

   bool missing = !gaux_ptr_array_find_with_equal_func(
                        traced_method_table, methodname, g_str_equal, NULL);
   if (missing)
      g_ptr_array_add(traced_method_table, g_strdup(methodname));

   if (debug)
      printf("(%s) Done. methodname=|%s|, missing=%s\n",
             __func__, methodname, missing ? "true" : "false");
}


/** Checks if a method is being traced.
 *
 *  @param  methodname  method name
 *  @return **true** if the method is being traced, **false** if not
 */
bool is_trace_method(const char * methodname) {
   bool result = (traced_method_table &&
                  gaux_ptr_array_find_with_equal_func(
                        traced_method_table, methodname, g_str_equal, NULL));
   return result;
}


/** Reports the contents of the traced method table.
 *
 *  @param  depth  logical indentation depth
 */
void dbgrpt_traced_method_table(int depth) {
   if (traced_method_table) {
      rpt_vstring(depth, "traced_method_table:");
      if (traced_method_table->len == 0)
         rpt_vstring(depth+1, "(empty)");
      else {
         g_ptr_array_sort(traced_method_table, gaux_ptr_scomp);
         for (guint ndx = 0; ndx < traced_method_table->len; ndx++)
            rpt_vstring(depth+1, "%s",
                        (char *) g_ptr_array_index(traced_method_table, ndx));
      }
   }
   else {
      rpt_vstring(depth, "traced_method_table: NULL");
   }
}


/** Adds a file to the list of files to be traced.
 *
 *  @param  filename  file name
 *
 *  @remark
 *  If the **traced_file_table** does not already exist, it is created.
 */
void add_traced_file(const char * filename) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting. filename=|%s|\n", __func__, filename);

   if (!traced_file_table)
      traced_file_table = g_ptr_array_new();

   bool missing = !gaux_ptr_array_find_with_equal_func(
                        traced_file_table, filename, g_str_equal, NULL);
   if (missing)
      g_ptr_array_add(traced_file_table, g_strdup(filename));

   if (debug)
      printf("(%s) Done. filename=|%s|, missing=%s\n",
             __func__, filename, missing ? "true" : "false");
}


/** Checks if a file is being traced.
 *
 *  @param  filename  file name
 *  @return **true** if the file is being traced, **false** if not
 */
bool is_traced_file(const char * filename) {
   bool result = (traced_file_table &&
                  gaux_ptr_array_find_with_equal_func(
                        traced_file_table, filename, g_str_equal, NULL));
   return result;
}


/** Reports the contents of the traced file table.
 *
 *  @param  depth  logical indentation depth
 */
void dbgrpt_traced_file_table(int depth) {
   if (traced_file_table) {
      rpt_vstring(depth, "traced_file_table:");
      if (traced_file_table->len == 0)
         rpt_vstring(depth+1, "(empty)");
      else {
         g_ptr_array_sort(traced_file_table, gaux_ptr_scomp);
         for (guint ndx = 0; ndx < traced_file_table->len; ndx++)
            rpt_vstring(depth+1, "%s",
                        (char *) g_ptr_array_index(traced_file_table, ndx));
      }
   }
   else {
      rpt_vstring(depth, "traced_file_table: NULL");
   }
}
