/** @file string_util.c
 *
 *  String utility functions copied from the ddcutil version of string_util.c
 */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_util.h"


/** Compares 2 strings for equality, handling nulls
 *
 *  @param s1  first string
 *  @param s2  second string
 *  @return true if the strings match, false if not
 */
bool streq(const char * s1, const char * s2) {
   bool result = false;
   if ( (s1 == NULL && s2 == NULL) ||
        (s1 != NULL && s2 != NULL && (strcmp(s1, s2) == 0) )
      )
      result = true;
   return result;
}


/** Tests if one string is a valid abbreviation of another.
 *
 * @param  value     is this string an abbreviation?
 * @param  longname  unabbreviated value
 * @param  minchars  minimum number of characters that must match
 * @return true/false
 */
bool is_abbrev(const char * value, const char * longname, size_t  minchars) {
   bool result = false;
   if (value && longname) {
      long unsigned int vlen = strlen(value);
      if ( vlen >= minchars &&
           vlen <= strlen(longname) &&
           memcmp(value, longname, vlen) == 0   // n. returns 0 if vlen == 0
         )
      {
         result = true;
      }
   }
   // printf("(%s) value=|%s|, longname=|%s| returning %d\n", __func__, value, longname, result);
   return result;
}


/** Tests if string starts with a string.
 *
 * @param  value_to_test  value to examine
 * @param  prefix         prefix to check for
 * @return true/false
 *
 * @remark Consider using lib function g_str_prefix() ?? instead
 *
 * @remark
 * Returns **false** if either **value_to_test** or **prefix** are null
 */
bool str_starts_with(const char * value_to_test, const char * prefix) {
   return value_to_test && prefix && is_abbrev(prefix, value_to_test, strlen(prefix));
}


/** Tests if string ends with a string.
 *
 * @param  value_to_test  value to examine
 * @param  suffix         substring to check for
 * @return true/false
 *
 * @remark Consider using lib function g_str_suffix() ?? instead
 */
bool str_ends_with(const char * value_to_test, const char * suffix) {
   bool debug = false;
   if (debug)
      printf("(%s) value_to_test=|%s|, end_part=|%s|\n", __func__, value_to_test, suffix);
   int value_len = strlen(value_to_test);
   int end_part_len = strlen(suffix);
   bool result = false;
   if (end_part_len <=value_len) {
      int startpos = value_len-end_part_len;
      result = streq(value_to_test+startpos, suffix);
   }
   if (debug)
      printf("(%s) returning: %d\n", __func__, result);
   return result;
}


// private copy:
static char * sbool(int val) {return  (val) ? "true" : "false"; }


/** Converts a string to a float value.
 *
 * @param sval   string representing an integer
 * @param p_fval address at which to store float value
 * @return true if conversion succeeded, false if it failed
 *
 * @remark
 * \remark
 * If conversion fails, the value pointed to by **p_fval** is unchanged.
 * @remark
 * This function wraps system function strtof(), hiding the ugly details.
 */
bool str_to_float(const char * sval, float * p_fval)
{
   bool debug = false;
   if (debug)
      printf("(%s) sval->|%s|\n", __func__, sval);

   bool ok = false;
   if ( *sval != '\0') {
      char * tailptr;
      float result = strtof(sval, &tailptr);

      if (*tailptr == '\0') {
         *p_fval = result;
         ok = true;
      }
   }

   if (debug) {
      if (ok)
        printf("(%s) sval=%s, Returning: %s, *p_fval = %16.7f\n", __func__, sval, sbool(ok), *p_fval);
      else
        printf("(%s) sval=%s, Returning: %s\n", __func__, sval, sbool(ok));
   }
   return ok;
}


/** Creates an upper case copy of an ASCII string
 *
 * @param  s  string to copy
 * @return newly allocated string, NULL if s is NULL
 */
char * strdup_uc(const char* s) {
   if (!s)
      return NULL;
   char * us = strdup( s );
   char * p = us;
   while (*p) {*p=toupper(*p); p++; }
   return us;
}



//
// Hex value conversion.
//

/** Converts a (null terminated) string of 2 hex characters to
 * its byte value.
 *
 * @param  s         pointer to hex string
 * @param  result    pointer to byte in which converted value will be returned
 * @retval **true**  successful conversion,
 * @retval **false** string does not consist of hex characters,
 *                    or is not 2 characters in length.
 */
