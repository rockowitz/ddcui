// data_structures.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

 

#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

/** \cond */
#include <stdbool.h>
#include <stdint.h>
/** \endcond */


// Identifier id to name and description lookup
//

/** \def VN(v)
 * Creates a Value_Name table entry by specifying its symbolic name.
 *
 *  @param v symbolic name
 */
#define VN(v) {v,#v,NULL}
/** \def VN_END
 * Terminating entry for a Value_Name table. */
#define VN_END {0xff,NULL,NULL}

/** \def VNT(v,t)
 *  Creates a Value_Name_Title table entry by specifying its symbolic name
 *  and description
 *
 *  @param v symbolic name
 *  @param t symbol description
 */
#define VNT(v,t) {v,#v,t}
/** Terminating entry for a Value_Name_Title table. */
#define VNT_END {0xff,NULL,NULL}

/** A Value_Name_Title table is used to map byte values to their
 * symbolic names and description (title).
 * Each entry is a value/name/description triple..
 *
 * The table is terminated by an entry whose name and description fields are NULL.
 */
typedef struct {
   uint32_t value;       ///< value
   char *   name;        ///< symbolic name
   char *   title;       ///< value description
} Value_Name_Title;

typedef Value_Name_Title Value_Name_Title_Table[];

typedef Value_Name_Title       Value_Name;
typedef Value_Name_Title_Table Value_Name_Table;


char * vnt_name( Value_Name_Title* table, uint32_t val);
#ifdef TRANSITIONAL
#define vn_name vnt_name
#endif
char * vnt_title(Value_Name_Title* table, uint32_t val);

uint32_t vnt_find_id(
           Value_Name_Title_Table table,
           const char * s,
           bool use_title,       // if false, search by symbolic name, if true, search by title
           bool ignore_case,
           uint32_t default_id);

#define INTERPRET_VNT_FLAGS_BY_NAME false
#define INTERPRET VNT_FLAGS_BY_TITLE true
char * vnt_interpret_flags(
      uint32_t                flags_val,
      Value_Name_Title_Table  bitname_table,
      bool                    use_title,
      char *                  sepstr);


#endif /* DATA_STRUCTURES_H_ */
