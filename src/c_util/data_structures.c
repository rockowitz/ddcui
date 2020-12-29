// data_structures.c

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <string.h>

#include "coredefs.h"
#include "glib_util.h"
#include "data_structures.h"

#define sbool(_b)  ( (_b) ? "true" : "false" )
// const char * sbool(b) { return (b) ? "true" : "false"; }

//
// Identifier id to name and description lookup
//

/** Returns the name of an entry in a Value_Nmme_Title table.
 *
 * @param table  pointer to table
 * @param val    value to lookup
 *
 * @return name of value, NULL if not found
 */
char * vnt_name(Value_Name_Title* table, uint32_t val) {
   // printf("(%s) val=%d\n", __func__, val);
   // debug_vnt_table(table);
   char * result = NULL;

   Value_Name_Title * cur = table;
   for (; cur->name; cur++) {
      if (val == cur->value) {
         result = cur->name;
         break;
      }
   }
   return result;
}


/** Returns the title (description field) of an entry in a Value_Nmme_Title table.
 *
 * @param table  pointer to table
 * @param val    value to lookup
 *
 * @return title of value, NULL if not found
 */
char * vnt_title(Value_Name_Title* table, uint32_t val) {
   // printf("(%s) val=%d\n", __func__, val);
   // debug_vnt_table(table);
   char * result = NULL;

   Value_Name_Title * cur = table;
   for (; cur->name; cur++) {
      if (val == cur->value) {
         result = cur->title;
         break;
      }
   }
   return result;
}


/** Searches a Value_Name_Title_Table for a specified name or title,
 *  and returns its id value.
 *
 *  @param table a      Value_Name_Title table
 *  @param s            string to search for
 *  @param use_title    if false, search name  field\n
 *                      if true,  search title field
 *  @param ignore_case  if true, search is case-insensitive
 *  @param default_id   value to return if not found
 *
 *  @result value id
 */
uint32_t vnt_find_id(
           Value_Name_Title_Table table,
           const char * s,
           bool use_title,       // if false, search by symbolic name, if true, search by title
           bool ignore_case,
           uint32_t default_id)
{
   assert(s);
   uint32_t result = default_id;
   Value_Name_Title * cur = table;
   for (; cur->name; cur++) {
      char * comparand = (use_title) ? cur->title : cur->name;
      if (comparand) {
         int comprc = (ignore_case)
                         ? strcasecmp(s, comparand)
                         : strcmp(    s, comparand);
         if (comprc == 0) {
            result = cur->value;
            break;
         }
      }
   }
   return result;
}


/** Interprets an integer whose bits represent named flags.
 *
 * @param flags_val      value to interpret
 * @param bitname_table  pointer to Value_Name table
 * @param use_title      if **true**, use the **title** field of the table,\n
 *                       if **false**, use the **name** field of the table
 * @param sepstr         if non-NULL, separator string to insert between values
 *
 * @return newly allocated character string
 *
 * @remark
 * - It is the responsibility of the caller to free the returned string
 * - If a referenced **title** field is NULL, "missing" is used as the value
 */
char * vnt_interpret_flags(
      uint32_t                flags_val,
      Value_Name_Title_Table  bitname_table,
      bool                    use_title,
      char *                  sepstr)
{
   bool debug = false;
   if (debug)
      printf("(%s) Starting. flags_val=0x%08x, bitname_table=%p, use_title=%s, sepstr=|%s|\n",
             __func__, flags_val, (void*)bitname_table, sbool(use_title), sepstr);

   GString * sbuf = g_string_sized_new(200);
   bool first = true;
   Value_Name_Title * cur_entry = bitname_table;
     while (cur_entry->name) {
        if (debug)
           printf("(%s) cur_entry=%p, Comparing flags_val=0x%08x vs cur_entry->value = 0x%08x\n",
                  __func__, (void*)cur_entry, flags_val, cur_entry->value);
        if (!flags_val && cur_entry->value == flags_val) { // special value for no bit set
           char * sval = (use_title) ? cur_entry->title : cur_entry->name;
           if (!sval)
              sval = "missing";
           g_string_append(sbuf, sval);
           break;
        }
        if (flags_val & cur_entry->value) {
           if (first)
              first = false;
           else {
              if (sepstr) {
                 g_string_append(sbuf, sepstr);
              }
           }

           char * sval = (use_title) ? cur_entry->title : cur_entry->name;
           if (!sval) {
              sval = "missing";
           }
           g_string_append(sbuf, sval);
        }
        cur_entry++;
     }
     char * result = strdup(sbuf->str);
     g_string_free(sbuf, true);

     if (debug)
        printf("(%s) Done. Returning: |%s|\n", __func__, result);
     return result;

}


