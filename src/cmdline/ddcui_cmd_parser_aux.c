/** \file cmd_parser_aux.c
 *
 *  Functions and strings that are independent of the parser package used.
 */

// Copyright (C) 2014-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// #include <config.h>

/** \cond */
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #include "util/string_util.h"
/** \endcond */

// #include "base/parms.h"

#include "ddcui_cmd_parser_aux.h"

bool all_digits(char * val, int ct) {
   // bool debug = false;
   // DBGMSF(debug, "ct-%d, val -> |%.*s|", ct, ct, val );
   bool ok = true;
   int ndx;
   for (ndx = 0; ndx < ct; ndx++) {
      if ( !isdigit(*(val+ndx)) ) {
         ok = false;
         break;
      }
   }
   // DBGMSF(debug, "Returning: %d  ", ok );
   return ok;
}


bool parse_int_arg(char * val, int * pIval) {
   int ct = sscanf(val, "%d", pIval);
   return (ct == 1);
}


char * tracing_comma_separated_option_help =
       "Tracing:\n"
       "  The argument to --trace is a comma separated list of trace classes, surrounded by \n"
       "  quotation marks if necessary."
       "  e.g. --trace all, --trace \"I2C,ADL\"\n"
       "  Valid trace classes are:  BASE, SLEEP, I2C, ADL, DDC, DDCIO, VCP, TOP, ENV, API, UDF, ALL.\n"
       "  Trace class names are not case sensitive.\n"
       "  (Some trace classes are more useful than others.)\n"
  //   "\n"
      ;

char * tracing_multiple_call_option_help =
       "Trace by trace class:\n"
       "  The argument to --trace is a trace class.  Use the --trace option multiple\n"
       "  times to activate multiple trace classes, e.g. \"--trace i2c --trace ddc\"\n"
       "  Valid trace classes are:  BASE, I2C, ADL, DDC, DDCIO, VCP, TOP, ENV, API, UDF, ALL.\n"
       "  Trace class names are not case sensitive.\n"
       "  (Some trace classes are more useful than others.)\n"
  //   "\n"
      ;

char * trcfunc_multiple_call_option_help =
       "Trace by function name:\n"
       "  The argument to --trcfunc is a function name.  Use the --trcfunc option multiple\n"
       "  times to trace multiple functions, e.g. \"--trcfunc func1 --trcfunc func2\"\n"
      ;

char * trcfile_multiple_call_option_help =
       "Trace by file name:\n"
       "  The argument to --trcfile is a simple file name.  Use the --trcfile option multiple\n"
       "  times to trace multiple functions, e.g. \"--trcfile ddc_edid.c --trcfile ddc_output.c\"\n"
      ;


char * stats_multiple_call_option_help =
       "Stats:\n"
       "  The argument to --stats is a statistics class.  Use the --stats option multiple\n"
       "  times to activate multiple statistics classes, e.g. \"--stats calls --stats errors\"\n"
       "  Valid statistics classes are:  TRY, TRIES, ERRS, ERRORS, CALLS, ALL.\n"
       "  Statistics class names are not case sensitive and can abbreviated to 3 characters.\n"
       "  If no argument is specified, or ALL is specified, then all statistics classes are\n"
       "  output.\n"
      ;

