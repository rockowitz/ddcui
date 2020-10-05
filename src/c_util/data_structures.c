// data_structures.c

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <string.h>

#include "coredefs.h"
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
// bbf - ByteBitFlags -
//
// An opaque data structure containing 256 flags
//

#define BYTE_BIT_MARKER  "BBFG"

#define BYTE_BIT_BYTE_CT 32    // number of bytes in data structure: 256/8
#define BYTE_BIT_UNOPAQUE(unopaque_var, opaque_var) _ByteBitFlags* unopaque_var = (_ByteBitFlags*) opaque_var
#define BYTE_BIT_VALIDATE(flags)    assert(flags && ( memcmp(flags->marker, BYTE_BIT_MARKER, 4) == 0))

typedef struct {
   char marker[4];    // always BBFG
   char byte[BYTE_BIT_BYTE_CT];
} _ByteBitFlags;
// typedef _ByteBitFlags* PByteBitFlags;


static _ByteBitFlags * bbf_create_internal() {
   _ByteBitFlags* flags = calloc(1, sizeof(_ByteBitFlags));
   memcpy(flags->marker, BYTE_BIT_MARKER, 4);
   return flags;
}


/** Creates a new **Byte_Bit_Flags** instance.
 *
 * @return opaque handle to new instance
 */
Byte_Bit_Flags bbf_create() {
   return bbf_create_internal();
}


/** Destroys a **Byte_Bit_Flags** instance.
 *
 * @param bbflags instance handle
 */
void bbf_free(Byte_Bit_Flags bbflags) {
   // _ByteBitFlags* flags = (_ByteBitFlags*) bbflags;
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   if (flags) {
      assert( memcmp(flags->marker, "BBFG",4) == 0);
      free(flags);
   }
}


/** Sets a flag in a **Byte_Bit_Flags** instance.
 *
 * @param bbflags instance handle
 * @param val     number of bit to set
 */
void bbf_set(Byte_Bit_Flags bbflags, Byte val) {
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   BYTE_BIT_VALIDATE(flags);
   int flagndx   = val >> 3;
   int shiftct   = val & 0x07;
   Byte flagbit  = 0x01 << shiftct;
   // printf("(%s) val=0x%02x, flagndx=%d, shiftct=%d, flagbit=0x%02x\n",
   //        __func__, val, flagndx, shiftct, flagbit);
   flags->byte[flagndx] |= flagbit;
}


/** Tests if a flag is set in a **Byte_Bit_Flags** instance.
 *
 * @param bbflags instance handle
 * @param val     number of bit to test
 * @return        true/false
 */
bool bbf_is_set(Byte_Bit_Flags bbflags, Byte val) {
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   BYTE_BIT_VALIDATE(flags);
   int flagndx   = val >> 3;
   int shiftct   = val & 0x07;
   Byte flagbit  = 0x01 << shiftct;
   // printf("(%s) val=0x%02x, flagndx=%d, shiftct=%d, flagbit=0x%02x\n",
   //        __func__, val, flagndx, shiftct, flagbit);
   bool result = flags->byte[flagndx] & flagbit;
   // printf("(%s) bbflags=0x%s, val=0x%02x, returning: %d\n",
   //        __func__, hexstring( (unsigned char *)flags->byte,32), val, result);
   // printf("(%s) val = 0x%02x, returning %s\n",  __func__, val, sbool(result));
   return result;
}


/** Subtracts one **Byte_Bit_Flags** instance from another.
 *  A flag is set in the result if it is set in the first instance
 *  but not in the second instance.
 *
 * @param bbflags1 handle to first instance
 * @param bbflags2 handle to second instance
 * @return newly created instance with the result
 */
Byte_Bit_Flags bbf_subtract(Byte_Bit_Flags bbflags1, Byte_Bit_Flags bbflags2) {
   BYTE_BIT_UNOPAQUE(flags1, bbflags1);
   BYTE_BIT_VALIDATE(flags1);
   BYTE_BIT_UNOPAQUE(flags2, bbflags2);
   BYTE_BIT_VALIDATE(flags2);
   _ByteBitFlags *  result = bbf_create();
   for (int ndx = 0; ndx < BYTE_BIT_BYTE_CT; ndx++) {
      result->byte[ndx] = flags1->byte[ndx] & ~flags2->byte[ndx];
   }
   return result;
}



