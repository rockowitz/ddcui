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


static const int Initial_Method_Table_Size = 200;
static const int Initial_Class_Table_Size  = 50;
static GPtrArray * method_name_table = NULL;
static GPtrArray * class_name_table  = NULL;


/** Adds a method name to the method name table.
 *
 *  @param  method_name  method name
 *
 *  @remark
 *  If the **method_name_table** does not already exist, it is created.
 *  Duplicate entries are not added.
 */
void rtti_method_name_table_add(const char * method_name) {
   if (!method_name_table) {
      method_name_table = g_ptr_array_new_full(Initial_Method_Table_Size, g_free);
      class_name_table  = g_ptr_array_new_full(Initial_Class_Table_Size,  g_free);
   }
   if (!gaux_ptr_array_find_with_equal_func(method_name_table, method_name, g_str_equal, NULL))
      g_ptr_array_add(method_name_table, g_strdup(method_name));

   const char * sep = strstr(method_name, "::");
   if (sep) {
      gchar * class_name = g_strndup(method_name, sep - method_name);
      if (!gaux_ptr_array_find_with_equal_func(class_name_table, class_name, g_str_equal, NULL))
         g_ptr_array_add(class_name_table, class_name);
      else
         g_free(class_name);
   }
}


/** Checks if a method name is in the method name table.
 *
 *  @param  method_name  method name, either simple or Class::method form
 *  @return **true** if found, **false** if not
 *
 *  @remark
 *  If method_name contains no "::", matches any entry whose method portion
 *  (after "::") equals method_name. If it contains "::", exact match only.
 */
bool rtti_method_name_table_contains(const char * method_name) {
   if (!method_name_table || !method_name)
      return false;
   if (strstr(method_name, "::"))
      return gaux_ptr_array_find_with_equal_func(method_name_table, method_name, g_str_equal, NULL);
   for (guint ndx = 0; ndx < method_name_table->len; ndx++) {
      const char * entry = (const char *) g_ptr_array_index(method_name_table, ndx);
      const char * sep = strstr(entry, "::");
      const char * entry_method = sep ? sep + 2 : entry;
      if (g_str_equal(entry_method, method_name))
         return true;
   }
   return false;
}


/** Checks if a class name is in the class name table.
 *
 *  @param  class_name  class name (unqualified, e.g. "MainWindow")
 *  @return **true** if found, **false** if not
 */
bool rtti_class_name_table_contains(const char * class_name) {
   if (!class_name_table || !class_name)
      return false;
   return gaux_ptr_array_find_with_equal_func(class_name_table, class_name, g_str_equal, NULL);
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


/** Reports the contents of the class name table.
 *
 *  @param  depth         logical indentation depth
 *  @param  show_internal if true, also show the table address
 */
void dbgrpt_rtti_class_name_table(int depth, bool show_internal) {
   if (show_internal) {
      rpt_vstring(depth, "Class name table at %p", class_name_table);
      depth++;
   }
   if (class_name_table) {
      g_ptr_array_sort(class_name_table, gaux_ptr_scomp);
      for (guint ndx = 0; ndx < class_name_table->len; ndx++)
         rpt_vstring(depth, "   %s", (char *) g_ptr_array_index(class_name_table, ndx));
   }
   else {
      if (!show_internal)
         rpt_label(depth, "None");
   }
}


/** Reports the class name table with an optional header message.
 *
 *  @param  depth  logical indentation depth
 *  @param  msg    optional header message, may be NULL
 */
void report_rtti_class_name_table(int depth, const char * msg) {
   if (msg) {
      rpt_label(depth, msg);
      depth++;
   }
   dbgrpt_rtti_class_name_table(depth, false);
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
   if (class_name_table) {
      g_ptr_array_free(class_name_table, true);
      class_name_table = NULL;
   }
}
