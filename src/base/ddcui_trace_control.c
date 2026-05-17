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
#include <string.h>
/** \endcond */

#include "c_util/glib_util.h"
#include "c_util/report_util.h"
#include "c_util/string_util.h"

#include "base/ddcui_trace_control.h"

typedef struct {
   char * class_name;
   char * method_name;
} Traced_Method_Entry;

static void parse_method_name(
      const char *  methodname,
      char **       class_name_loc,
      char **       method_name_loc)
{
   const char * sep = strstr(methodname, "::");
   if (sep) {
      *class_name_loc  = g_strndup(methodname, sep - methodname);
      *method_name_loc = g_strdup(sep + 2);
   }
   else {
      *class_name_loc  = NULL;
      *method_name_loc = g_strdup(methodname);
   }
}

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

   char * class_name;
   char * method_name;
   parse_method_name(methodname, &class_name, &method_name);

   bool missing = true;
   for (guint ndx = 0; ndx < traced_method_table->len && missing; ndx++) {
      Traced_Method_Entry * entry = g_ptr_array_index(traced_method_table, ndx);
      bool class_match = (entry->class_name == NULL && class_name == NULL) ||
                         (entry->class_name != NULL && class_name != NULL &&
                          streq(entry->class_name, class_name));
      if (class_match && streq(entry->method_name, method_name))
         missing = false;
   }

   if (missing) {
      Traced_Method_Entry * entry = calloc(1, sizeof(Traced_Method_Entry));
      entry->class_name  = class_name;
      entry->method_name = method_name;
      g_ptr_array_add(traced_method_table, entry);
   }
   else {
      free(class_name);
      free(method_name);
   }

   if (debug)
      printf("(%s) Done. methodname=|%s|, missing=%s\n",
             __func__, methodname, missing ? "true" : "false");
}


/** Checks if a method is being traced.
 *
 *  @param  methodname  method name
 *  @return **true** if the method is being traced, **false** if not
 */
bool is_traced_method(const char * methodname) {
   bool result = false;
   if (traced_method_table && methodname) {
      char * class_name;
      char * method_name;
      parse_method_name(methodname, &class_name, &method_name);

      for (guint ndx = 0; ndx < traced_method_table->len && !result; ndx++) {
         Traced_Method_Entry * entry = g_ptr_array_index(traced_method_table, ndx);
         if (streq(entry->method_name, method_name)) {
            if (entry->class_name == NULL || class_name == NULL ||
                streq(entry->class_name, class_name))
               result = true;
         }
      }
      free(class_name);
      free(method_name);
   }
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
         for (guint ndx = 0; ndx < traced_method_table->len; ndx++) {
            Traced_Method_Entry * entry = g_ptr_array_index(traced_method_table, ndx);
            if (entry->class_name)
               rpt_vstring(depth+1, "%s::%s", entry->class_name, entry->method_name);
            else
               rpt_vstring(depth+1, "%s", entry->method_name);
         }
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
 *  The canonical form stored is the basename without any .c or .cpp extension.
 *  @remark
 *  If the **traced_file_table** does not already exist, it is created.
 */
void add_traced_file(const char * filename) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting. filename=|%s|\n", __func__, filename);

   if (!traced_file_table)
      traced_file_table = g_ptr_array_new();

   if (filename) {
      gchar * bname = g_path_get_basename(filename);
      if (str_ends_with(bname, ".cpp"))
         bname[strlen(bname) - 4] = '\0';
      else if (str_ends_with(bname, ".c"))
         bname[strlen(bname) - 2] = '\0';

      bool missing = !gaux_ptr_array_find_with_equal_func(
                           traced_file_table, bname, g_str_equal, NULL);
      if (missing)
         g_ptr_array_add(traced_file_table, bname);
      else
         free(bname);
   }

   if (debug)
      printf("(%s) Done. filename=|%s|\n", __func__, filename);
}


/** Checks if a file is being traced.
 *
 *  @param  filename  file name
 *  @return **true** if the file is being traced, **false** if not
 */
bool is_traced_file(const char * filename) {
   bool result = false;
   if (filename) {
      gchar * bname = g_path_get_basename(filename);
      if (str_ends_with(bname, ".cpp"))
         bname[strlen(bname) - 4] = '\0';
      else if (str_ends_with(bname, ".c"))
         bname[strlen(bname) - 2] = '\0';
      result = (traced_file_table &&
                gaux_ptr_array_find_with_equal_func(
                      traced_file_table, bname, g_str_equal, NULL));
      free(bname);
   }
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
