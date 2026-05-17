/** @file ddcui_rtti.c
 *
 * Runtime trace information
 */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

/** \cond */
#include <glib-2.0/glib.h>
#include <stdbool.h>
/** \endcond */

#include "c_util/glib_util.h"
#include "c_util/report_util.h"

#include "base/ddcui_rtti.h"


static GPtrArray * method_name_table = NULL;


/** Adds a method name to the method name table.
 *
 *  @param  method_name  method name
 *
 *  @remark
 *  If the **method_name_table** does not already exist, it is created.
 *  Duplicate entries are not added.
 */
void rtti_method_name_table_add(const char * method_name) {
   if (!method_name_table)
      method_name_table = g_ptr_array_new_with_free_func(g_free);
   if (!gaux_ptr_array_find_with_equal_func(method_name_table, method_name, g_str_equal, NULL))
      g_ptr_array_add(method_name_table, g_strdup(method_name));
}


/** Checks if a method name is in the method name table.
 *
 *  @param  method_name  method name
 *  @return **true** if found, **false** if not
 */
bool rtti_method_name_table_contains(const char * method_name) {
   return (method_name_table &&
           gaux_ptr_array_find_with_equal_func(method_name_table, method_name, g_str_equal, NULL));
}


/** Reports the contents of the method name table.
 *
 *  @param  depth         logical indentation depth
 *  @param  show_internal if true, also show the table address
 */
void dbgrpt_rtti_method_name_table(int depth, bool show_internal) {
   if (show_internal) {
      rpt_vstring(depth, "Method name table at %p", method_name_table);
      depth++;
   }
   if (method_name_table) {
      g_ptr_array_sort(method_name_table, gaux_ptr_scomp);
      for (guint ndx = 0; ndx < method_name_table->len; ndx++)
         rpt_vstring(depth, "   %s", (char *) g_ptr_array_index(method_name_table, ndx));
   }
   else {
      if (!show_internal)
         rpt_label(depth, "None");
   }
}


/** Reports the method name table with an optional header message.
 *
 *  @param  depth  logical indentation depth
 *  @param  msg    optional header message, may be NULL
 */
void report_rtti_method_name_table(int depth, const char * msg) {
   if (msg) {
      rpt_label(depth, msg);
      depth++;
   }
   dbgrpt_rtti_method_name_table(depth, false);
}


/** Releases all resources used by the method name table. */
void terminate_rtti() {
   if (method_name_table) {
      g_ptr_array_free(method_name_table, true);
      method_name_table = NULL;
   }
}
