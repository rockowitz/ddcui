/** @file string_util.h
 *
 *  String utility function, copied from ddcutil_string_util.h
 */

// Copyright (C) 2019-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <glib-2.0/glib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "coredefs.h"

#ifdef _cplusplus
extern "C" {
#endif


bool is_abbrev(const char * value, const char * longname, size_t  minchars);

bool str_to_float(const char * sval, float * p_fval);

bool any_one_byte_hex_string_to_byte_in_buf(const char * s, Byte * result);

/** pointer to null-terminated array of strings */
typedef char** Null_Terminated_String_Array;
void ntsa_free(  Null_Terminated_String_Array string_array, bool free_strings);
int  ntsa_length(Null_Terminated_String_Array string_array);
void ntsa_show(  Null_Terminated_String_Array string_array);
// int  ntsa_findx( Null_Terminated_String_Array string_array, char * value, String_Comp_Func func);
int  ntsa_find(  Null_Terminated_String_Array string_array, char * value);
Null_Terminated_String_Array  ntsa_join(  Null_Terminated_String_Array a1, Null_Terminated_String_Array a2, bool dup);

Null_Terminated_String_Array strsplit(const char * str_to_split, const char* delims);
Null_Terminated_String_Array strsplit_maxlength(
                                      const char * str_to_split,
                                      uint16_t     max_piece_length,
                                      const char * delims);

GPtrArray * ntsa_to_g_ptr_array(Null_Terminated_String_Array ntsa);
Null_Terminated_String_Array g_ptr_array_to_ntsa(GPtrArray * garray, bool duplicate);

#ifdef _cplusplus
}
#endif


#endif
