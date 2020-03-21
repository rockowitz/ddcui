/** \file parsed_cmd.c - parsed ddcui command line */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

/** \cond */
#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/** \endcond */

#include "c_util/data_structures.h"
#include "cmdline/parsed_cmd.h"


static inline char * SBOOL(int b) {
   return (b) ? "true" : "false";
}


const char * trival_repr(Optional_True_False value) {
   char * result = NULL;
   switch(value) {
   case TRIVAL_TRUE:    result = "true";   break;
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
   return parsed_cmd;
}



Value_Name_Title view_table[] = {
   VNT(VIEW_UNSET,         NULL),
   VNT(VIEW_SUMMARY,       "Summary"),
   VNT(VIEW_CAPABILITIES,  "Capabilities"),
   VNT(VIEW_FEATURES,      "Features"),
   VN_END
};

/*
 * typedef enum {
   FS_UNSET = 0,
   FS_MCCS,
   FS_CAPABILITIES,
   FS_MANUFACTURER,
   FS_COLOR,
   FS_SCAN
} Parsed_Feature_Set;


 *
 */

Value_Name_Title feature_set_table[] = {
      VNT(FS_UNSET,          NULL),
      VNT(FS_MCCS,          "MCCS"),
      VNT(FS_CAPABILITIES,  "Capabilities"),
      VNT(FS_MANUFACTURER,  "Manufacturer"),
      VNT(FS_COLOR,         "Color"),
      VNT(FS_SCAN,          "Scan"),
      VNT_END
};

Value_Name_Title nc_values_source_table[] = {
       VNT(  NC_VALUES_SOURCE_UNSET,        NULL),
       VNT(  NC_VALUES_SOURCE_MCCS,         "MCCS"),
       VNT(  NC_VALUES_SOURCE_CAPABILITIES, "Capabilities"),
       VNT(  NC_VALUES_SOURCE_BOTH,         "Both"),
       VNT_END
};




/*
 * typedef enum {
   FS_UNSET = 0,
   FS_MCCS,
   FS_CAPABILITIES,
   FS_MANUFACTURER,
   FS_COLOR,
   FS_SCAN
} Parsed_Feature_Set;

 *
 */





const char * get_feature_set_table_entry_name(Parsed_Feature_Set value) {
   return vnt_name(feature_set_table, value);
}


const char * get_view_table_entry_name(Parsed_View pv) {
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

const char * get_nc_values_source_table_entry_name(Parsed_NC_Values_Source src) {
   return vnt_name(nc_values_source_table, src);
}


Parsed_NC_Values_Source  find_nc_values_source_table_value(const char * value) {
   return vnt_find_id(
              nc_values_source_table,
              value,
              true,      // search by description, not symbolic name
              true,      // ignore case
              VIEW_UNSET);

}


Parsed_Feature_Set       find_feature_set_table_value(char * value) {
   return vnt_find_id(
              feature_set_table,
              value,
              true,      // search by description, not symbolic name
              true,      // ignore case
              FS_UNSET);
}




// Debugging function
void dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd) {
   printf("(%s) Parsed_Cmd at %p\n", __func__, parsed_cmd);
   if (parsed_cmd) {
      printf("   stats:                     0x%08x\n",   parsed_cmd->stats_types);
#ifdef UNIMPLEMENTED
      printf("   timestamp_trace:           %s\n",      SBOOL(parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE));
#endif
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
      printf("   max_retries:               %s\n", buf);
      printf("   report_freed_exceptions:   %s\n", SBOOL( parsed_cmd->flags & CMD_FLAG_REPORT_FREED_EXCP) );
      printf("   enable udf:                %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_ENABLE_UDF) );
      printf("   nousb                      %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_NOUSB) );
      printf("   display ddc errors:        %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_DDCDATA) );
      printf("   timestamp prefix:          %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE) );
      printf("   thread_id prefix:          %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_THREAD_ID_TRACE) );
      printf("   show styles:               %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_SHOW_STYLES) );

      printf("   include all capabilities:  %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_NC_VALUES_ALL_IN_CAPABILITIES) );
      printf("   ignore capabilities        %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_NC_VALUES_MUST_BE_IN_CAPABILITIES) );
      printf("   require control key:       %s\n", SBOOL(parsed_cmd->flags & CMD_FLAG_UI_REQUIRE_CONTROL_KEY) );
      printf("   initial view:              %s\n", get_view_table_entry_name(parsed_cmd->view) );
      printf("   default feature set:       %s\n", get_feature_set_table_entry_name(parsed_cmd->feature_set));

      printf("   sleep multiplier:         %9.1f\n", parsed_cmd->sleep_multiplier);
#ifdef REF
      Optional_True_False nc_values_all_in_capabilities;
      Optional_True_False nc_values_must_be_in_capabilities;
#endif
      printf("  nc_values_must_be_in_capabilities: %s\n", trival_repr(parsed_cmd->nc_values_must_be_in_capabilities));
      printf("  nc_values_all_in_capabilities: %s\n",     trival_repr(parsed_cmd->nc_values_all_in_capabilities));

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

#ifdef string_util
      ntsa_free(parsed_cmd->traced_files, true);
       ntsa_free(parsed_cmd->traced_functions, true);
#endif
      g_strfreev(parsed_cmd->traced_files);
      g_strfreev(parsed_cmd->traced_functions);


      free(parsed_cmd);
   }

   if (debug) {
      printf("(%s) Done\n", __func__); fflush(stdout);
   }
}
