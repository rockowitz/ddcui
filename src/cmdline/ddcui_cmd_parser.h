/* @file ddcui_cmd_parser.h */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_CMD_PARSER_H_
#define DDCUI_CMD_PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "base/ddcui_core_aux.h"
#include "ddcui_parsed_cmd.h"

bool parse_ddcui_syslog_level(const char * level_name, Ddcui_Syslog_Level * result_loc);

Parsed_Ddcui_Cmd * parse_ddcui_command(int argc, char * argv[]);

#ifdef __cplusplus
}  // extern "C" {
#endif

#endif /* DDCUI_CMD_PARSER_H_ */
