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
      char * unparsed_string,
      Null_Terminated_String_Array * error_msgs_loc)
{
    assert(unparsed_string);
    assert(error_msgs_loc);
    bool debug = false;
    if (debug)
       printf("(parse_custom_feature_list) unparsed_string = |%s|\n", unparsed_string );

    DDCA_Feature_List feature_list = DDCA_EMPTY_FEATURE_LIST;
    *error_msgs_loc = NULL;

    // convert all commas to blanks
    char * x = unparsed_string;
    while (*x) {
       if (*x == ',')
          *x = ' ';
       x++;
    }

    GPtrArray * errors = g_ptr_array_new();

    Null_Terminated_String_Array pieces = strsplit(unparsed_string, " ");
    // gchar ** pieces = g_strsplit(features_work, " ", -1); // doesn't handle multipler blanks
    int ntsal = ntsa_length(pieces);
    if (debug)
       printf("(parse_custom_feature_list) ntsal=%d\n", ntsal );
    if (ntsal == 0) {
       // char * s = "No custom features specified";
       // g_ptr_array_add(errors, s);
       // fprintf(stderr, "%s\n", s);
       if (debug)
          printf("(parse_custom_feature_list) Empty string\n");
    }
    else {
       bool ok = true;
       int ndx = 0;
       for (; pieces[ndx] != NULL; ndx++) {
           // char * token = strtrim_r(pieces[ndx], trimmed_piece, 10);
           char * token = g_strstrip(pieces[ndx]);
           // printf("(parse_features_list) token= |%s|\n", token);
           Byte feature_code = 0;
           if ( any_one_byte_hex_string_to_byte_in_buf(token, &feature_code) ) {
              ddca_feature_list_add(&feature_list, feature_code);
           }
           else {
              // fprintf(stderr, "Invalid feature code in --custom_features: %s\n", token);
              char * s = g_strdup_printf("Invalid feature code: %s", token);
              g_ptr_array_add(errors, s);
              ok = false;
           }
       }
       assert(ndx == ntsal);
       ntsa_free(pieces, /* free_strings */ true);

       ASSERT_IFF(ok, errors->len == 0);

       if (ok) {
          g_ptr_array_free(errors,true);
          *error_msgs_loc = NULL;
       }
       else {
          feature_list = DDCA_EMPTY_FEATURE_LIST;
          *error_msgs_loc = g_ptr_array_to_ntsa(errors, false);
          g_ptr_array_free(errors, false);
       }
     }

    if (debug) {
       const char * s = ddca_feature_list_string(feature_list, /*prefix*/ "x", /*sepstr*/ ",");
       printf("Returning feature list: %s\n", s);
       if (*error_msgs_loc) {
          printf("(parse_custom_feature_list) Returning error messages:\n");
          ntsa_show(*error_msgs_loc);
       }
    }
    return feature_list;
}
