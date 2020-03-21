/* \file cmd_parser.c
 * ddcui command line parser for ddcui
 */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ddcutil_types.h>
#include <ddcutil_c_api.h>

// #include "base/core.h"
#include "c_util/string_util.h"

#include "base/ddcui_parms.h"

#include "cmdline/cmd_parser_aux.h"
#include "cmdline/parsed_cmd.h"
#include "cmdline/cmd_parser.h"

//#include "base/core.h"

// Variables used by callback functions
static DDCA_Stats_Type   stats_work    = DDCA_STATS_NONE;
char *   maxtrywork      = NULL;



// Callback function for processing --stats
gboolean stats_arg_func(const    gchar* option_name,
                        const    gchar* value,
                        gpointer data,
                        GError** error)
{
   bool debug = false;
   // DBGMSF(debug,"option_name=|%s|, value|%s|, data=%p", option_name, value, data);
   if (debug)
      printf("(%s) option_name=|%s|, value|%s|, data=%p\n", __func__, option_name, value, data);

   bool ok = true;
   if (value) {
      char * v2 = g_ascii_strup(value, -1);  // n. returns newly allocated string
      if ( strcmp(v2,"ALL") == 0 ) {
         stats_work |= DDCA_STATS_ALL;
      }
      else if (strcmp(v2,"TRY") == 0 || is_abbrev(v2, "TRIES",3)) {
         stats_work |= DDCA_STATS_TRIES;
      }
      else if ( is_abbrev(v2, "CALLS",3)) {
         stats_work |= DDCA_STATS_CALLS;
      }
      else if (strcmp(v2,"ERRS") == 0 || is_abbrev(v2, "ERRORS",3)) {
         stats_work |= DDCA_STATS_ERRORS;
      }
      else if ( is_abbrev(v2,"ELAPSED",3) || is_abbrev(v2, "TIME",3)) {
         stats_work |= DDCA_STATS_ELAPSED;
      }
      else
         ok = false;
      free(v2);
   }
   else {
      stats_work = DDCA_STATS_ALL;
   }

   if (!ok) {
      g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_FAILED, "invalid stats type: %s", value );
   }
   return ok;
}



/** Primary parsing function
 *
 *  \param  argc      number of command line arguments
 *  \param  argv      array of pointers to command line arguments
 *  \return pointer to a Parsed_Cmd struct if parsing successful
 *          NULL if execution should be terminated
 */
