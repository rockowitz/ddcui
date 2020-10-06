/** @file coredefs.h
 *  Basic definitions - Not application specific
 */

// Copyright (C) 2017-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef COREDEFS_H_
#define COREDEFS_H_

/** Raw byte
 */
typedef unsigned char Byte;

#ifndef ARRAY_SIZE
/** Number of entries in array
 */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#endif /* COREDEFS_H_ */
