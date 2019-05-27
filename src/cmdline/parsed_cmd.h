/** \file parsed_cmd.h - parsed ddcui command line */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PARSED_CMD_H_
#define PARSED_CMD_H_


typedef enum {
   CMD_FLAG_ENABLE_UDF        = 0x100000,
} Parsed_Cmd_Flags;

#define PARSED_CMD_MARKER  "PCMD"
typedef struct {
   char                        marker[4];   // PARSED_CMD_MARKER
   Parsed_Cmd_Flags            flags;
} Parsed_Cmd;

Parsed_Cmd *  new_parsed_cmd();
void          free_parsed_cmd(Parsed_Cmd * parsed_cmd);
// void       dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd, int depth);   // debugging function

#endif /* PARSED_CMD_H_ */