bool hhs_to_byte_in_buf(const char * s, Byte * result)
{
   // printf("(%s) Starting s=%s, strlen(s)=%zd\n", __func__, s, strlen(s) );
   // consider changing to fail if len != 2, or perhaps len != 1,2
   //assert(strlen(s) == 2);

   bool ok = true;
   if (strlen(s) != 2)
      ok = false;
   else {
      char * endptr = NULL;
      errno = 0;
      long longtemp = strtol(s, &endptr, 16 );
      int errsv = errno;
      if (endptr != s+2 || errsv != 0) {
         ok = false;
      }
      else
         *result = (Byte) longtemp;
   }

   // printf("(%s) Returning ok=%d\n", __func__, ok);
   return ok;
}


/** Converts a hex string representing a single byte into its byte value.
 *  This is a more lenient version of hhs_to_byte_in_buf(), allowing
 *  the value to begin with "0x" or "x", or end with "h".  The allowed
 *  prefix or suffix is case-insensitive.
 *
 *  @param  s         pointer to hex string
 *  @param  result    pointer to byte in which result will be returned
 *  @retval **true**  successful conversion,
 *  @retval **false** conversion unsuccessful
 */
bool any_one_byte_hex_string_to_byte_in_buf(const char * s, Byte * result)
{
   // printf("(%s) s = |%s|\n", __func__, s);
   char * suc = strdup_uc(s);
   char * suc0 = suc;
   if (str_starts_with(suc, "0X"))
         suc = suc + 2;
   else if (*suc == 'X')
         suc = suc + 1;
   else if (str_ends_with(suc, "H"))
         *(suc+strlen(suc)-1) = '\0';
   bool ok = hhs_to_byte_in_buf(suc, result);
   free(suc0);
   // printf("(%s) returning %d, *result=0x%02x\n", __func__, ok, *result);
   return ok;
}


/** Frees a null terminated array of strings.
 *
 *  @param string_array null terminated array of pointers to strings
 *  @param free_strings if try, each string in the array is freed as well
 */
void ntsa_free(Null_Terminated_String_Array string_array, bool free_strings) {
   if (string_array) {
      if (free_strings) {
      int ndx = 0;
      while (string_array[ndx] != NULL)
         free(string_array[ndx++]);
      }
      free(string_array);
   }
}


/* Reports the contents of a #Null_Terminated_String_Array.
 *
 * @param string_array null-terminated string array
 *
 * @remark This is not a **report** function as that would make string_util
 * depend on report_util, creating a circular dependency within util
 */
void ntsa_show(Null_Terminated_String_Array string_array) {
   assert(string_array);
   printf("Null_Terminated_String_Array at %p:\n", (void*) string_array);
   int ndx = 0;
   while (string_array[ndx]) {
      printf("  %p: |%s|\n", string_array[ndx], string_array[ndx]);
      ndx++;
   }
   printf("Total entries: %d\n", ndx);
}


/** Returns the number of strings in a null terminated array of strings.
 *
 * @param  string_array null terminated array of pointers to strings
 * @return number of strings in the array
 */
int ntsa_length(Null_Terminated_String_Array string_array) {
   assert(string_array);
   int ndx = 0;
   while (string_array[ndx] != NULL) {
      ndx++;
   }
   return ndx;
}



/** Converts a #Null_Terminated_String_Array to a GPtrArray of pointers to strings.
 * The underlying strings are referenced, not duplicated.
 *
 * @param  ntsa  null-terminated array of strings
 * @return newly allocate GPtrArray
 */
GPtrArray * ntsa_to_g_ptr_array(Null_Terminated_String_Array ntsa) {
   int len = ntsa_length(ntsa);
   GPtrArray * garray = g_ptr_array_sized_new(len);
   int ndx;
   for (ndx=0; ndx<len; ndx++) {
      g_ptr_array_add(garray, ntsa[ndx]);
   }
   return garray;
}


/** Converts a GPtrArray of pointers to strings to a Null_Terminated_String_Array.
 *
 * @param gparray pointer to GPtrArray
 * @param duplicate if true, the strings are duplicated
 *                  if false, the pointers are copied
 * @return null-terminated array of string pointers
 */
Null_Terminated_String_Array
g_ptr_array_to_ntsa(
      GPtrArray * gparray,
      bool        duplicate)
{
   assert(gparray);
   Null_Terminated_String_Array ntsa = calloc(gparray->len+1, sizeof(char *));
   for (guint ndx=0; ndx < gparray->len; ndx++) {
      if (duplicate)
         ntsa[ndx] = strdup(g_ptr_array_index(gparray,ndx));
      else
         ntsa[ndx] = g_ptr_array_index(gparray,ndx);
   }
   return ntsa;
}



