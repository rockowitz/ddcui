/** \file parsed_cmd.h - parsed ddcui command line */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PARSED_CMD_H_
#define PARSED_CMD_H_

#include <ddcutil_types.h>

// Used to distinguish between an explicit option set vs no option set,
// in which case the default value will be used
typedef enum {
   TRIVAL_UNSET,
   TRIVAL_FALSE,
   TRIVAL_TRUE
} Optional_True_False;      // what's the term used for kernel configuration?

const char * trival_repr(Optional_True_False value);


// View Menu

typedef enum {
   VIEW_UNSET = 0,     // unset or invalid
   VIEW_SUMMARY,
   VIEW_CAPABILITIES,
   VIEW_FEATURES
} Parsed_View;


// Options Menu

typedef enum {
   FS_UNSET = 0,
   FS_MCCS,
   FS_CAPABILITIES,
   FS_MANUFACTURER,
   FS_COLOR,
   FS_SCAN,
   FS_CUSTOM
} Parsed_Feature_Set;

typedef enum {
   NC_VALUES_SOURCE_UNSET = 0,
   NC_VALUES_SOURCE_MCCS,
   NC_VALUES_SOURCE_CAPABILITIES,
   NC_VALUES_SOURCE_BOTH
} Parsed_NC_Values_Source;

// General

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

   CMD_FLAG_SHOW_UNSUPPORTED       = 0x000100,
   CMD_FLAG_SHOW_ACTIVE_STYLE      = 0x000200,

   CMD_FLAG_TIMEOUT_I2C_IO         = 0x001000,
   CMD_FLAG_DEFER_SLEEPS           = 0x004000,
   CMD_FLAG_DSA                    = 0x008000,

   CMD_FLAG_F1                   = 0x01000000,
   CMD_FLAG_F2                   = 0x02000000,
   CMD_FLAG_F3                   = 0x04000000,
   CMD_FLAG_F4                   = 0x08000000,
   CMD_FLAG_F5                   = 0x10000000,
   CMD_FLAG_F6                   = 0x20000000,

} Parsed_Cmd_Flags;


#define PARSED_CMD_MARKER  "PCMD"
typedef struct {
   char                    marker[4];   // PARSED_CMD_MARKER
   DDCA_Stats_Type         stats_types;
   DDCA_Trace_Group        traced_groups;
   char **                 traced_files;
   char **                 traced_functions;
   int                     max_tries[3];
   Parsed_Cmd_Flags        flags;
   Parsed_View             view;
   Parsed_Feature_Set      feature_set;
   Parsed_NC_Values_Source nc_values_source;
   float                   sleep_multiplier;
   Optional_True_False     include_all_capabilities_features;
   Optional_True_False     include_only_capabilities_features;
   Optional_True_False     enable_sleep_suppression;
   char *                  model;
   int                     busno;
   DDCA_Feature_List       custom_feature_list;
} Parsed_Cmd;


// VNT Table Lookup

const char *             get_feature_set_table_symbol(Parsed_Feature_Set feature_set_id);

const char *             get_view_table_symbol(Parsed_View view_id);
Parsed_View              find_view_table_value(const char * value);

const char *             get_nc_values_source_table_symbol(Parsed_NC_Values_Source src_id);
Parsed_NC_Values_Source  find_nc_values_source_table_value(const char * value);
Parsed_Feature_Set       find_feature_set_table_value( char * value);


// Lifecycle and debug

Parsed_Cmd *  new_parsed_cmd();
void          free_parsed_cmd(Parsed_Cmd * parsed_cmd);
void          dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd);   // debugging function

#endif /* PARSED_CMD_H_ */
