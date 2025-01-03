/** @file string_util.h
 *  String utility functions header file
 */

// Copyright (C) 2014-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

/** \cond */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
/** \endcond */

#include "coredefs_base.h"
#include "glib_util.h"

#ifdef __cplusplus
extern "C" {
#endif


//
// General
//

static inline const char * sbool(uint64_t val) {  return (val)  ? "true" : "false"; }

// A macro alternative to sbool()
#define SBOOL(val) ( (val) ? "true" : "false" )

//
// String functions (other than hex)
//

bool   streq(const char * s1, const char * s2);
bool   is_abbrev(const char * value, const char * longname, size_t minchars);
bool   str_starts_with(const char * value_to_test, const char * start_part);
bool   str_ends_with(const char * value_to_test, const char * end_part);
int    str_contains(const char * value_to_test, const char * segment);
bool   str_all_printable(const char * s);
void   strupper(char * s);
void   strlower(char * s);
char * strdup_uc(const char* s);
char * strjoin( const char ** pieces, const int ct, const char * sepstr);
char * chars_to_string(const char * start, int len);
char * strtrim(const char * s);
char * strtrim_r(const char * s, char * buffer, int bufsz);
char * ltrim_in_place(char * s);
char * rtrim_in_place(char * s);
char * trim_in_place(char * s);
char * substr(const char * s, size_t startpos, size_t ct);
char * lsub(const char * s, size_t ct);
char * str_replace_char(char * s, char old_char, char new_char);
char * strcat_new(char * s1, char * s2);
bool   sbuf_append(char * buf, int bufsz, char * sepstr, char * nextval);
char * ascii_strcasestr(const char * haystack, const char * needle);
int    indirect_strcmp(const void * a, const void * b);

typedef bool (*String_Comp_Func)(const char * a, const char * b);
int matches_by_func(    const char * word, const char ** match_list, String_Comp_Func  comp_func);
int exactly_matches_any(const char * word, const char ** match_list);
int starts_with_any(    const char * word, const char ** match_list);

char * int_array_to_string(uint16_t * start, int ct);

/** pointer to null-terminated array of strings */
typedef char** Null_Terminated_String_Array;    // equivalent to GStrv
void ntsa_free(  Null_Terminated_String_Array string_array, bool free_strings);
int  ntsa_length(Null_Terminated_String_Array string_array);
void ntsa_show(  Null_Terminated_String_Array string_array);
int  ntsa_findx( Null_Terminated_String_Array string_array, const char * value, String_Comp_Func func);
int  ntsa_find(  Null_Terminated_String_Array string_array, const char * value);
Null_Terminated_String_Array ntsa_join(  Null_Terminated_String_Array a1, Null_Terminated_String_Array a2, bool dup);
Null_Terminated_String_Array ntsa_copy(Null_Terminated_String_Array a1, bool dup);
Null_Terminated_String_Array ntsa_prepend(char * value, Null_Terminated_String_Array string_array, bool dup);
Null_Terminated_String_Array ntsa_create_empty_array();

Null_Terminated_String_Array strsplit(const char * str_to_split, const char* delims);
Null_Terminated_String_Array strsplit_maxlength(
                                      const char * str_to_split,
                                      uint16_t     max_piece_length,
                                      const char * delims);

GPtrArray * ntsa_to_g_ptr_array(Null_Terminated_String_Array ntsa);
Null_Terminated_String_Array g_ptr_array_to_ntsa(GPtrArray * garray, bool duplicate);


//
// Numeric conversion
//

bool str_to_long( const char * sval, long *  p_ival, int base);
bool str_to_int(  const char * sval, int *   p_ival, int base);
bool str_to_float(const char * sval, float * p_fval);


//
// Hex value conversion.
//

char * canonicalize_possible_hex_value(char * string_value);
bool hhs_to_byte_in_buf(const char * s,  Byte * result);    // converts null terminated string into buffer
bool any_one_byte_hex_string_to_byte_in_buf(const char * s, Byte * result);
bool hhc_to_byte_in_buf(const char * hh, Byte * result);    // converts 2 characters at hh into buffer
int  hhs_to_byte_array(const char * hhs, Byte** ba_loc);
bool hhs4_to_uint16(char * hhs4, uint16_t* result);

char * hexstring(const Byte * bytes, int size);  // buffer returned must be freed
char * hexstring_t(
          const unsigned char * bytes,
          int                   len);
char * hexstring2(
          const unsigned char * bytes,      // bytes to convert
          int                   len,        // number of bytes
          const char *          sepstr,     // separator string between hex digits
          bool                  uppercase,  // use upper case hex characters
          char *                buffer,     // buffer in which to return hex string
          size_t                bufsz);     // buffer size
char * hexstring3_t(
          const unsigned char * bytes,      // bytes to convert
          int                   len,        // number of bytes
          const char *          sepstr,     // separator string between hex digits
          uint8_t               hunk_size,  // separator string frequency
          bool                  uppercase); // use upper case hex characters

void hex_dump_indented_collect(GPtrArray * collector, const Byte* data, int size, int indents);
void fhex_dump_indented(FILE * fh, const Byte* data,  int size, int indents);
void fhex_dump(         FILE * fh, const Byte* bytes, int size);
void hex_dump(                     const Byte* bytes, int size);


//
// Standard function variants that handle stream == NULL
//

int f0putc(int c, FILE * stream);
int f0puts(const char * s, FILE * stream);
int f0printf( FILE * stream, const char * format, ...);
int vf0printf(FILE * stream, const char * format, va_list ap);


//
// Miscellaneous
//

bool all_bytes_zero(Byte * bytes, int bytect);
bool apply_filter_terms(const char * text, char ** terms, bool ignore_case);

#ifdef __cplusplus
}
#endif


#endif /* STRING_UTIL_H_ */