/** Splits a string based on a list of delimiter characters.
 *
 *  @param  str_to_split     string to be split
 *  @param  delims           string of delimiter characters
 *  @return null terminated array of pieces
 *
 * Note: Each character in delims is used as an individual test.
 * The full string is NOT a delimiter string.
 */
Null_Terminated_String_Array strsplit(const char * str_to_split, const char * delims) {
   bool debug = false;
   int max_pieces = (strlen(str_to_split)+1);
   if (debug)
      printf("(%s) str_to_split=|%s|, delims=|%s|, max_pieces=%d\n", __func__, str_to_split, delims, max_pieces);

   char** workstruct = calloc(sizeof(char *), max_pieces+1);
   int piecect = 0;

   char * str_to_split_dup = strdup(str_to_split);
   char * rest = str_to_split_dup;
   char * token;
   // originally token assignment was in while() clause, but valgrind
   // complaining about uninitialized variable, trying to figure out why
   token = strsep(&rest, delims);      // n. overwrites character found
   while (token) {
      // printf("(%s) token: |%s|\n", __func__, token);
      if (strlen(token) > 0)
         workstruct[piecect++] = strdup(token);
      token = strsep(&rest, delims);
   }
   if (debug)
      printf("(%s) piecect=%d\n", __func__, piecect);
   char ** result = calloc(sizeof(char *), piecect+1);
   // n. workstruct[piecect] == NULL because we used calloc()
   memcpy(result, workstruct, (piecect+1)*sizeof(char*) );
   if (debug) {
      int ndx = 0;
      char * curpiece = result[ndx];
      while (curpiece != NULL) {
         printf("(%s) curpiece=%p |%s|\n", __func__, curpiece, curpiece);
         ndx++;
         curpiece = result[ndx];

      }
   }
   free(workstruct);
   free(str_to_split_dup);
   return result;
}


/** Splits a string into segments, each of which is no longer
 *  that a specified number of characters.  If delimiters are
 *  specified, then they are used to split the string into segments.
 *  Otherwise all segments, except possibly the last, are
 *  **max_piece_length** in length.
 *
 *  @param  str_to_split     string to be split
 *  @param  max_piece_length maximum length of each segment
 *  @param  delims           string of delimiter characters
 *  @return null terminated array of pieces
 *
 * @remark
 * Each character in **delims** is used as an individual test.
 * The full string is NOT a delimiter string.
 */
Null_Terminated_String_Array
strsplit_maxlength(
      const char *  str_to_split,
      uint16_t      max_piece_length,
      const char *  delims)
{
   bool debug = false;
   if (debug)
      printf("(%s) max_piece_length=%u, delims=|%s|, str_to_split=|%s|\n",
             __func__, max_piece_length, delims, str_to_split);

   GPtrArray * pieces = g_ptr_array_sized_new(20);
   char * str_to_split2 = strdup(str_to_split);   // work around constness
   char * start = str_to_split2;
   char * str_to_split2_end = str_to_split2 + strlen(str_to_split);
   if (debug)
      printf("(%s)x start=%p, str_to_split2_end=%p\n", __func__, start, str_to_split2_end);
   while (start < str_to_split2_end) {
      if (debug)
         printf("(%s) start=%p, str_to_split2_end=%p\n", __func__, start, str_to_split2_end);
      char * end = start + max_piece_length;
      if (end > str_to_split2_end)
         end = str_to_split2_end;
      // int cursize = end-start;
      // printf("(%s) end=%p, start=%p, cursize=%d, max_piece_length=%d\n",
      //        __func__, end, start, cursize, max_piece_length);
      if ( end < str_to_split2_end) {
         // printf("(%s) Need to split. \n", __func__);
         if (delims) {
            char * last = end-1;
            while(last >= start) {
               // printf("(%s) last = %p\n", __func__, last);
               if (strchr(delims, *last)) {
                  end = last+1;
                  break;
               }
               last--;
            }
         }
      }
      char * piece = strndup(start, end-start);
      g_ptr_array_add(pieces, piece);
      start = start + strlen(piece);
   }

   // for some reason, if g_ptr_array_to_ntsa() is called with duplicate=false and
   // g_ptr_array(pieces, false) is called, valgrind complains about memory leak
   Null_Terminated_String_Array result = g_ptr_array_to_ntsa(pieces, /*duplicate=*/ true);
   g_ptr_array_set_free_func(pieces, g_free);
   g_ptr_array_free(pieces, true);
   free(str_to_split2);
   if (debug)
      ntsa_show(result);
   return result;
}



