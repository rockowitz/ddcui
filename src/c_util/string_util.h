/** @file string_util.h
 *
 *  String utility function, copied from ddcutil_string_util.h
 */

// Copyright (C) 2019-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <stdbool.h>

bool is_abbrev(const char * value, const char * longname, size_t  minchars);

bool str_to_float(const char * sval, float * p_fval);

#endif