/** Shows the contents of a **Value_Name_Title table.
 *  Output is written to stdout.
 *
 * @param table pointer to table
 */
void vnt_debug_table(Value_Name_Title * table) {
   printf("Value_Name_Title table:\n");
   Value_Name_Title * cur = table;
   for (; cur->name; cur++) {
      printf("   %2d %-30s %s\n",  cur->value, cur->name, cur->title);
   }
}


//
// bs256 - Bit_Set_256
//
// A data structure containing 256 flags
//

const Bit_Set_256 EMPTY_BIT_SET_256 = {{0}};

Bit_Set_256 bs256_add(
    Bit_Set_256 flags,
    Byte val)
{
    bool debug = false;

    Bit_Set_256 result = flags;
    int flagndx   = val >> 3;
    int shiftct   = val & 0x07;
    Byte flagbit  = 0x01 << shiftct;
    if (debug)
       printf("(%s) val=0x%02x, flagndx=%d, shiftct=%d, flagbit=0x%02x\n",
              __func__, val, flagndx, shiftct, flagbit);
    result.bytes[flagndx] |= flagbit;

    if (debug) {
       char * bs1 = bs256_to_string(flags, "","");
       char * bs2 = bs256_to_string(result, "","");
       printf("(%s) bitstring=%s, value %d, returning: %s\n",
              __func__, bs1, val, bs2);
       // free( bs1);
       // free(bs2);
       // printf("(%s) wolf 3\n", __func__);
    }


    return result;
}


bool bs256_contains(
    Bit_Set_256 flags,
    Byte val)
{
    bool debug = false;

    int flagndx   = val >> 3;
    int shiftct   = val  & 0x07;
    Byte flagbit  = 0x01 << shiftct;
    // printf("(%s) val=0x%02x, flagndx=%d, shiftct=%d, flagbit=0x%02x\n",
    //        __func__, val, flagndx, shiftct, flagbit);
    bool result = flags.bytes[flagndx] & flagbit;
    if (debug) {
       printf("(%s) flags:\n   ",__func__);
       for (int ndx = 0; ndx < 32; ndx++) {
          printf("%02x", flags.bytes[ndx]);
       }
       printf("\n");
       printf("(%s)  bit %d, returning: %d\n",  __func__, val, result);
    }
    return result;
}


bool bs256_eq(
    Bit_Set_256 set1,
    Bit_Set_256 set2)
{
   return memcmp(&set1, &set2, 32) == 0;
}


Bit_Set_256 bs256_or(
   Bit_Set_256 set1,
   Bit_Set_256 set2)
{
   Bit_Set_256 result;
   for (int ndx = 0; ndx < 32; ndx++) {
      result.bytes[ndx] =  set1.bytes[ndx] | set2.bytes[ndx];
   }
   return result;
}


Bit_Set_256 bs256_and(
   Bit_Set_256 set1,
   Bit_Set_256 set2)
{
   Bit_Set_256 result;
   for (int ndx = 0; ndx < 32; ndx++) {
      result.bytes[ndx] =  set1.bytes[ndx] & set2.bytes[ndx];
   }
   return result;
}


Bit_Set_256 bs256_and_not(
      Bit_Set_256 set1,
      Bit_Set_256 set2)
{
   // DBGMSG("Starting. vcplist1=%p, vcplist2=%p", vcplist1, vcplist2);
   Bit_Set_256 result;
   for (int ndx = 0; ndx < 32; ndx++) {
      result.bytes[ndx] =  set1.bytes[ndx] & ~set2.bytes[ndx];
   }

   // char * s = ddca_bs256_string(&result, "0x",", ");
   // DBGMSG("Returning: %s", s);
   // free(s);
   return result;
}


// #ifdef OLD
int bs256_count(
   Bit_Set_256 bbset)
{
   int result = 0;
   int flagndx;
   int bitndx;
   for (flagndx=0; flagndx < 32; flagndx++) {
      for (bitndx = 0; bitndx < 8; bitndx++) {
         unsigned char flagbit = (0x80 >> bitndx);
         if (bbset.bytes[flagndx] & flagbit)
            result += 1;
      }
   }
   // printf("(%s) returning: %d\n", __func__, result);
   return result;
}
// #endif


#ifdef COMPILE_ERRORS
int bs256_count(
      Bit_Set_256 bbset)
{
   // regard the array of 32 bytes as an array of 8 4-byte unsigned integers
   uint64_t  list2 = (uint64_t) bbset.bytes;
   unsigned int ct = 0;
   for (int ndx = 0; ndx < 4; ndx++) {
      // clever algorithm for counting number of bits per Brian Kernihgan
      uint64_t v = list2[ndx];
      for (; v; ct++) {
        v &= v - 1; // clear the least significant bit set
      }
      // DBGMSG("feature_list_count() returning: %d", ct);
   }
// #ifdef OLD
   assert(ct == bs256_count0(bbset));
// #endif
   return ct;
}
#endif


