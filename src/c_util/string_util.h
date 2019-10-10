/** @file string_util.h
 *
 */

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <stdbool.h>

// copied from ddcutil string_util.h

bool is_abbrev(const char * value, const char * longname, size_t  minchars);

// linking fails with inline form
// inline  char * sbool(int val) {  return (val)  ? "true" : "false"; }
char * sbool(int val);

// A macro alternative to sbool()
#define SBOOL(val) ( (val) ? "true" : "false" )

bool str_to_float(const char * sval, float * p_fval);

// End of functions from string_util.h


#endif
