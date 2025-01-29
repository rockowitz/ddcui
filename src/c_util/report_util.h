/** @file report_util.h
 *
 *  Report utility package
 */

// Copyright (C) 2014-2025 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef REPORT_UTIL_H_
#define REPORT_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \cond */
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdbool.h>
/** \endcond */

#include "coredefs_base.h"
#include "string_util.h"

#ifdef PERHAPS
#define RPT_PREFIX_NONE   0x00
#define RPT_PREFIX_PID    0x01
#define RPT_PREFIX_TID    0x02
#define RPT_PREFIX_TS     0x04
#define RPT_PREFIX_WTD    0x08

extern __thread Byte rpt_prefix_options;
#endif

// If set, report messages are written to the system log, not the terminal
extern bool redirect_reports_to_syslog;
// If set, report messages have a letter tag appended, as a debugging aid
// for identifying message source location.
extern bool tag_output;

void rpt_set_default_ornamentation_enabled(bool onoff);
bool rpt_get_ornamentation_enabled();
bool rpt_set_ornamentation_enabled(bool enabled);

void rpt_set_default_output_dest(FILE* output_dest);
void rpt_push_output_dest(FILE* new_dest);
void rpt_pop_output_dest();
FILE * rpt_cur_output_dest();
void rpt_reset_output_dest_stack();
void rpt_change_output_dest(FILE* new_dest);
void rpt_debug_output_dest();

int rpt_get_indent(int depth);

void rpt_flush();

void rpt_nl();
void rpt_title(const char * title, int depth);
void rpt_label(int depth, const char * text);
void rpt_label_collect(int depth, GPtrArray* collector, const char * text);
void rpt_multiline(int depth, ...);
void rpt_g_ptr_array(int depth, GPtrArray * strings);

void rpt_vstring(int depth, char * format, ...) ;
void rpt_vstring_collect(int depth, GPtrArray* collector, char * format, ...);
void rpt_2col(char * s1,  char * s2,  int col2offset, bool offset_absolute, int depth);
void rpt_structure_loc(const char * name, const void * ptr, int depth);
void rpt_hex_dump(const Byte * data, int size, int depth);
void rpt_ntsa(Null_Terminated_String_Array ntsa, int depth);
int rpt_file_contents(const char * fn, bool verbose, int depth);

// Remaining rpt_ functions share common formatting
void rpt_str(const char * name, char * info, const char * val, int depth);
void rpt_int(char * name, char * info, int val, int depth);
void rpt_unsigned(char * name, char * info, int val, int depth);
void rpt_bool(char * name, char * info, bool val, int depth);

/** Interpretation function used by rpt_mapped_int() */
typedef char * (*Value_To_Name_Function)(int val);
void rpt_mapped_int(char * name, char * info, int val, Value_To_Name_Function func, int depth);

void rpt_int_as_hex(char * name, char * info, int val, int depth);
void rpt_uint8_as_hex(char * name, char * info, unsigned char val, int depth) ;
void rpt_bytes_as_hex(const char * name, char * info, Byte * bytes, int ct, bool hex_prefix_flag, int depth);

typedef
struct {
   char * flag_name;
   char * flag_info;
   int    flag_val;
} Flag_Info;

typedef
struct {
   int         flag_info_ct;
   Flag_Info * flag_info_recs;
} Flag_Dictionary;

typedef
struct {
   int     flag_name_ct;
   char ** flag_names;
} Flag_Name_Set;

void rpt_ifval2(char * name,
                char * info,
                int    val,
                Flag_Name_Set *   pflagNameSet,
                Flag_Dictionary * pDict,
                int    depth);

#ifdef __cplusplus
}    // extern "C"
#endif

#endif /* REPORT_UTIL_H_ */
