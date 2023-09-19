/* \file ddcui_cmd_parser.c
 * ddcui command line parser
 */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <ddcutil_macros.h>
#include <ddcutil_types.h>
#include <ddcutil_c_api.h>

#include "c_util/report_util.h"
#include "c_util/string_util.h"

// #include "base/ddcui_core.h"
#include "base/ddcui_parms.h"
#include "base/feature_list.h"

#include "cmdline/ddcui_cmd_parser_aux.h"
#include "cmdline/ddcui_parsed_cmd.h"

#include "ddcui_cmd_parser.h"


// Variables used by callback function
static DDCA_Stats_Type  stats_work = DDCA_STATS_NONE;
// static char *           maxtrywork = NULL;

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


// signature GOptionParseFunc()
gboolean pre_parse_hook(
      GOptionContext * context,
      GOptionGroup * group,
      gpointer data,
      GError** error)
{
   // printf("(%s) data = %d\n", __func__, GPOINTER_TO_INT(data));
   return true;
}

gboolean post_parse_hook(
      GOptionContext * context,
      GOptionGroup * group,
      gpointer data,
      GError** error)
{
   // printf("(%s) data = %d\n", __func__, GPOINTER_TO_INT(data));
   return true;
}


bool parse_ddcui_syslog_level(
      const char *        sval,
      Ddcui_Syslog_Level* result_loc)
{
   bool debug = false;
   bool parsing_ok = true;
   if (debug)
      printf("(%s) sval=|%s|\n", __func__, sval);

#ifdef REF
   VNT(DDCA_SYSLOG_DEBUG,   "DEBUG"),
   VNT(DDCA_SYSLOG_VERBOSE, "VERBOSE"),
   VNT(DDCA_SYSLOG_INFO,    "INFO"),
   VNT(DDCA_SYSLOG_NOTICE,  "NOTICE"),
   VNT(DDCA_SYSLOG_WARNING, "WARN"),
   VNT(DDCA_SYSLOG_ERROR,   "ERROR"),
   VNT(DDCA_SYSLOG_NEVER,   "NEVER"),
#endif

   *result_loc = ddca_syslog_level_from_name(sval);
   if (*result_loc == DDCA_SYSLOG_NOT_SET) {
      parsing_ok = false;
      // emit_parser_error(errmsgs, __func__, "Invalid syslog level: %s", sval );
      // emit_parser_error(errmsgs, __func__, "Valid values are NEVER, ERROR, WARN, INFO, DEBUG");
      fprintf(stderr, "Invalid syslog level: %s\n", sval );
      fprintf(stderr, "Valid values are NEVER, ERROR, WARN, NOTICE, INFO, VERBOSE, DEBUG\n");
   }
   if (debug)
      printf("(%s) Returning %s, *result_loc = %d\n",
            __func__, sbool(parsing_ok), *result_loc);
   return parsing_ok;
}



/** Primary parsing function
 *
 *  \param  argc      number of command line arguments
 *  \param  argv      array of pointers to command line arguments
 *  \return pointer to a Parsed_Cmd struct if parsing successful
 *          NULL if execution should be terminated
 */
