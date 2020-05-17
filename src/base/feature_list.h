// feature_list.h

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ddcutil_types.h"

#include "c_util/string_util.h"

#ifndef FEATURE_LIST_H_
#define FEATURE_LIST_H_


#define ASSERT_IFF(_cond1, _cond2) \
   assert( ( (_cond1) && (_cond2) ) || ( !(_cond1) && !(_cond2) ) )

#ifdef __cplusplus
extern "C"
{
#endif

DDCA_Feature_List parse_custom_feature_list(
      char * features_work,
      Null_Terminated_String_Array * error_msgs_loc);

#ifdef __cplusplus
}
#endif

#endif /* FEATURE_LIST_H_ */
