// feature_list.cpp

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

 
#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ddcutil_types.h>
#include <ddcutil_c_api.h>

#include "c_util/string_util.h"
#include "base/ddcui_parms.h"
#include "base/feature_list.h"





DDCA_Feature_List parse_custom_feature_list(
      char * features_work,
//    char ** error_msgs_loc)
      Null_Terminated_String_Array * error_msgs_loc)
{
    assert(features_work);
    assert(error_msgs_loc);
    bool debug = true;
    DDCA_Feature_List feature_list = DDCA_EMPTY_FEATURE_LIST;

    printf("features_work: %s\n", features_work);
    char * x = features_work;
    while (*x) {
       if (*x == ',')
          *x = ' ';
       x++;
    }
    printf("features_work: %s\n", features_work);

    if (debug)
       printf("features_work = |%s|\n", features_work );

    // GArray * errors = g_array_new(true,      // extra 0 element at end
    //                              false,     // set new elements to 0
    //                              sizeof(char*) );
    GPtrArray * errors = g_ptr_array_new();

    Null_Terminated_String_Array pieces = strsplit(features_work, " ");
    // gchar ** pieces = g_strsplit(features_work, " ", -1);
    int ntsal = ntsa_length(pieces);
    // int ntsal = g_strv_length(pieces);
    if (debug)
       printf("ntsal=%d\n", ntsal );
    if (ntsal == 0) {
       fprintf(stderr, "no features specified for --custom_features\n");
    }
    else {
       int ok = true;
       int ndx = 0;
       for (; pieces[ndx] != NULL; ndx++) {
           // char * token = strtrim_r(pieces[ndx], trimmed_piece, 10);
           char * token = g_strstrip(pieces[ndx]);
           printf("(parse_features_list) token= |%s|\n", token);
           Byte feature_code = 0;
           if ( any_one_byte_hex_string_to_byte_in_buf(token, &feature_code) ) {
              ddca_feature_list_add(&feature_list, feature_code);
           }
           else {
              fprintf(stderr, "Invalid feature code in --custom_features: %s\n", token);
              char * s = g_strdup_printf("Invalid feature code: %s", token);
              g_ptr_array_add(errors, s);
              // g_array_append_val(errors, s);

              ok = false;
           }
       }
       assert(ndx == ntsal);
       ntsa_free(pieces, /* free_strings */ true);
       // g_strfreev(pieces);

       ASSERT_IFF(ok, errors->len == 0);

       if (ok) {
          // g_array_free(errors, true);

          g_ptr_array_free(errors,true);
          *error_msgs_loc = NULL;
       }
       else {
          feature_list = DDCA_EMPTY_FEATURE_LIST;
          // *error_msgs_loc = errors->data;
          // g_array_free(errors, false);

          *error_msgs_loc = g_ptr_array_to_ntsa(errors, false);
          g_ptr_array_free(errors, false);
       }
     }

    if (debug) {
       const char * prefix = "x";
       const char * sepstr = ",";
       const char * s =    ddca_feature_list_string(&feature_list, prefix, sepstr);
       printf("Returning feature list: %s\n", s);
    }
    return feature_list;
}
