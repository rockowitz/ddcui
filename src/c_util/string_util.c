/** @file string_util.c
 *
 *  String utility functions copied from the ddcutil version of string_util.c
 */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_util.h"


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

