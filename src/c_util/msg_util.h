/** @file msg_util.h */

// Copyright (C) 2024-2025 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MSG_UTIL_H_
#define MSG_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <glib-2.0/glib.h>
#include <stdbool.h>

#define PRItid "[%6jd]"

extern bool dbgtrc_show_time;       // prefix debug/trace messages with elapsed time
extern bool dbgtrc_show_wall_time;  // prefix debug/trace messages with wall time
extern bool dbgtrc_show_thread_id;  // prefix debug/trace messages with thread id
extern bool dbgtrc_show_process_id; // prefix debug/trace messages with process id
extern bool dbgtrc_trace_to_syslog_only;
extern bool stdout_stderr_redirected;

extern __thread bool msg_decoration_suspended;

char*      get_msg_decoration(char * buf, uint bufsize, bool dest_syslog);
char*      formatted_wall_time();

#ifdef __cplusplus
}
#endif

#endif /* MSG_UTIL_H_ */
