/** \file cmd_parser_aux.h
 *
 *  Functions and strings that are independent of the parser package used.
 */

// Copyright (C) 20014-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_CMD_PARSER_AUX_H_
#define DDCUI_CMD_PARSER_AUX_H_

#include <string.h>

#include "ddcui_parsed_cmd.h"

bool all_digits(char * val, int ct);
bool parse_int_arg(char * val, int * pIval);

extern char * tracing_comma_separated_option_help;
extern char * tracing_multiple_call_option_help;
extern char * trcfunc_multiple_call_option_help;
extern char * trcfile_multiple_call_option_help;
extern char * stats_multiple_call_option_help;

#endif /* DDCUI_CMD_PARSER_AUX_H_ */
