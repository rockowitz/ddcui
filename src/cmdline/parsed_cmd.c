/** \file parsed_cmd.c - parsed ddcui command line */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


/** \cond */
#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/** \endcond */

#include "ddcutil_c_api.h"

#include "c_util/data_structures.h"
#include "cmdline/parsed_cmd.h"


// all uses of this local function are safe,
static inline char * SBOOL(int b) {
   return (b) ? "true" : "false";
}


const char * trival_repr(Optional_True_False value) {
   char * result = NULL;
   switch(value) {
   case TRIVAL_TRUE:    result = "true";    break;
   case TRIVAL_FALSE:   result = "false";   break;
   case TRIVAL_UNSET:   result = "not set"; break;
   }
   return result;
}


/** Allocates new Parsed_Cmd data structure, sets default values.
 *
 *  @return initialized #Parsed_Cmd
 */
Parsed_Cmd *  new_parsed_cmd() {
   Parsed_Cmd * parsed_cmd = (Parsed_Cmd*) calloc(1, sizeof(Parsed_Cmd));
   memcpy(parsed_cmd->marker, PARSED_CMD_MARKER, 4);
   // n. all flags are false, byte values 0, integers 0, pointers NULL because of calloc
   // parsed_cmd->flags |= CMD_FLAG_ENABLE_UDF;
   parsed_cmd->sleep_multiplier = 1.0;
   parsed_cmd->busno = -1;
   return parsed_cmd;
}


// VNT tables for enums

Value_Name_Title view_table[] = {
   VNT(VIEW_UNSET,         NULL),
   VNT(VIEW_SUMMARY,       "Summary"),
   VNT(VIEW_CAPABILITIES,  "Capabilities"),
   VNT(VIEW_FEATURES,      "Features"),
   VN_END
};

Value_Name_Title feature_set_table[] = {
      VNT(FS_UNSET,          NULL),
      VNT(FS_MCCS,          "MCCS"),
      VNT(FS_CAPABILITIES,  "Capabilities"),
      VNT(FS_MANUFACTURER,  "Manufacturer"),
      VNT(FS_COLOR,         "Color"),
      VNT(FS_SCAN,          "Scan"),
      VNT(FS_CUSTOM,        "Custom"),
      VNT_END
};

Value_Name_Title nc_values_source_table[] = {
       VNT(  NC_VALUES_SOURCE_UNSET,        NULL),
       VNT(  NC_VALUES_SOURCE_MCCS,         "MCCS"),
       VNT(  NC_VALUES_SOURCE_CAPABILITIES, "Capabilities"),
       VNT(  NC_VALUES_SOURCE_BOTH,         "Both"),
       VNT_END
};


// VNT access functions

const char * get_feature_set_table_symbol(Parsed_Feature_Set value) {
   return vnt_name(feature_set_table, value);
}

const char * get_view_table_symbol(Parsed_View pv) {
   return vnt_name(view_table, pv);
}

Parsed_View  find_view_table_value(const char * value) {
   return vnt_find_id(
              view_table,
              value,
              true,      // search by description, not symbolic name
              true,      // ignore case
              VIEW_UNSET);
}

const char * get_nc_values_source_table_symbol(Parsed_NC_Values_Source src) {
   return vnt_name(nc_values_source_table, src);
}

Parsed_NC_Values_Source  find_nc_values_source_table_value(const char * value) {
   return vnt_find_id(
              nc_values_source_table,
              value,
              true,      // search by description, not symbolic name
              true,      // ignore case
              NC_VALUES_SOURCE_UNSET);
}

Parsed_Feature_Set       find_feature_set_table_value(char * value) {
   return vnt_find_id(
              feature_set_table,
              value,
              true,      // search by description, not symbolic name
              true,      // ignore case
              FS_UNSET);
}


/** Debugging report of #Parsed_Cmd struct
 *
 * \param parsed_cmd  instance to report
 */
void dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd) {
   printf("(%s) Parsed_Cmd at %p\n", __func__, (void *) parsed_cmd);
   if (parsed_cmd) {
      printf("   stats:                     0x%08x\n",  parsed_cmd->stats_types);
      printf("   traced_groups              0x%08x\n",  parsed_cmd->traced_groups);
      if (parsed_cmd->traced_functions) {
         char * joined = g_strjoinv(", ", parsed_cmd->traced_functions);
         printf("   traced_functions:          %s\n", joined);
         free(joined);
      }
      else
         printf("   traced_functions:          none\n" );
      if (parsed_cmd->traced_files) {
         char * joined = g_strjoinv(", ", parsed_cmd->traced_files);
         printf("   traced_files:                %s\n", joined);
         free(joined);
      }
      else
         printf("   traced_files:              none\n" );

      char buf[20];
      snprintf(buf,20, "%d,%d,%d", parsed_cmd->max_tries[0], parsed_cmd->max_tries[1], parsed_cmd->max_tries[2] );
      printf("   max_retries:               %s\n",   buf);
      printf("   report_freed_exceptions:   %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_REPORT_FREED_EXCP) );
      printf("   enable udf:                %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_ENABLE_UDF) );
      printf("   nousb                      %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_NOUSB) );
      printf("   display ddc errors:        %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_DDCDATA) );
      printf("   timestamp prefix:          %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE) );
      printf("   thread_id prefix:          %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_THREAD_ID_TRACE) );
      printf("   show styles:               %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_SHOW_STYLES) );
      printf("   show active style:         %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_SHOW_ACTIVE_STYLE) );
      printf("   sleep multiplier:         %9.1f\n", parsed_cmd->sleep_multiplier);

      printf("   require control key:       %s\n",   SBOOL(parsed_cmd->flags & CMD_FLAG_UI_REQUIRE_CONTROL_KEY) );
      printf("   initial view:              %s\n",   get_view_table_symbol(parsed_cmd->view) );
      printf("   default feature set:       %s\n",   get_feature_set_table_symbol(parsed_cmd->feature_set));
      printf("   custom feature set:        %s\n",   ddca_feature_list_string(parsed_cmd->custom_feature_list, "",","));
      printf("   NC values source:          %s\n",   get_nc_values_source_table_symbol(parsed_cmd->nc_values_source));
      printf("   include all  capabilities features: %s\n", trival_repr(parsed_cmd->include_all_capabilities_features));
      printf("   include_only capabilities features: %s\n", trival_repr(parsed_cmd->include_only_capabilities_features));
      printf("   initial model:             %s\n",   parsed_cmd->model);
      printf("   initial bus number:        %d\n",   parsed_cmd->busno);
      printf("   enable sleep suppression   %s\n",    trival_repr(parsed_cmd->enable_sleep_suppression) );
   }
}


/** Frees a #Parsed_Cmd instance
 *
 *  @param parsed_cmd  instance to free
 */
void free_parsed_cmd(Parsed_Cmd * parsed_cmd) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting.  parsed_cmd=%p\n", __func__, (void*) parsed_cmd);

   if (parsed_cmd) {
      assert ( memcmp(parsed_cmd->marker,PARSED_CMD_MARKER,4) == 0);
      parsed_cmd->marker[3] = 'x';

      g_strfreev(parsed_cmd->traced_files);       // used instead of ntsa_free()
      g_strfreev(parsed_cmd->traced_functions);

      free(parsed_cmd);
   }

   if (debug) {
      printf("(%s) Done\n", __func__); fflush(stdout);
   }
}