Parsed_Cmd * parse_command(int argc, char * argv[]) {
   bool debug = true;

   if (debug) {
      printf("(%s) Starting\n", __func__ );

      printf("argc=%d\n", argc);
      for (int ndx=0; ndx < argc; ndx++) {
         printf("argv[%d] = |%s|\n", ndx, argv[ndx]);
      }
   }

   Parsed_Cmd * parsed_cmd = new_parsed_cmd();

   gboolean ddc_flag               = false;
   gboolean enable_udf_flag        = false;
   gboolean nousb_flag             = false;
   gboolean report_freed_excp_flag = false;
   gboolean timestamp_trace_flag = false;
   gboolean thread_id_trace_flag = false;
   gboolean version_flag           = false;
   gboolean show_styles_flag       = false;
   gchar**  cmd_and_args           = NULL;
   gchar**  trace_classes          = NULL;
   gchar**  trace_filenames        = NULL;
   gchar**  trace_functions        = NULL;
   char *   sleep_multiplier_work  = NULL;

   gchar*   view_work              = NULL;
   gchar*   nc_values_source_work  = NULL;
   gchar*   feature_set_work       = NULL;
   gboolean control_key_required   = false;
   gboolean show_unsupported_features = false;

   gboolean include_only_capabilities = true;     // hack, default value shouldn't be set here
   gboolean include_all_capabilities = false;

   gboolean only_capabilities_true_set  = false;
   gboolean only_capabilities_false_set = false;
   gboolean all_capabilities_true_set   = false;
   gboolean all_capabilities_false_set  = false;

   GOptionEntry option_entries[] = {
   //  long_name short flags option-type          gpointer           description                    arg description
   //  monitor selection options

   // output control
      {"ddc",     '\0', 0, G_OPTION_ARG_NONE,     &ddc_flag,         "Report DDC protocol and data errors", NULL},

      {"udf",     '\0', 0, G_OPTION_ARG_NONE,     &enable_udf_flag,  "Enable user defined feature support", NULL},
      {"noudf",   '\0', G_OPTION_FLAG_REVERSE,
                           G_OPTION_ARG_NONE,     &enable_udf_flag,  "Disable user defined feature support", NULL},
      {"nousb",  '\0',  0, G_OPTION_ARG_NONE,     &nousb_flag,       "Do not detect USB devices", NULL},

      // tuning
      {"maxtries",'\0', 0, G_OPTION_ARG_STRING,   &maxtrywork,       "Max try adjustment",  "comma separated list" },
#ifdef UNIMPLEMENTED
      // unclear how to implement in GUI environment
      {"stats",   's',  G_OPTION_FLAG_OPTIONAL_ARG,
                           G_OPTION_ARG_CALLBACK, stats_arg_func,    "Show retry statistics",    "stats type"},
#endif
      {"sleep-multiplier", '\0', 0,
                                 G_OPTION_ARG_STRING,   &sleep_multiplier_work, "Multiplication factor for DDC sleeps", "number"},


  // debugging
      {"excp",     '\0',   0, G_OPTION_ARG_NONE,     &report_freed_excp_flag,  "Report freed exceptions", NULL},
      {"trace",    '\0',   0, G_OPTION_ARG_STRING_ARRAY, &trace_classes, "Trace classes",         "trace class name" },
  //  {"trace",    '\0',   0, G_OPTION_ARG_STRING,   &tracework,        "Trace classes",          "comma separated list" },
      {"trcfunc",  '\0',   0, G_OPTION_ARG_STRING_ARRAY, &trace_functions, "Trace functions",     "function name" },
      {"trcfile",  '\0',   0, G_OPTION_ARG_STRING_ARRAY, &trace_filenames,    "Trace files",     "file name" },
      {"timestamp",'\0',   0, G_OPTION_ARG_NONE,   &timestamp_trace_flag, "Prepend trace msgs with elapsed time",  NULL},
      {"ts",       '\0',   0, G_OPTION_ARG_NONE,   &timestamp_trace_flag, "Prepend trace msgs with elapsed time",  NULL},
      {"thread-id",'\0',   0, G_OPTION_ARG_NONE,   &thread_id_trace_flag, "Prepend trace msgs with thread id",     NULL},
      {"tid",      '\0',   0, G_OPTION_ARG_NONE,   &thread_id_trace_flag, "Prepend trace msgs with thread id",     NULL},

      {"view",     '\0',   0, G_OPTION_ARG_STRING,   &view_work,            "Initial view",             "Summary|Capabilities|Features"},
      {"require-control-key",
                   '\0',   0, G_OPTION_ARG_NONE,     &control_key_required, "Control key must be pressed to move slider", NULL},
     {"nc-values-source",
                   '\0',   0, G_OPTION_ARG_STRING,   &nc_values_source_work,  "Initial NC values source",   "MMCS|Capabilities|Both"},

      {"feature-set",
                   '\0',   0, G_OPTION_ARG_STRING,   &feature_set_work,  "Feature set selection",
                                                                       "MMCS|Capabilities|Manufacturer|Color|Scan"},

      {"show-unsupported",
                   '\0',   0, G_OPTION_ARG_NONE,      &show_unsupported_features, "Show unsupported features", NULL},
      {"only-capabilities",
                   '\0',   0, G_OPTION_ARG_NONE,      &only_capabilities_true_set, "Include only values in capabilities", NULL},
      {"not-only-capabilities",
                    '\0',  0, G_OPTION_ARG_NONE,      &only_capabilities_false_set, "Do not exclude values only in MCCS", NULL},
      {"all-capabilities",
                   '\0',   0, G_OPTION_ARG_NONE,      &all_capabilities_true_set, "Include all values in capabilities", NULL},
      {"not-all-capabilities",
                     '\0', 0, G_OPTION_ARG_NONE,      &all_capabilities_false_set, "Negate include all values in capabilities", NULL},




#ifdef REF
                               _ui->showUnsupported_checkbox->setChecked( fsel->_showUnsupportedFeatures);
                               _ui->onlyCapabilities_checkbox->setChecked(fsel->_includeOnlyCapabilities);
                               _ui->allCapabilities_checkbox->setChecked( fsel->_includeAllCapabilities);
#endif

// Pre-GUI queries
      {"styles",   '\0',   0, G_OPTION_ARG_NONE,     &show_styles_flag,     "List known styles",        NULL},
      {"version",  'V',    0, G_OPTION_ARG_NONE,     &version_flag,         "Show version information", NULL},



      // collect to verify that does not exist
      {G_OPTION_REMAINING,
                 '\0', 0,  G_OPTION_ARG_STRING_ARRAY, &cmd_and_args, NULL,   NULL},

      { NULL }
   };

   GError* error = NULL;
   GOptionContext* context = g_option_context_new("- graphical interface to ddcutil");
   g_option_context_add_main_entries(context, option_entries, NULL);

   g_option_context_set_help_enabled(context, true);
   // bool ok = false;

#ifdef OLD
   const char * pieces3[] = {
                             tracing_multiple_call_option_help,
                             "\n",
                             trcfunc_multiple_call_option_help,
                             "\n",
                             trcfile_multiple_call_option_help,
                             "\n",
                             stats_multiple_call_option_help,
                             "\n",
                             maxtries_option_help};
   char * help_description = strjoin(pieces3, 10, NULL);
#endif
   char * help_description = g_strjoin("\n",
         tracing_multiple_call_option_help,
         trcfunc_multiple_call_option_help,
         trcfile_multiple_call_option_help,
         stats_multiple_call_option_help,
         maxtries_option_help,
         NULL
         );


   // on --help, comes at end after option detail
   g_option_context_set_description(context, help_description);
   free(help_description);

   bool ok = g_option_context_parse(context, &argc, &argv, &error);
   if (!ok) {
      fprintf(stderr, "Option parsing failed: %s\n", error->message);
   }


#define SET_CMDFLAG(_bit, _flag) \
   do { \
      if (_flag) \
         parsed_cmd->flags |= _bit; \
   } while(0)

   parsed_cmd->stats_types      = stats_work;
   SET_CMDFLAG(CMD_FLAG_DDCDATA,           ddc_flag);
   SET_CMDFLAG(CMD_FLAG_TIMESTAMP_TRACE,   timestamp_trace_flag);
   SET_CMDFLAG(CMD_FLAG_THREAD_ID_TRACE,   thread_id_trace_flag);
   SET_CMDFLAG(CMD_FLAG_REPORT_FREED_EXCP, report_freed_excp_flag);

   SET_CMDFLAG(CMD_FLAG_ENABLE_UDF,        enable_udf_flag);
   SET_CMDFLAG(CMD_FLAG_NOUSB,             nousb_flag);
   SET_CMDFLAG(CMD_FLAG_SHOW_STYLES,       show_styles_flag);

   SET_CMDFLAG(CMD_FLAG_UI_REQUIRE_CONTROL_KEY, control_key_required);
   SET_CMDFLAG(CMD_FLAG_SHOW_UNSUPPORTED,       show_unsupported_features);
   SET_CMDFLAG(CMD_FLAG_NC_VALUES_ALL_IN_CAPABILITIES, include_all_capabilities);
   SET_CMDFLAG(CMD_FLAG_NC_VALUES_MUST_BE_IN_CAPABILITIES, include_only_capabilities);

#undef SET_CMDFLAG

#ifdef REF
   Optional_True_False nc_values_all_in_capabilities;
   Optional_True_False nc_values_must_be_in_capabilities;
#endif

   if (all_capabilities_true_set || all_capabilities_false_set) {
      if (all_capabilities_true_set)
         parsed_cmd->nc_values_all_in_capabilities = TRIVAL_TRUE;
      else
         parsed_cmd->nc_values_all_in_capabilities = TRIVAL_FALSE;
   }
   else
      parsed_cmd->nc_values_all_in_capabilities = TRIVAL_UNSET;

   if (only_capabilities_true_set || only_capabilities_false_set) {
      if (only_capabilities_true_set)
         parsed_cmd->nc_values_must_be_in_capabilities = TRIVAL_TRUE;
      else
         parsed_cmd->nc_values_must_be_in_capabilities = TRIVAL_FALSE;
   }
   else
      parsed_cmd->nc_values_must_be_in_capabilities = TRIVAL_UNSET;


// #ifdef REPLACE_NTSA
   if (maxtrywork) {
       bool saved_debug = debug;
       debug = false;
       // DBGMSF(debug, "retrywork, argument = |%s|", maxtrywork );
       if (debug)
          printf("(%s) maxtrywork, argument = |%s|\n", __func__, maxtrywork);

       // Null_Terminated_String_Array pieces = strsplit(maxtrywork, ",");
       gchar ** pieces = g_strsplit(maxtrywork, ",", -1);
       // int ntsal = ntsa_length(pieces);
       int ntsal = g_strv_length(pieces);
       // DBGMSF(debug, "ntsal=%d", ntsal );
       if (debug)
          printf("(%s) ntsal=%d\n", __func__, ntsal);
       if (ntsal != 3) {
          fprintf(stderr, "--retries requires 3 values\n");
          ok = false;
       }
       else {
          int ndx = 0;
          // char trimmed_piece[10];    // unused
          for (; pieces[ndx] != NULL; ndx++) {
             // char * token = strtrim_r(pieces[ndx], trimmed_piece, 10);
             char * token = g_strstrip(pieces[ndx]);
             int max_max_tries = ddca_max_max_tries();
             // DBGMSG("token=|%s|", token);
             if (strlen(token) > 0 && strcmp(token,".") != 0) {
                int ival;
                int ct = sscanf(token, "%d", &ival);
                if (ct != 1) {
                   fprintf(stderr, "Invalid --maxtries value: %s\n", token);
                   ok = false;
                }
                else if (ival > max_max_tries) {
                   fprintf(stderr, "--maxtries value %d exceeds %d\n", ival, max_max_tries);
                   ok = false;
                }
                else {
                   parsed_cmd->max_tries[ndx] = ival;
                }
             }
          }
          assert(ndx == ntsal);
       }
       // ntsa_free(pieces, /* free_strings */ true);
       g_strfreev(pieces);

       // DBGMSF(debug, "retries = %d,%d,%d", parsed_cmd->max_tries[0],
       //                                    parsed_cmd->max_tries[1],
       //                                 parsed_cmd->max_tries[2]);
       if (debug)
          printf("(%s) retries = %d,%d,%d\n",
                __func__,
                parsed_cmd->max_tries[0],
                parsed_cmd->max_tries[1],
                parsed_cmd->max_tries[2]);
       debug = saved_debug;
    }
// #endif


   if (sleep_multiplier_work) {
      // DBGMSF(debug, "sleep_multiplier_work = |%s|", sleep_multiplier_work);
      float multiplier = 0.0f;
      bool arg_ok = str_to_float(sleep_multiplier_work, &multiplier);
      if (arg_ok) {
         if (multiplier <= 0.0f)
            arg_ok = false;
      }

      if (!arg_ok) {
          fprintf(stderr, "Invalid sleep-multiplier: %s\n", sleep_multiplier_work );
          ok = false;
      }
      else {
         parsed_cmd->sleep_multiplier = multiplier;
      }
   }


   // #ifdef MULTIPLE_TRACE
      if (trace_classes) {
         DDCA_Trace_Group traceClasses = 0x00;
         int ndx = 0;
         for (;trace_classes[ndx] != NULL; ndx++) {
            char * token = g_ascii_strup(trace_classes[ndx], -1);   // n g_ascii_strup returns newly allocated string
            // DBGMSG("token=|%s|", token);
            if (strcmp(token, "ALL") == 0 || strcmp(token, "*") == 0) {
               traceClasses = DDCA_TRC_ALL;   // 0xff
            }
            else {
               // DBGMSG("token: |%s|", token);
               DDCA_Trace_Group tg = ddca_trace_group_name_to_value(token);
               // DBGMSG("tg=0x%02x", tg);
               if (tg) {
                  traceClasses |= tg;
               }
               else {
                  fprintf(stderr, "Invalid trace group: %s\n", token);
                  ok = false;
               }
           }
           free(token);
         }
         // DBGMSG("traceClasses = 0x%02x", traceClasses);
         parsed_cmd->traced_groups = traceClasses;
      }
   // #endif

      if (trace_functions) {
         parsed_cmd->traced_functions = trace_functions;
      }
      if (trace_filenames) {
         parsed_cmd->traced_files = trace_filenames;
      }

      if (view_work) {
         printf("view_work = %p -> |%s|\n", view_work, view_work);
         Parsed_View pv = find_view_table_value(view_work);
         if (pv == VIEW_UNSET) {
            fprintf(stderr, "Unrecognized: %s\n", view_work);
            ok = false;
         }
         else{
            parsed_cmd->view = pv;
         }
      }


      if (nc_values_source_work) {
         printf("nc_values_source_work = %p -> |%s|\n", nc_values_source_work, nc_values_source_work);
         Parsed_NC_Values_Source src = find_nc_values_source_table_value(nc_values_source_work);
         if (src == VIEW_UNSET) {
            fprintf(stderr, "Unrecognized: %s\n", nc_values_source_work);
            ok = false;
         }
         else{
            parsed_cmd->nc_values_source = src;
         }
      }

#define VALUE_LOOKUP(_ENUM, _NAME, _NOT_FOUND_VALUE) \
      if (_NAME ## _work) {                          \
         _ENUM src = find_ ## _NAME ##_table_value(_NAME ## _work);                  \
         if (src == _NOT_FOUND_VALUE) {                                        \
            fprintf(stderr, "Unrecognized: %s\n", _NAME ## _work);             \
            ok = false;                                                        \
         }                                                                     \
         else{                                                                 \
            parsed_cmd->_NAME = src;                                           \
         }                                                                     \
      }

      VALUE_LOOKUP(Parsed_Feature_Set, feature_set, FS_UNSET);





      if (cmd_and_args && cmd_and_args[0]) {
      // int rest_ct = 0;   // unused
      // don't pull debug into the if clause, need rest_ct to be set
      // if (cmd_and_args) {
      //    for (; cmd_and_args[rest_ct] != NULL; rest_ct++) {
      //          DBGMSF(debug, "cmd_and_args[%d]: %s", rest_ct, cmd_and_args[rest_ct]);
      //    }
      // }

         char * s = g_strjoinv(" ",cmd_and_args);
         printf("Unrecognized: %s\n", s);
         free(s);
         ok = false;

      }


   if (version_flag) {
      printf("ddcui %s\n", DDCUI_VERSION);
       puts("");
      // if no command specified, include license in version information and terminate
         puts("Copyright (C) 2018-2019 Sanford Rockowitz");
         puts("License GPLv2: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>");
         puts("This is free software: you are free to change and redistribute it.");
         puts("There is NO WARRANTY, to the extent permitted by law.");

         exit(0);
   }

   if (show_styles_flag) {
   }

   // DBGMSF(debug, "Calling g_option_context_free(), context=%p...", context);
   g_option_context_free(context);


   if (debug) {
      dbgrpt_parsed_cmd(parsed_cmd);
   }

   if (!ok) {
      free_parsed_cmd(parsed_cmd);
      parsed_cmd = NULL;
   }

   printf("Returning: %p\n", parsed_cmd);
   return parsed_cmd;
}
