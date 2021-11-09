/* \file cmd_parser.h */

// Copyright (C) 2018-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_CMD_PARSER_H_
#define DDCUI_CMD_PARSER_H_

#include "ddcui_parsed_cmd.h"

Parsed_Ddcui_Cmd * parse_ddcui_command(int argc, char * argv[]);

#endif /* DDCUI_CMD_PARSER_H_ */