Parsed_Ddcui_Cmd * parse_ddcui_command(int argc, char * argv[]) {
   bool debug = false;

   char * s = getenv("DDCUTIL_DEBUG_PARSE");
   if (s && strlen(s) > 0)
      debug = true;

   if (debug) {
      printf("(%s) Starting\n", __func__ );

      printf("(%s) argc=%d\n", __func__, argc);
      for (int ndx=0; ndx < argc; ndx++) {
         printf("(%s) argv[%d] = |%s|\n", __func__, ndx, argv[ndx]);
      }
      assert(argc == ntsa_length(argv));

      printf("(%s) prgname = %s, application_name = %s\n",
             __func__, g_get_prgname(), g_get_application_name() );
   }

   Parsed_Ddcui_Cmd * parsed_cmd = new_parsed_ddcui_cmd();

// gboolean ddc_flag                       = false;
// gboolean nousb_flag                     = false;
// gboolean report_freed_excp_flag         = false;
   gboolean timestamp_trace_flag           = false;
   gboolean thread_id_trace_flag           = false;
   gboolean version_flag                   = false;
   gboolean show_styles_flag               = false;
   gboolean show_active_style_flag         = false;
// gboolean use_latest_nc_values_true_set  = false;
// gboolean use_latest_nc_values_false_set = false;
   gboolean hidpi_flag                     = false;   //currently used only for testing
   gchar**  cmd_and_args                   = NULL;

   DDCA_Syslog_Level syslog_level = DDCA_SYSLOG_INFO;
   char *   syslog_work     = NULL;
   gboolean disable_config_file            = false;
// gboolean disable_libddcutil_config_file = false;
   gboolean trace_to_syslog_flag           = false;   //???


#ifdef DISABLE_VIEW_OPTION
   gchar*   view_work                      = NULL;
#endif
   gchar*   nc_values_source_work          = NULL;
   gchar*   feature_set_work               = NULL;
   gchar*   custom_feature_set_work        = NULL;
   gboolean control_key_required           = false;
   gboolean show_unsupported_features      = false;
   gchar**  libopts_pieces                 = NULL;

   gboolean only_capabilities_true_set     = false;
// gboolean only_capabilities_false_set    = false;
   gboolean all_capabilities_true_set      = false;
// gboolean all_capabilities_false_set     = false;

   gboolean force_slave_address_true_set   = false;
   gboolean force_slave_address_false_set  = false;

   gboolean debug_parse                    = false;
   gboolean parse_only                     = false;

   gboolean f1_flag        = false;
   gboolean f2_flag        = false;
   gboolean f3_flag        = false;
   gboolean f4_flag        = false;
   gboolean f5_flag        = false;
   gboolean f6_flag        = false;

   GOptionEntry public_option_entries[] = {
   //  long_name short flags  option-type            gpointer           description                    arg description
   //  monitor selection options

   // Options dialog

#ifdef DISABLE_VIEW_OPTION
      {"view",      '\0',  0, G_OPTION_ARG_STRING,   &view_work,             "Initial view",             "Summary|Capabilities|Features"},
#endif
      {"require-control-key",
                   '\0',   0, G_OPTION_ARG_NONE,     &control_key_required,  "Control key must be pressed to move slider", NULL},
      {"nc-values-source",
                   '\0',   0, G_OPTION_ARG_STRING,   &nc_values_source_work, "Initial NC values source",   "MMCS|Capabilities|Both"},

      {"feature-set",
                   '\0',   0, G_OPTION_ARG_STRING,   &feature_set_work,      "Feature set selection",
                                                                                   "MMCS|Capabilities|Manufacturer|Color|Scan"},
//    {"use-latest-nc-values",
//                 '\0',   0, G_OPTION_ARG_NONE,     &use_latest_nc_values_true_set, "Use NC values from the latest MCCS version", NULL},

      {"custom-feature-set",
                   '\0',   0, G_OPTION_ARG_STRING,    &custom_feature_set_work, "User feature set definition",
                                                                    "comma separated list of feature codes"},
      {"show-unsupported",
                   '\0',   0, G_OPTION_ARG_NONE,     &show_unsupported_features, "Show unsupported features", NULL},
      {"only-capabilities",
                   '\0',   0, G_OPTION_ARG_NONE,     &only_capabilities_true_set, "Include only values in capabilities", NULL},
//    {"not-only-capabilities",
//                 '\0',  0, G_OPTION_ARG_NONE,      &only_capabilities_false_set, "Do not exclude values only in MCCS", NULL},
      {"all-capabilities",
                   '\0',   0, G_OPTION_ARG_NONE,     &all_capabilities_true_set, "Include all values in capabilities", NULL},
//    {"not-all-capabilities",
//                 '\0', 0, G_OPTION_ARG_NONE,       &all_capabilities_false_set, "Negate include all values in capabilities", NULL},

// Display selection
      {"model",    '\0',   0, G_OPTION_ARG_STRING,   &parsed_cmd->model,                "Model of default display",              NULL},
//    {"bus",      '\0',   0, G_OPTION_ARG_INT,      &parsed_cmd->busno,    "I2C bus number",                        "integer"},
#ifdef DISABLED_LIBDDCUTIL_ONLY
      {"nousb",    '\0',   0, G_OPTION_ARG_NONE,     &nousb_flag,       "Do not detect USB devices", NULL},
#endif

// Tuning
      {"stats",    's',  G_OPTION_FLAG_OPTIONAL_ARG,
                            G_OPTION_ARG_CALLBACK, stats_arg_func,    "Show performance statistics",  "TRIES|ERRORS|CALLS|ALL"},
//    {"ddc",      '\0', 0, G_OPTION_ARG_NONE,     &ddc_flag,         "Report DDC protocol and data errors", NULL},
      {"force-slave-address",
                   '\0', 0, G_OPTION_ARG_NONE,     &force_slave_address_true_set, "Deprecated", NULL},
      {"disable-force-slave-address",
                   '\0', 0, G_OPTION_ARG_NONE,     &force_slave_address_false_set, "Deprecated", NULL},

// Pre-GUI queries
      {"styles",   '\0', 0, G_OPTION_ARG_NONE,     &show_styles_flag,            "List known styles",   NULL},
      {"show-style",'\0',0, G_OPTION_ARG_NONE,     &show_active_style_flag,      "Show active style",   NULL},

// libddcutil options
      {"libopts",  '\0', 0, G_OPTION_ARG_STRING_ARRAY, &libopts_pieces,  "libddcutil options",  "string"},

// output control
      {"disable-config-file",
                   '\0', 0, G_OPTION_ARG_NONE,     &disable_config_file, "Ignore ddcutil config file", NULL},
      {"noconfig", '\0', 0, G_OPTION_ARG_NONE,     &disable_config_file, "Ignore ddcutil config file", NULL},

      {"syslog",   '\0', 0, G_OPTION_ARG_STRING,   &syslog_work,                    "system log level", "NONE, ERROR, WARN, INFO, NONE"},

      {"version",  'V',  0, G_OPTION_ARG_NONE,     &version_flag,         "Show version information",   NULL},
      { NULL }
   };

   GOptionEntry development_options[] = {
// debugging
//    {"excp",     '\0',   0, G_OPTION_ARG_NONE,   &report_freed_excp_flag, "Report freed exceptions", NULL},
      {"timestamp",'\0',   0, G_OPTION_ARG_NONE,   &timestamp_trace_flag, "Prepend trace msgs with elapsed time",  NULL},
      {"ts",       '\0',   0, G_OPTION_ARG_NONE,   &timestamp_trace_flag, "Prepend trace msgs with elapsed time",  NULL},
      {"thread-id",'\0',   0, G_OPTION_ARG_NONE,   &thread_id_trace_flag, "Prepend trace msgs with thread id",     NULL},
      {"tid",      '\0',   0, G_OPTION_ARG_NONE,   &thread_id_trace_flag, "Prepend trace msgs with thread id",     NULL},

// Debug parser
      {"debug-parse", '\0', 0, G_OPTION_ARG_NONE,  &debug_parse,          "Show parse result",        NULL},
      {"parse-only",  '\0', 0, G_OPTION_ARG_NONE,  &parse_only,           "Exit after parsing",       NULL},

// Other
      {"hidpi",    '\0',   0, G_OPTION_ARG_NONE,  &hidpi_flag,            "Test hidpi code", NULL},

// Undocumented library flag options
      {"f1",      '\0', 0,  G_OPTION_ARG_NONE,     &f1_flag,              "Special flag 1",    NULL},
      {"f2",      '\0', 0,  G_OPTION_ARG_NONE,     &f2_flag,              "Special flag 2",    NULL},
      {"f3",      '\0', 0,  G_OPTION_ARG_NONE,     &f3_flag,              "Special flag 3",    NULL},
      {"f4",      '\0', 0,  G_OPTION_ARG_NONE,     &f4_flag,              "Special flag 4",    NULL},
      {"f5",      '\0', 0,  G_OPTION_ARG_NONE,     &f5_flag,              "Special flag 5",    NULL},
      {"f6",      '\0', 0,  G_OPTION_ARG_NONE,     &f6_flag,              "Special flag 6",    NULL},
      {"i1",      '\0', 0,  G_OPTION_ARG_INT,      &parsed_cmd->i1,       "Special integer 1", NULL},
      {"i2",      '\0', 0,  G_OPTION_ARG_INT,      &parsed_cmd->i2,       "Special integer 2", NULL},

  //    {G_OPTION_REMAINING,
  //                    '\0', 0,  G_OPTION_ARG_STRING_ARRAY, &cmd_and_args, NULL,   NULL},

      { NULL }
   };

#ifdef DOESNT_WORK
   GOptionEntry final_options[] = {
     // collect to verify that does not exist
      {G_OPTION_REMAINING,
                      '\0', 0,  G_OPTION_ARG_STRING_ARRAY, &cmd_and_args, NULL,   NULL},

      // if there were arguments:
      // {G_OPTION_REMAINING,
      //            '\0', 0,  G_OPTION_ARG_STRING_ARRAY, &cmd_and_args, "ARGUMENTS description",   "command [arguments...]"},

      { NULL }
   };
#endif

   GOptionGroup * development_option_group =
   g_option_group_new("development", "Development related options", "Not for general use", GINT_TO_POINTER(3), NULL);
   g_option_group_add_entries(development_option_group, development_options);
   g_option_group_set_parse_hooks(development_option_group, pre_parse_hook, post_parse_hook);

#ifdef DOESNT_WORK
   GOptionGroup * remainder_group =
   g_option_group_new("remainder", "Command sink", "Should be empty", NULL, NULL);
   g_option_group_add_entries(remainder_group, final_options);
#endif

   GError* error = NULL;
   GOptionContext* context = g_option_context_new("- graphical interface to ddcutil");
   g_option_context_add_main_entries(context, public_option_entries, NULL);
   g_option_context_add_group(context, development_option_group);
#ifdef DOESNT_WORK
   g_option_context_add_group(context, remainder_group);
#endif

   g_option_context_set_help_enabled(context, true);
   // bool ok = false;

   char * help_description = g_strjoin("\n",
         tracing_multiple_call_option_help,
         trcfunc_multiple_call_option_help,
         trcfile_multiple_call_option_help,
         stats_multiple_call_option_help,
         NULL
         );

   // on --help, comes at end after option detail
   // g_option_context_set_description(context, help_description);
   free(help_description);

   char ** mangleable_argv = argv;
   if (debug) {
      printf("(%s) Before g_option_context_parse_strv(), mangleable_argv:\n", __func__);
      rpt_ntsa(mangleable_argv, 3);
   }
   char * combined = strjoin((const char**)(mangleable_argv+1), ntsa_length(mangleable_argv)-1, " ");
   printf("ddcui: Applying combined options: %s\n", combined);
   syslog(LOG_NOTICE,"Applying combined ddcui options: %s",   combined);
   free(combined);
   bool ok = g_option_context_parse_strv(context, &mangleable_argv, &error);
   if (!ok) {
      if (error) {
         fprintf(stderr,  "ddcui option parsing failed: %s\n", error->message);
         syslog(LOG_CRIT, "ddcui option parsing failed: %s",   error->message);
         g_error_free(error);
      }
      else {
         fprintf(stderr,  "ddcui option parsing failed.\n");
         syslog(LOG_CRIT, "ddcui option parsing failed.");
      }
   }

   if (ntsa_length(mangleable_argv) > 1) {
      char * remainder = strjoin((const char**)(mangleable_argv+1), ntsa_length(mangleable_argv)-1, " ");
      fprintf(stderr, "Unrecognized: %s\n", remainder);
      syslog(LOG_CRIT, "Unrecognized: %s\n", remainder);
      free(remainder);
      ok = false;
   }

   if (false) {
      printf("(%s) Freeing mangleable_argv=%p:\n", __func__, (void*)mangleable_argv);
      rpt_ntsa(mangleable_argv, 3);
   }
   ntsa_free(mangleable_argv, true);

   if (force_slave_address_true_set) {
      char * s = "Deprecated option ignored: --force_slave_address";
      fprintf(stderr,     "%s\n", s);
      syslog(LOG_WARNING, "%s",   s);
   }
   if (force_slave_address_false_set) {
      char * s = "Deprecated option ignored: --disable-force_slave_address";
      fprintf(stderr,     "%s\n", s);
      syslog(LOG_WARNING, "%s",   s);
   }

   if (libopts_pieces) {
      char * combined_libopts_pieces = strjoin((const char**)libopts_pieces, ntsa_length(libopts_pieces), " ");
      parsed_cmd->library_options = combined_libopts_pieces;
      // printf("Combined_libopts_pieces: |%s|\n", combined_libopts_pieces);
   }

#define SET_CMDFLAG(_bit, _flag) \
   do { \
      if (_flag) \
         parsed_cmd->flags |= _bit; \
   } while(0)

   parsed_cmd->stats_types      = stats_work;
// SET_CMDFLAG(CMD_FLAG_DDCDATA,           ddc_flag);
   SET_CMDFLAG(CMD_FLAG_TIMESTAMP_TRACE,   timestamp_trace_flag);
   SET_CMDFLAG(CMD_FLAG_THREAD_ID_TRACE,   thread_id_trace_flag);
// SET_CMDFLAG(CMD_FLAG_REPORT_FREED_EXCP, report_freed_excp_flag);

// SET_CMDFLAG(CMD_FLAG_NOUSB,             nousb_flag);

   SET_CMDFLAG(CMD_FLAG_SHOW_STYLES,       show_styles_flag);
   SET_CMDFLAG(CMD_FLAG_SHOW_ACTIVE_STYLE, show_active_style_flag);

   SET_CMDFLAG(CMD_FLAG_UI_REQUIRE_CONTROL_KEY, control_key_required);
   SET_CMDFLAG(CMD_FLAG_SHOW_UNSUPPORTED,       show_unsupported_features);
// SET_CMDFLAG(CMD_FLAG_LATEST_NC_VALUE_NAMES,  use_latest_nc_values_true_set);   // n. not handling case where default is true
   SET_CMDFLAG(CMD_FLAG_DISABLE_CONFIG_FILE, disable_config_file);

   SET_CMDFLAG(CMD_FLAG_F1,                f1_flag);
   SET_CMDFLAG(CMD_FLAG_F2,                f2_flag);
   SET_CMDFLAG(CMD_FLAG_F3,                f3_flag);
   SET_CMDFLAG(CMD_FLAG_F4,                f4_flag);
   SET_CMDFLAG(CMD_FLAG_F5,                f5_flag);
   SET_CMDFLAG(CMD_FLAG_F6,                f6_flag);

   SET_CMDFLAG(CMD_FLAG_HIDPI,             hidpi_flag);
#undef SET_CMDFLAG

   if (syslog_work) {
      Ddcui_Syslog_Level level;
      bool this_ok = parse_ddcui_syslog_level(syslog_work, &level);
      // printf("(%s) this_ok = %s\n", __func__, sbool(this_ok));
      if (this_ok)
         syslog_level = level;
      else
         ok = false;
   }
   parsed_cmd->syslog_level = syslog_level;


   if (all_capabilities_true_set)
      parsed_cmd->include_all_capabilities_features = TRIVAL_TRUE;
#ifdef TRIVAL
   else if (all_capabilities_false_set)
      parsed_cmd->include_all_capabilities_features = TRIVAL_FALSE;
#endif
   else
      parsed_cmd->include_all_capabilities_features = TRIVAL_UNSET;


   if (only_capabilities_true_set)
      parsed_cmd->include_only_capabilities_features = TRIVAL_TRUE;
#ifdef TRIVAL
   else if (only_capabilities_false_set)
      parsed_cmd->include_only_capabilities_features = TRIVAL_FALSE;
#endif
   else
      parsed_cmd->include_only_capabilities_features = TRIVAL_UNSET;


   if (custom_feature_set_work) {
      char ** error_msgs;         // Null_Terminated_String_Array error_msgs;
      DDCA_Feature_List flist = parse_custom_feature_list(custom_feature_set_work, &error_msgs);
      parsed_cmd->custom_feature_list = flist;
      if (ddca_feature_list_count(flist) == 0) {
         ok = false;
         if (error_msgs) {
            char * s = "Errors in --custom-feature-set:";
            fprintf(stderr, "%s\n", s);
            syslog(LOG_CRIT, "%s", s);
            for (int ndx = 0; error_msgs[ndx]; ndx++) {
               fprintf(stderr,  "   %s\n", error_msgs[ndx]);
               syslog(LOG_CRIT, "   %s",   error_msgs[ndx]);
            }
            ntsa_free(error_msgs, true);
         }
         else {
            char * s = "Empty --custom-feature-set";
            fprintf(stderr,  "%s\n", s);
            syslog(LOG_CRIT, "%s",   s);
         }
      }
   }

#ifdef DISABLE_VIEW_OPTION
      if (view_work) {
         // printf("view_work = %p -> |%s|\n", view_work, view_work);
         Parsed_View pv = find_view_table_value(view_work);
         if (pv == VIEW_UNSET) {
            fprintf(stderr, "Unrecognized: %s\n", view_work);
            ok = false;
         }
         else{
            parsed_cmd->view = pv;
         }
      }
#endif

      if (nc_values_source_work) {
         // printf("nc_values_source_work = %p -> |%s|\n", nc_values_source_work, nc_values_source_work);
         Parsed_NC_Values_Source src = find_nc_values_source_table_value(nc_values_source_work);
         if (src == NC_VALUES_SOURCE_UNSET) {
            char * s = "Unrecognized: ";
            fprintf(stderr, "%s%s\n", s, nc_values_source_work);
            fprintf(stderr, "%s%s",   s, nc_values_source_work);
            ok = false;
         }
         else{
            parsed_cmd->nc_values_source = src;
         }
      }

// Does this macro make code cleaner or more obscure?
#define VALUE_LOOKUP(_ENUM, _NAME, _NOT_FOUND_VALUE) \
      if (_NAME ## _work) {                          \
         _ENUM src = find_ ## _NAME ##_table_value(_NAME ## _work);            \
         if (src == _NOT_FOUND_VALUE) {                                        \
            fprintf(stderr,  "Unrecognized: %s\n", _NAME ## _work);            \
            syslog(LOG_CRIT, "Unrecognized: %s",   _NAME ## _work);            \
            ok = false;                                                        \
         }                                                                     \
         else{                                                                 \
            parsed_cmd->_NAME = src;                                           \
         }                                                                     \
      }

      VALUE_LOOKUP(Parsed_Feature_Set, feature_set, FS_UNSET);

#undef VALUE_LOOKUP

      if (parsed_cmd->feature_set != FS_UNSET &&
          ddca_feature_list_count(parsed_cmd->custom_feature_list) > 0)
      {
         char * s = "--feature-set and --custom-feature-set are mutually exclusive";
         fprintf(stderr,  "%s\n", s);
         syslog(LOG_CRIT, "%s",   s);
         ok = false;
      }

      if (all_capabilities_true_set && only_capabilities_true_set) {
         if (parsed_cmd->feature_set != FS_CAPABILITIES) {
            char * s = "--all-capabilities and --only-capabilities are mutually exclusive"
                            " except when --feature-set = CAPABILITIES";
            fprintf(stderr,  "%s\n", s);
            syslog(LOG_CRIT, "%s",   s);
            ok = false;
         }
      }


      if (cmd_and_args && cmd_and_args[0]) {
         // int rest_ct = 0;   // unused
         // don't pull debug into the if clause, need rest_ct to be set
         // if (cmd_and_args) {
         //    for (; cmd_and_args[rest_ct] != NULL; rest_ct++) {
         //          DBGMSF(debug, "cmd_and_args[%d]: %s", rest_ct, cmd_and_args[rest_ct]);
         //    }
         // }

         char * s = g_strjoinv(" ",cmd_and_args);
         fprintf(stderr,  "Unrecognized: %s\n", s);
         syslog(LOG_CRIT, "Unrecognized: %s",   s);
         free(s);
         ok = false;
      }

   if (version_flag) {
   }

   // DBGMSF(debug, "Calling g_option_context_free(), context=%p...", context);
   g_option_context_free(context);

   if (debug || debug_parse) {
      dbgrpt_parsed_ddcui_cmd(parsed_cmd);
   }

   if (!ok || parse_only) {
      free_parsed_ddcui_cmd(parsed_cmd);
      parsed_cmd = NULL;
   }

   if (debug)
      printf("(%s) ddcui parser returning: %p\n", __func__, (void*) parsed_cmd);
   return parsed_cmd;
}
