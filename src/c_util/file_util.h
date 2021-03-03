// file_util.h

// Copyright (C) 2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <glib-2.0/glib.h>

#ifdef __cplusplus
extern "C" {
#endif

int
file_getlines(
      const char * fn,
      GPtrArray*   line_array,
      bool         verbose)
;
#ifdef __cplusplus
}    // extern "C"
#endif

#endif /* FILE_UTIL_H_ */
