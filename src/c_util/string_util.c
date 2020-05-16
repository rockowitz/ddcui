/** @file string_util.c
 *
 *  String utility functions copied from the ddcutil version of string_util.c
 */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

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



