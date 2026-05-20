/** @file ddcui_trace_control.h
 *
 *  Manage whether tracing is performed for ddcui Qt C++ methods and files.
 */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_TRACE_CONTROL_H_
#define DDCUI_TRACE_CONTROL_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void add_traced_method(      const char * methodname);
bool is_traced_method(       const char * methodname);
void dbgrpt_traced_method_table(int depth);

void add_traced_class(       const char * classname);
bool is_traced_class(        const char * classname);
void dbgrpt_traced_class_table(int depth);

void add_traced_metaclass(   const char * classname);
bool is_traced_metaclass(    const char * classname);
void dbgrpt_traced_metaclass_table(int depth);

void add_traced_file(        const char * filename);
bool is_traced_file(         const char * filename);
void dbgrpt_traced_file_table(int depth);

#ifdef __cplusplus
}
#endif

#endif /* DDCUI_TRACE_CONTROL_H_ */
