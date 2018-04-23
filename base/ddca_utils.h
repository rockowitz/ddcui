/* ddca_utils.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef DDCA_UTILS_H_
#define DDCA_UTILS_H_

#include <ddcutil_types.h>

typedef enum {
   CapsOnly,
   CapsPlusMccs
} Nc_Values_Merge_Mode;

DDCA_Feature_Value_Table
ddcutil_merge_feature_values(
      DDCA_Cap_Vcp *             capabilities_feature_record,
      DDCA_Feature_Value_Table   mccs_table,
      Nc_Values_Merge_Mode       merge_mode);

DDCA_Cap_Vcp *
ddcutil_find_cap_vcp(
      DDCA_Capabilities * parsed_caps,
      uint8_t feature_code);


void
free_ddca_feature_value_table(
      DDCA_Feature_Value_Table * table);

#endif /* DDCA_UTILS_H_ */