/** Returns a 64 character long hex string representing the data structure.
 *
 *  @param   bbflags instance handle
 *  @param   buffer  buffer in which to return string
 *  @param   buflen  buffer length
 *
 * @return  character string representation of flags that are set
 *
 * If buffer is NULL then memory is malloc'd.  It is the responsibility
 * of the caller to free the returned string.
 *
 * If buflen is insufficiently large an assertion fails.
 *
 * @remark
 * Future enhancement:  Insert a separator character every n characters?
 */
char * bbf_repr(Byte_Bit_Flags bbflags, char * buffer, int buflen) {
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   BYTE_BIT_VALIDATE(flags);
   int reqd_size = (2*BYTE_BIT_BYTE_CT) /* 2 hex chars for each byte*/ + 1 /* trailing null*/ ;
   if (buffer)
      assert(buflen >= reqd_size);
   else
      buffer = malloc(reqd_size);
   *buffer = '\0';
   int flagndx = 0;
   for (; flagndx < 8; flagndx++)
     sprintf(buffer + strlen(buffer), "%02x", flags->byte[flagndx]);
   return buffer;
}


/** Returns the number of bits set in a **Byte_Bit_Flags**
 *
 * @param bbflags  instance handle
 * @return number of bits set (0..256)
 */
int bbf_count_set(Byte_Bit_Flags bbflags) {
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   BYTE_BIT_VALIDATE(flags);
   int result = 0;
   int flagndx;
   int bitndx;
   for (flagndx=0; flagndx < BYTE_BIT_BYTE_CT; flagndx++) {
      for (bitndx = 0; bitndx < 8; bitndx++) {
         unsigned char flagbit = (0x80 >> bitndx);
         if (flags->byte[flagndx] & flagbit)
            result += 1;
      }
   }
   // printf("(%s) returning: %d\n", __func__, result);
   return result;
}


/** Returns a string of space separated 2 character hex values
 * representing the bits set in the Byte_Bit_Flag,
 * e.g. "03 7F" if bits 0x03 and 0x7F are set
 *
 * @param  bbflags  instance handle
 * @param  buffer   pointer to buffer in which to return character string,
 *                  if NULL malloc a new buffer
 * @param  buflen   buffer length
 *
 * @return pointer to character string
 *
 * If a new buffer is allocated, it is the responsibility of the caller to
 * free the string returned.
 *
 * For complete safety in case every bit is set, buflen should be >= 768.
 * (2 chars for every bit (512), 255 separator characters, 1 terminating null)
 * If buflen in insufficiently large to contain the result, an assertion fails.
 */
char * bbf_to_string(Byte_Bit_Flags bbflags, char * buffer, int buflen) {
   // printf("(%s) Starting\n", __func__);
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   BYTE_BIT_VALIDATE(flags);
   int bit_set_ct = bbf_count_set(flags);
   int reqd_size = bit_set_ct * 2     +     // char rep of bytes
                   (bit_set_ct-1) * 1 +     // separating spaces
                   1;                       // trailing null
   if (buffer)
      assert(buflen >= reqd_size);
   else
      buffer = malloc(reqd_size);
   char * pos = buffer;
   unsigned int flagno = 0;
   // printf("(%s) bbflags->byte=0x%s\n", __func__, hexstring(flags->byte,32));
   for (flagno = 0; flagno < 256; flagno++) {
      Byte flg = (Byte) flagno;
      // printf("(%s) flagno=%d, flg=0x%02x\n", __func__, flagno, flg);
      if (bbf_is_set(flags, flg)) {
         // printf("(%s) Flag is set: %d, 0x%02x\n", __func__, flagno, flg);
         if (pos > buffer) {
            *pos  = ' ';
            pos++;
         }
         // printf("(%s) flg=%02x\n", __func__, flg);
         sprintf(pos, "%02x", flg);
         pos += 2;
         // printf("(%s) pos=%p\n", __func__, pos);
      }
   }
   // printf("(%s) Done.  Returning: %s\n", __func__, buffer);
   return buffer;
}


