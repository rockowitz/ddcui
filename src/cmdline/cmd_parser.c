/* \file cmd_parser.c */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include <assert.h>
#include <config.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "base/ddcui_parms.h"

#include "cmdline/cmd_parser.h"
#include "cmdline/parsed_cmd.h"

/** Primary parsing function
 *
 *  \param  argc      number of command line arguments
 *  \param  argv      array of pointers to command line arguments
 *  \return pointer to a Parsed_Cmd struct if parsing successful
 *          NULL if execution should be terminated
 */
Parsed_Cmd * parse_command(int argc, char * argv[]) {
   bool debug = false;
   if (debug)
      printf("(%s) Starting\n", __func__ );

   if (debug) {
      printf("argc=%d\n", argc);
      int ndx = 0;
      for (; ndx < argc; ndx++) {
         printf("argv[%d] = |%s|\n", ndx, argv[ndx]);
      }
   }

   Parsed_Cmd * parsed_cmd = new_parsed_cmd();

   gboolean version_flag   = false;
   gboolean enable_udf_flag = false;

   GOptionEntry option_entries[] = {
   //  long_name short flags option-type          gpointer           description                    arg description
   //  monitor selection options

      {"udf",     '\0', 0, G_OPTION_ARG_NONE,     &enable_udf_flag,  "Enable user defined feature support", NULL},
      {"noudf",   '\0', G_OPTION_FLAG_REVERSE,
                           G_OPTION_ARG_NONE,     &enable_udf_flag,  "Disable user defined feature support", NULL},

      {"version", 'V',  0, G_OPTION_ARG_NONE,     &version_flag,     "Show version information", NULL},

      { NULL }
   };

   GError* error = NULL;
   GOptionContext* context = g_option_context_new("- DDC query and manipulation");
   g_option_context_add_main_entries(context, option_entries, NULL);

   g_option_context_set_help_enabled(context, true);
   // bool ok = false;

   bool ok = g_option_context_parse(context, &argc, &argv, &error);
   if (!ok) {
      fprintf(stderr, "Option parsing failed: %s\n", error->message);
   }


#define SET_CMDFLAG(_bit, _flag) \
   do { \
      if (_flag) \
         parsed_cmd->flags |= _bit; \
   } while(0)

   SET_CMDFLAG(CMD_FLAG_ENABLE_UDF,        enable_udf_flag);

#undef SET_CMDFLAG

   if (version_flag) {
      printf("ddcui %s\n", DDCUI_VERSION);
       puts("");
      // if no command specified, include license in version information and terminate
         puts("Copyright (C) 2018 Sanford Rockowitz");
         puts("License GPLv2: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>");
         puts("This is free software: you are free to change and redistribute it.");
         puts("There is NO WARRANTY, to the extent permitted by law.");

         exit(0);
   }

   // DBGMSF(debug, "Calling g_option_context_free(), context=%p...", context);
   g_option_context_free(context);


   // if (debug) {
   //    dbgrpt_parsed_cmd(parsed_cmd, 0);
   // }

   if (!ok) {
      free_parsed_cmd(parsed_cmd);
      parsed_cmd = NULL;
   }

   // DBGMSF(debug, "Returning: %p", parsed_cmd);
   return parsed_cmd;
}