char *
bs256_to_string(
      Bit_Set_256  bitset,
      const char * value_prefix,
      const char * sepstr)
{
   bool debug = false;
   if (debug) {
      printf("(%s) value_prefix=|%s|, sepstr=|%s|\n",
             __func__, value_prefix, sepstr);
      for (int ndx = 0; ndx < 32; ndx++) {
         printf("%02x", bitset.bytes[ndx]);
      }
      printf("\n");
      // rpt_hex_dump((Byte*)feature_list, 32, 2);
   }

   static GPrivate  key = G_PRIVATE_INIT(g_free);
   static GPrivate  len_key = G_PRIVATE_INIT(g_free);

   if (!value_prefix)
      value_prefix = "";
   if (!sepstr)
      sepstr = "";
   int vsize = strlen(value_prefix) + 2 + strlen(sepstr);
   int bit_ct = bs256_count(bitset);
   int reqd_size = (bit_ct*vsize)+1;   // +1 for trailing null

   char * buf = get_thread_dynamic_buffer(&key, &len_key, reqd_size);
   // char * buf = calloc(1, reqd_size);

   buf[0] = '\0';
   // printf("(%s) feature_ct=%d, vsize=%d, buf size = %d",
   //          __func__, feature_ct, vsize, vsize*feature_ct);

   for (int ndx = 0; ndx < 256; ndx++) {
      if ( bs256_contains(bitset, ndx) )
         sprintf(buf + strlen(buf), "%s%02x%s", value_prefix, ndx, sepstr);
   }

   if (bit_ct > 0)
      buf[ strlen(buf)-strlen(sepstr)] = '\0';

   // printf("(%s) wolf 4\n", __func__);
   // DBGMSG("Returned string length: %d", strlen(buf));
   // DBGMSG("Returning %p - %s", buf, buf);
   if (debug)
   printf("(%s) Returning: len=%d, %s\n", __func__, (int) strlen(buf), buf);

   return buf;
}


#define BBF_ITER_MARKER "BBFI"
typedef struct {
   char        marker[4];
   Bit_Set_256 bbflags;
   int         lastpos;
} _Bit_Set_256_Iterator;


/** Creates an iterator for a #Bit_Set_256 instance.
 *  The iterator is an opaque object.
 */
Bit_Set_256_Iterator
bs256_iter_new(Bit_Set_256 bbflags) {
   _Bit_Set_256_Iterator * result = malloc(sizeof(_Bit_Set_256_Iterator));
   memcpy(result->marker, BBF_ITER_MARKER, 4);
   result->bbflags = bbflags;   // TODO: save pointer to unopaque _BitByteFlags
   result->lastpos = -1;
   return result;
}


/** Free a #Bit_Set_256_Iterator.
 *
 * \param bs256_iter handle to iterator (may be NULL)
 */
void
bs256_iter_free(
      Bit_Set_256_Iterator bs256_iter)
{
   _Bit_Set_256_Iterator * iter = (_Bit_Set_256_Iterator *) bs256_iter;

   if (bs256_iter) {
      assert(memcmp(iter->marker, BBF_ITER_MARKER, 4) == 0);
      iter->marker[3] = 'x';
      free(iter);
   }
}

/** Reinitializes an iterator.  Sets the current position before the first
 *  value.
 *
 * \param bs256_iter handle to iterator
 */
void
bs256_iter_reset(
      Bit_Set_256_Iterator bs256_iter)
{
   _Bit_Set_256_Iterator * iter = (_Bit_Set_256_Iterator *) bs256_iter;
   assert(iter && memcmp(iter->marker, BBF_ITER_MARKER, 4) == 0);

   iter->lastpos = -1;
}


/** Returns the number of the next bit that is set.
 *
 * \param bs256_iter handle to iterator
 * \return number of next bit that is set, -1 if no more
 */
int
bs256_iter_next(
      Bit_Set_256_Iterator
      bs256_iter)
{
   _Bit_Set_256_Iterator * iter = (_Bit_Set_256_Iterator *) bs256_iter;
   assert( iter && memcmp(iter->marker, BBF_ITER_MARKER, 4) == 0);
   // printf("(%s) Starting. lastpos = %d\n", __func__, iter->lastpos);

   int result = -1;
   for (int ndx = iter->lastpos + 1; ndx < 256; ndx++) {
      if (bs256_contains(iter->bbflags, ndx)) {
         result = ndx;
         iter->lastpos = ndx;
         break;
      }
   }
   // printf("(%s) Returning: %d\n", __func__, result);
   return result;
}

