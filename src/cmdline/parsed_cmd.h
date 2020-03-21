/** \file parsed_cmd.h - parsed ddcui command line */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PARSED_CMD_H_
#define PARSED_CMD_H_

#include <ddcutil_types.h>

typedef enum {
   VIEW_UNSET = 0,     // unset or invalid
   VIEW_SUMMARY,
   VIEW_CAPABILITIES,
   VIEW_FEATURES
} Parsed_View;


/*
 typedef enum {
   NcValuesFromMccs,
   NcValuesFromCapabilities,
   NcValuesFromBoth,
} NcValuesSource;

 */


typedef enum {
   FS_UNSET = 0,
   FS_MCCS,
   FS_CAPABILITIES,
   FS_MANUFACTURER,
   FS_COLOR,
   FS_SCAN
} Parsed_Feature_Set;





typedef enum {
   NC_VALUES_SOURCE_UNSET = 0,
   NC_VALUES_SOURCE_MCCS,
   NC_VALUES_SOURCE_CAPABILITIES,
   NC_VALUES_SOURCE_BOTH
} Parsed_NC_Values_Source;



typedef enum {
   CMD_FLAG_NONE                   = 0x000000,
   CMD_FLAG_DDCDATA                = 0x000001,
   CMD_FLAG_TIMESTAMP_TRACE        = 0x000002,  // prepend trace and debug msgs with elapsed time
   CMD_FLAG_REPORT_FREED_EXCP      = 0x000004,
   CMD_FLAG_THREAD_ID_TRACE        = 0x000008,
   CMD_FLAG_ENABLE_UDF             = 0x000010,
   CMD_FLAG_NOUSB                  = 0x000020,
   CMD_FLAG_SHOW_STYLES            = 0x000040,
   CMD_FLAG_UI_REQUIRE_CONTROL_KEY = 0x000080,
   CMD_FLAG_NC_VALUES_ALL_IN_CAPABILITIES = 0x000100,
   CMD_FLAG_NC_VALUES_MUST_BE_IN_CAPABILITIES = 0x000200,
   CMD_FLAG_SHOW_UNSUPPORTED       = 0x000400,
} Parsed_Cmd_Flags;

typedef enum {
   TRIVAL_UNSET,
   TRIVAL_FALSE,
   TRIVAL_TRUE
} Optional_True_False;      // what's the term used for kernel configuration?

const char * trival_repr(Optional_True_False value);


#define PARSED_CMD_MARKER  "PCMD"
typedef struct {
   char                marker[4];   // PARSED_CMD_MARKER
   DDCA_Stats_Type     stats_types;
   DDCA_Trace_Group    traced_groups;
   char **             traced_files;
   char **             traced_functions;
   int                 max_tries[3];
   Parsed_Cmd_Flags    flags;
   Parsed_View         view;
   Parsed_Feature_Set  feature_set;
   Parsed_NC_Values_Source nc_values_source;
   float               sleep_multiplier;
   Optional_True_False nc_values_all_in_capabilities;
   Optional_True_False nc_values_must_be_in_capabilities;

} Parsed_Cmd;

const char * get_feature_set_table_entry_name(Parsed_Feature_Set value);

const char * get_view_table_entry_name(Parsed_View pv);

Parsed_View  find_view_table_value(const char * value);

const char * get_nc_values_source_table_entry_name(Parsed_NC_Values_Source src);
Parsed_NC_Values_Source  find_nc_values_source_table_value(const char * value);
Parsed_Feature_Set       find_feature_set_table_value( char * value);



Parsed_Cmd *  new_parsed_cmd();
void          free_parsed_cmd(Parsed_Cmd * parsed_cmd);
void          dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd);   // debugging function

#endif /* PARSED_CMD_H_ */
