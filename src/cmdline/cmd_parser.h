/* \file cmd_parser.h */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include "cmdline/parsed_cmd.h"

Parsed_Cmd * parse_command(int argc, char * argv[]);

#endif /* CMD_PARSER_H_ */