int bbf_to_bytes(Byte_Bit_Flags bbflags, Byte * buffer, int buflen) {
   // printf("(%s) Starting\n", __func__);
   BYTE_BIT_UNOPAQUE(flags, bbflags);
   BYTE_BIT_VALIDATE(flags);

   int bit_set_ct = bbf_count_set(flags);
   assert(buflen >= bit_set_ct);

   unsigned int bufpos = 0;
   unsigned int flagno = 0;
   // printf("(%s) bbflags->byte=0x%s\n", __func__, hexstring(flags->byte,32));
   for (flagno = 0; flagno < 256; flagno++) {
      Byte flg = (Byte) flagno;
      // printf("(%s) flagno=%d, flg=0x%02x\n", __func__, flagno, flg);
      if (bbf_is_set(flags, flg)) {
         // printf("(%s) Flag is set: %d, 0x%02x\n", __func__, flagno, flg);
         buffer[bufpos++] = flg;
      }
   }
   // printf("(%s) Done.  Returning: %d\n", __func__, bupos);
   return bufpos;
}




#define BBF_ITER_MARKER "BBFI"
typedef struct {
   char  marker[4];
   Byte_Bit_Flags bbflags;
   int   lastpos;
} _Byte_Bit_Flags_Iterator;


/** Creates an iterator for a #Byte_Bit_Flags instance.
 *  The iterator is an opaque object.
 *
 * \param bbflags handle to #Byte_Bit_Flags instance
 * \return iterator
 */
Byte_Bit_Flags_Iterator bbf_iter_new(Byte_Bit_Flags bbflags) {
   _Byte_Bit_Flags_Iterator * result = malloc(sizeof(_Byte_Bit_Flags_Iterator));
   memcpy(result->marker, BBF_ITER_MARKER, 4);
   result->bbflags = bbflags;   // TODO: save pointer to unopaque _BitByteFlags
   result->lastpos = -1;
   return result;
}


/** Free a #Byte_Bit_Flags_Iterator.
 *
 * \param bbf_iter handle to iterator (may be NULL)
 */
void bbf_iter_free(Byte_Bit_Flags_Iterator bbf_iter) {
   _Byte_Bit_Flags_Iterator * iter = (_Byte_Bit_Flags_Iterator *) bbf_iter;

   if (bbf_iter) {
      assert(memcmp(iter->marker, BBF_ITER_MARKER, 4) == 0);
      iter->marker[3] = 'x';
      free(iter);
   }
}

/** Reinitializes an iterator.  Sets the current position before the first
 *  value.
 *
 * \param bbf_iter handle to iterator
 */
void bbf_iter_reset(Byte_Bit_Flags_Iterator bbf_iter) {
   _Byte_Bit_Flags_Iterator * iter = (_Byte_Bit_Flags_Iterator *) bbf_iter;
   assert(iter && memcmp(iter->marker, BBF_ITER_MARKER, 4) == 0);

   iter->lastpos = -1;
}


/** Returns the number of the next bit that is set.
 *
 * \param bbf_iter handle to iterator
 * \return number of next bit that is set
 */
int bbf_iter_next(Byte_Bit_Flags_Iterator bbf_iter) {
   _Byte_Bit_Flags_Iterator * iter = (_Byte_Bit_Flags_Iterator *) bbf_iter;
   assert( iter && memcmp(iter->marker, BBF_ITER_MARKER, 4) == 0);
   // printf("(%s) Starting. lastpos = %d\n", __func__, iter->lastpos);

   int result = -1;
   for (int ndx = iter->lastpos + 1; ndx < 256; ndx++) {
      if (bbf_is_set(iter->bbflags, ndx)) {
         result = ndx;
         iter->lastpos = ndx;
         break;
      }
   }
   // printf("(%s) Returning: %d\n", __func__, result);
   return result;
}

