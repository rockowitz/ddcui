/** \file parsed_cmd.h - parsed ddcui command line */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PARSED_CMD_H_
#define PARSED_CMD_H_

#include <ddcutil_types.h>

typedef enum {
   CMD_FLAG_DDCDATA             = 0x0001,
   CMD_FLAG_TIMESTAMP_TRACE     = 0x0008,  // prepend trace and debug msgs with elapsed time
   CMD_FLAG_REPORT_FREED_EXCP   = 0x0200,
   CMD_FLAG_THREAD_ID_TRACE     = 0x0800,
   CMD_FLAG_ENABLE_UDF        = 0x100000,
   CMD_FLAG_NOUSB             = 0x200000,
} Parsed_Cmd_Flags;

#define PARSED_CMD_MARKER  "PCMD"
typedef struct {
   char                marker[4];   // PARSED_CMD_MARKER
   DDCA_Stats_Type     stats_types;
   DDCA_Trace_Group    traced_groups;
   char **             traced_files;
   char **             traced_functions;
   int                 max_tries[3];
   Parsed_Cmd_Flags    flags;
   float               sleep_multiplier;
} Parsed_Cmd;

Parsed_Cmd *  new_parsed_cmd();
void          free_parsed_cmd(Parsed_Cmd * parsed_cmd);
void          dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd);   // debugging function

#endif /* PARSED_CMD_H_ */
