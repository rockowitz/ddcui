/** @file ddcui_rtti.h
 * Runtime trace information
 */

// Copyright (C) 2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_RTTI_H_
#define DDCUI_RTTI_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RTTI_ADD_METHOD(_NAME) rtti_method_name_table_add(#_NAME);

void   rtti_method_name_table_add(const char * method_name);
bool   rtti_method_name_table_contains(const char * method_name);
bool   rtti_class_name_table_contains(const char * class_name);
bool   rtti_metaclass_name_table_contains(const char * metaclass_name);
void   dbgrpt_rtti_method_name_table(int depth, bool show_internal);
void   report_rtti_method_name_table(int depth, const char * msg);
void   dbgrpt_rtti_class_name_table(int depth, bool show_internal);
void   report_rtti_class_name_table(int depth, const char * msg);
void   dbgrpt_rtti_metaclass_name_table(int depth, bool show_internal);
void   report_rtti_metaclass_name_table(int depth, const char * msg);
void   terminate_rtti();

#ifdef __cplusplus
}
#endif

#endif /* DDCUI_RTTI_H_ */
