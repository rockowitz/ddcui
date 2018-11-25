/** \file parsed_cmd.c */

/* Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

/** \cond */
#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/** \endcond */

#include "cmdline/parsed_cmd.h"

/** Allocates new Parsed_Cmd data structure, sets default values.
 *
 *  @return initialized Parsed_Cmd
 */
Parsed_Cmd *  new_parsed_cmd() {
   Parsed_Cmd * parsed_cmd = (Parsed_Cmd*) calloc(1, sizeof(Parsed_Cmd));
   memcpy(parsed_cmd->marker, PARSED_CMD_MARKER, 4);
   // n. all flags are false, byte values 0, integers 0, pointers NULL because of calloc
   // parsed_cmd->flags |= CMD_FLAG_ENABLE_UDF;
   return parsed_cmd;
}


// Debugging function
// void dbgrpt_parsed_cmd(Parsed_Cmd * parsed_cmd, int depth) {
//    printf("(%s) Unimplemented\n", __func__); fflush(stdout);
// }


/** Frees a #Parsed_Cmd instance
 *
 *  @param parsed_cmd  instance to free
 */
void free_parsed_cmd(Parsed_Cmd * parsed_cmd) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting.  parsed_cmd=%p\n", __func__, parsed_cmd);
   assert ( memcmp(parsed_cmd->marker,PARSED_CMD_MARKER,4) == 0);

   free(parsed_cmd);
   if (debug) {
      printf("(%s) Done\n", __func__); fflush(stdout);
   }
}
