/** \file parsed_cmd.c - parsed ddcui command line */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

/** \cond */
#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/** \endcond */

#include "cmdline/parsed_cmd.h"


static inline char * sbool(int b) {
   return (b) ? "true" : "false";
}



/** Allocates new Parsed_Cmd data structure, sets default values.
 *
 *  @return initialized #Parsed_Cmd
 */
Parsed_Cmd *  new_parsed_cmd() {
   Parsed_Cmd * parsed_cmd = (Parsed_Cmd*) calloc(1, sizeof(Parsed_Cmd));
   memcpy(parsed_cmd->marker, PARSED_CMD_MARKER, 4);
   // n. all flags are false, byte values 0, integers 0, pointers NULL because of calloc
   // parsed_cmd->flags |= CMD_FLAG_ENABLE_UDF;
   return parsed_cmd;
}


// Debugging function
void dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd) {
   printf("(%s) Parsed_Cmd at %p\n", __func__, parsed_cmd);
   if (parsed_cmd) {
      printf("   stats:                     0x%08x\n",   parsed_cmd->stats_types);
#ifdef UNIMPLEMENTED
      printf("   timestamp_trace:           %s\n",      sbool(parsed_cmd->flags & CMD_FLAG_TIMESTAMP_TRACE));
#endif
      printf("   traced_groups              0x%08x\n",  parsed_cmd->traced_groups);
      if (parsed_cmd->traced_functions) {
         char * joined = g_strjoinv(", ", parsed_cmd->traced_functions);
         printf("   traced_functions:          %s\n", joined);
         free(joined);
      }
      else
         printf("   traced_functions:          none\n" );
      if (parsed_cmd->traced_files) {
         char * joined = g_strjoinv(", ", parsed_cmd->traced_files);
         printf("   traced_files:                %s\n", joined);
         free(joined);
      }
      else
         printf("   traced_files:              none\n" );

      char buf[20];
      snprintf(buf,20, "%d,%d,%d", parsed_cmd->max_tries[0], parsed_cmd->max_tries[1], parsed_cmd->max_tries[2] );
      printf("   max_retries:               %s\n", buf);
      printf("   report_freed_exceptions:   %s\n", sbool( parsed_cmd->flags & CMD_FLAG_REPORT_FREED_EXCP) );
      printf("   enable udf:                %s\n", sbool(parsed_cmd->flags & CMD_FLAG_ENABLE_UDF) );
      printf("   nousb                      %s\n", sbool(parsed_cmd->flags & CMD_FLAG_NOUSB) );
      printf("   display ddc errors:        %s\n", sbool(parsed_cmd->flags & CMD_FLAG_DDCDATA) );
      printf("   sleep multiplier:         %9.1f\n", parsed_cmd->sleep_multiplier);

   }
}




/** Frees a #Parsed_Cmd instance
 *
 *  @param parsed_cmd  instance to free
 */
void free_parsed_cmd(Parsed_Cmd * parsed_cmd) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting.  parsed_cmd=%p\n", __func__, (void*) parsed_cmd);

   if (parsed_cmd) {
      assert ( memcmp(parsed_cmd->marker,PARSED_CMD_MARKER,4) == 0);
      parsed_cmd->marker[3] = 'x';

#ifdef string_util
      ntsa_free(parsed_cmd->traced_files, true);
       ntsa_free(parsed_cmd->traced_functions, true);
#endif
      g_strfreev(parsed_cmd->traced_files);
      g_strfreev(parsed_cmd->traced_functions);


      free(parsed_cmd);
   }

   if (debug) {
      printf("(%s) Done\n", __func__); fflush(stdout);
   }
}
