/** \f error_info.c
 *
 *  Struct for reporting errors.
 *
 *  #Error_Info provides a pseudo-exception framework that can be integrated
 *  with more traditional status codes.  Instead of returning a status code,
 *  a C function returns a pointer to an #Error_Info instance in the case of
 *  an error, or NULL if there is no error.  Information about the cause of an
 *  error is retained for use by higher levels in the call stack.
 */

// Copyright (C) 2017-2024 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


/** \cond */

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <string.h>
/** \endcond */

#include "debug_util.h"
#include "glib_util.h"
#include "msg_util.h"
#include "report_util.h"
#include "string_util.h"
#include "traced_function_stack.h"

#include "error_info.h"


// Validates a pointer to an #Error_Info, using asserts
#define VALID_ERROR_INFO_PTR(ptr) \
   assert(ptr); \
   if (memcmp(ptr->marker, ERROR_INFO_MARKER, 4) != 0) { \
      DBG("Invalid ptr->marker, ptr=%p", ptr); \
      show_backtrace(1); \
      debug_current_traced_function_stack(false); \
   } \
   assert(memcmp(ptr->marker, ERROR_INFO_MARKER, 4) == 0);


// Forward references
static char * default_status_code_desc(int rc);

// Constants
// allows cause list to always be null terminated, even when no causes:
static Error_Info * empty_list[] = {NULL};
static const int CAUSE_ALLOC_INCREMENT = 10;

// Globals
// status code to string functions:
static ErrInfo_Status_String errinfo_desc_func = default_status_code_desc;
static ErrInfo_Status_String errinfo_name_func =  NULL;


//
// Initialization
//

/** Initializes the module.
 *
 *   \param   name_func  function returning the name of a status code
 *   \param   desc_func  function returning a description of a status code
 */
void
errinfo_init(
      ErrInfo_Status_String name_func,
      ErrInfo_Status_String desc_func)
{
   errinfo_name_func = name_func;
   errinfo_desc_func = desc_func;
}


//
// Utilities
//

/** Checks whether all causes have the same status code.
 *  If **status_code** != 0, all causes must have that value.
 *  If **status_code** == 0, all causes must have the same status code
 *  value as the first cause.
 *
 *  @param  erec        pointer to #Error_Info instance
 *  @param  status_code status code value to check
 *  @return true/false
 *
 *  if **erec** == NULL or the instance has no causes, returns false.
 */
bool
errinfo_all_causes_same_status(
      Error_Info * erec,
      int          status_code)
{
   bool debug = false;
   DBGF(debug, "Starting. status_code=%d, erec=%s", status_code, errinfo_summary(erec));

   bool all_same = false;
   if (erec) {
      VALID_ERROR_INFO_PTR(erec);
      if (erec->cause_ct > 0) {
         if (status_code == 0)
            status_code = erec->causes[0]->status_code;
         all_same = true;
         for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
            if (erec->causes[ndx]->status_code != status_code) {
               all_same = false;
               break;
            }
         }
      }
   }

   DBGF(debug, "Returning: %s", SBOOL(all_same));
   return all_same;
}

#ifdef ALT
bool
errinfo_all_causes_same_status(Error_Info * ddc_excp, int status_code) {
     bool all_same = true;
     for (int ndx = 0; ndx < ddc_excp->cause_ct; ndx++) {
        if (ddc_excp->causes[ndx]->status_code != status_code) {
           all_same = false;
           break;
        }
     }
     return all_same;
  }
#endif


//
// Instance destruction
//

/** Releases a #Error_Info instance, including all instances it points to.
 *
 *  \param erec pointer to #Error_Info instance, do nothing if NULL
 */
void
errinfo_free(Error_Info * erec){
   bool debug = false;
   if (debug) {
      DBG("Starting. erec=%p", (void*)erec);
      // show_backtrace(2);
      // errinfo_report(erec, 2);
   }
   if (erec) {
      VALID_ERROR_INFO_PTR(erec);

      if (debug) {
         DBG("Freeing exception:");
         errinfo_report(erec, 2);
      }

      if (erec->detail)
         free(erec->detail);

      if (erec->cause_ct > 0) {
         DBGF(debug, "Freeing causes...");
         for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
            errinfo_free(erec->causes[ndx]);
         }
         DBGF(debug, "Freeing erec->causes = %p", erec->causes);
         free(erec->causes);
      }
#ifdef ALT
      if (erec->causes_alt) {
         for (int ndx = 0; ndx < erec->causes_alt->len; ndx++) {
            errinfo_free( g_ptr_array_index(erec->causes_alt, ndx) );
         }
      }
#endif

      free(erec->func);
      erec->marker[3] = 'x';
      free(erec);
   }

   DBGF(debug, "Done.  Free'd: %p", (void*) erec);
}


/** Releases a #Error_Info instance, including all instances it points to.
 *  Optionally reports the instance before freeing it.
 *
 *  \param  erec   pointer to #Error_Info instance,
 *                 do nothing if NULL
 *  \param  report if true, report the instance
 *  \param  func   name of calling function
 */
void
errinfo_free_with_report(
      Error_Info * erec,
      bool         report,
      const char * func)
{
   if (erec) {
      if (report) {
         rpt_vstring(0, "(%s) Freeing exception:", func);
         errinfo_report(erec, 1);
      }
      errinfo_free(erec);
   }
}


#ifdef ALT
// signature satisfying GDestroyNotify()

static void ddc_error_free2(void * erec) {
   Error_Info* erec2 = (Error_Info *) erec;
   VALID_ERROR_INFO_PTR(erec2);
   errinfo_free(erec2);
}
#endif

//
// Instance modification
//

/** Sets the status code in a existing #Error_Info instance.
 *
 *  \param  erec   pointer to instance
 *  \param  code   status code
 */
void
errinfo_set_status(Error_Info * erec, int code) {
   VALID_ERROR_INFO_PTR(erec);
   erec->status_code = code;
}


/** Sets the detail string in a existing #Error_Info instance.
 *  If there is already a detail string in the instance, it is replaced.
 *  The substitution values for the detail string are specified as a va_list.
 *
 *  \param  erec   pointer to instance
 *  \param  detail detail format string
 *  \param  args   arguments for detail string
 */
static void
errinfo_set_detailv(
      Error_Info * erec,
      const char * detail,
      va_list      args)
{
   if (detail) {
      erec->detail = g_strdup_vprintf(detail, args);
   }
}


/** Sets the detail string in a existing #Error_Info instance.
 *  If there is already a detail string in the instance, it is replaced.
 *
 *  \param  erec   pointer to instance
 *  \param  detail detail format string
 *  \param  ...    arguments for detail format string
 */
void
errinfo_set_detail(
      Error_Info *  erec,
      const char *  detail_fmt,
      ...)
{
   VALID_ERROR_INFO_PTR(erec);
   if (erec->detail) {
      free(erec->detail);
      erec->detail = NULL;
   }
   va_list ap;
   va_start(ap, detail_fmt);

   errinfo_set_detailv(erec, detail_fmt, ap);
   va_end(ap);
}


/** Make a deep copy of a #Error_Info record.
 *
 *  @param  old  record to copy
 *  @return copy of record
 */
Error_Info * errinfo_copy(Error_Info* old) {
   bool debug = false;
   DBGF(debug, "Starting. old=%p", (void*) old);

   Error_Info * new = calloc(1, sizeof(Error_Info));
   memcpy(new->marker, old->marker, 4);
   new->status_code = old->status_code;
   if (old->func)
    new->func = g_strdup(old->func);
   if (old->detail)
      new->detail = g_strdup(old->detail);
   new->max_causes = old->max_causes;
   new->cause_ct = old->cause_ct;
   if (new->cause_ct == 0)
      new->causes = empty_list;
   else {
      new->causes = calloc(new->max_causes+1, sizeof(Error_Info*));
      DBGF(debug, "Allocated new->causes=%p, new->cause_ct=%d", (void*) new->causes, new->cause_ct);
   }
   for (int ndx = 0; ndx < new->cause_ct; ndx++) {
      new->causes[ndx] = errinfo_copy(old->causes[ndx]);
   }

   DBGF(debug, "Done.  Returning %p", (void*) new);
   return new;
}


/** Adds a cause to an existing #Error_Info instance
 *
 *  \param  parent instance to which cause will be added
 *  \param  cause  instance to add
 */
void
errinfo_add_cause(
      Error_Info * parent,
      Error_Info * cause)
{
   bool debug = false;
   DBGF(debug, "parent=%p, cause=%p", parent, cause);
   VALID_ERROR_INFO_PTR(parent);
   VALID_ERROR_INFO_PTR(cause);
   DBGF(debug, "parent->cause_ct = %d, parent->max_causes = %d",
               parent->cause_ct, parent->max_causes);

   if (parent->cause_ct == parent->max_causes) {
      int new_max = parent->max_causes + CAUSE_ALLOC_INCREMENT;
#ifdef ALT
      Error_Info **  new_causes = calloc(new_max+1, sizeof(Error_Info *) );
      memcpy(new_causes, parent->causes, parent->cause_ct * sizeof(Error_Info *) );
      free(parent->causes);
      parent->causes = new_causes;
#endif
      if (parent->causes == empty_list) {
         DBGF(debug, "empty_list");
         parent->causes = calloc(new_max+1, sizeof(Error_Info *) );
      }
      else {
         // works, but requires _GNU_SOURCE feature test macro:
         // parent->causes = reallocarray(parent->causes, new_max+1, sizeof(Error_Info*) );
         void * new_causes = calloc(new_max+1, sizeof(Error_Info*) );
         memcpy(new_causes, parent->causes, parent->max_causes * sizeof(Error_Info *) );
         free(parent->causes);
         parent->causes = new_causes;
      }
      parent->max_causes = new_max;
   }

   DBGF(debug, "causes = %p, cause_ct=%d", parent->causes, parent->cause_ct);
   // DBGF(debug, "%p", &parent->causes[parent->cause_ct]);

   parent->causes[parent->cause_ct++] = cause;

#ifdef ALT
   if (!parent->causes_alt) {
      parent->causes_alt = g_ptr_array_new_with_free_func(ddc_error_free2);
      // parent->causes_alt = g_ptr_array_new();   // *** TRANSITIONAL ***
   }
   g_ptr_array_add(parent->causes_alt, cause);
#endif

   DBGF(debug, "Done. causes = %p, cause_ct=%d", parent->causes, parent->cause_ct);
}


//
// Instance creation
//

/** Creates a new #Error_Info instance with the specified status code,
 *  function name, and detail string.  The substitution values for the
 *  detail string are specified as a va_list.
 *
 *  \param  status_code  status code
 *  \param  func         name of function generating status code
 *  \param  detail       detail string, may be NULL
 *  \param  args         substitution values
 *  \return pointer to new instance
 */
static Error_Info *
errinfo_newv(
      int            status_code,
      const char *   func,
      const char *   detail,
      va_list        args)
{
   bool debug = false;
   DBGF(debug, "Starting. status_code=%d, func=%s, detail=%s", status_code, func, detail);

   Error_Info * erec = calloc(1, sizeof(Error_Info));
   memcpy(erec->marker, ERROR_INFO_MARKER, 4);
   erec->status_code = status_code;
   erec->causes = empty_list;
   erec->func = g_strdup(func);   // g_strdup to avoid constness warning, must free

   if (detail) {
      erec->detail = g_strdup_vprintf(detail, args);
   }

   DBGF(debug, "Done:    Returning %p", erec);
   return erec;
}


/** Creates a new #Error_Info instance with the specified status code,
 *  function name, and detail string.
 *
 *  \param  status_code  status code
 *  \param  func         name of function generating status code
 *  \param  detail       optional detail string
 *  \param  ...          substitution values
 *  \return pointer to new instance
 */
Error_Info *
errinfo_new(
      int            status_code,
      const char *   func,
      const char *   detail,
      ...)
{
   Error_Info * erec = NULL;
   va_list ap;
   va_start(ap, detail);
   erec = errinfo_newv(status_code, func, detail, ap);
   va_end(ap);
   // errinfo_report(erec, 1);
   return erec;
}


/** Creates a new #Error_Info instance with a detail string, including a
 * reference to another instance that is the cause of the current error.
 *  The substitution values for the detail string are specified as a va_list.
 *
 *  \param  status_code  status code
 *  \param  cause        pointer to another #Error_Info that is included as a cause
 *  \param  func         name of function creating new instance
 *  \param  detail_fmt   optional detail format string
 *  \param  args         substitution value for detail_fmt
 *  \return pointer to new instance
 */
static Error_Info *
errinfo_new_with_causev(
      int            status_code,
      Error_Info *   cause,
      const char *   func,
      const char *   detail_fmt,
      va_list        args)
{
   Error_Info * erec = errinfo_newv(status_code, func, detail_fmt, args);
   if (cause)
      errinfo_add_cause(erec, cause);
   return erec;
}


/** Creates a new #Error_Info instance with a detail string, including a
 * reference to another instance that is the cause of the current error.
 *
 *  \param  status_code  status code
 *  \param  cause        pointer to another #Error_Info that is included as a cause
 *  \param  func         name of function creating new instance
 *  \param  detail_fmt   optional detail format string
 *  \param  ...          optional arguments for detail_fmt
 *  \return pointer to new instance
 */
Error_Info *
errinfo_new_with_cause(
      int            status_code,
      Error_Info *   cause,
      const char *   func,
      const char *   detail_fmt,
      ...)
{
   Error_Info * erec = NULL;
   va_list ap;
   va_start(ap, detail_fmt);
   erec = errinfo_new_with_causev(status_code, cause, func, detail_fmt, ap);
   va_end(ap);
   return erec;
}


#ifdef UNUSED
/** Creates a new #Error_Info instance, including a reference to another
 *  instance that is the cause of the current error.  The status code
 *  of the new instance is the same as that of the referenced instance.
 *
 *  \param  cause pointer to another #Error_Info that is included as a cause
 *  \param  func  name of function creating new instance
 *  \return pointer to new instance
 */
Error_Info * errinfo_new_chained(
      Error_Info * cause,
      const char * func)
{
   VALID_ERROR_INFO_PTR(cause);
   Error_Info * erec = errinfo_new_with_cause(cause->status_code, cause, func);
   return erec;
}
#endif


/** Creates a new #Error_Info instance with a collection of
 *  instances specified as the causes.
 *
 *  \param  code            status code of the new instance
 *  \param  causes          array of #Error_Info instances
 *  \param  cause_ct        number of causes
 *  \param  func            name of function creating the new #Error_Info
 *  \param  detail          detail format string
 *  \param  ...             optional arguments for detail format string
 *  \return pointer to new instance
 */
Error_Info *
errinfo_new_with_causes(
      int            status_code,
      Error_Info **  causes,
      int            cause_ct,
      const char *   func,
      char *         detail,
      ...)
{
   va_list ap;
   va_start(ap, detail);
   Error_Info * result = errinfo_newv(status_code, func, detail, ap);
   va_end(ap);
   for (int ndx = 0; ndx < cause_ct; ndx++) {
      errinfo_add_cause(result, causes[ndx]);
   }
   return result;
}


/** Creates a new #Error_Info instance with a collection of instances specified
 *  as the causes. The collection is passed as a GPtrArray.  A deep copy is
 *  made of each instance in the collection, and the causes collection
 *  is unchanged.
 *
 *  \param  code            status code of the new instance
 *  \param  causes          GPtrArray of #Error_Info instances
 *  \param  func            name of function creating the new #Error_Info
 *  \param  detail          detail format string
 *  \param  ...             optional arguments for detail format string
 *  \return pointer to new instance
 */
Error_Info * errinfo_new_with_causes_gptr(
      int            status_code,
      GPtrArray*     causes,
      const char *   func,
      char *         detail,
      ...)
{
   bool debug = false;
   DBGF(debug, "Starting.  status_code-%d, detail=%s", status_code, detail);

   va_list ap;
   va_start(ap, detail);
   Error_Info * result = errinfo_newv(status_code, func, detail, ap);
   va_end(ap);
   for (int ndx = 0; ndx < causes->len; ndx++) {
         errinfo_add_cause(result, errinfo_copy(g_ptr_array_index(causes,ndx)));
   }

   DBGF(debug, "Returning: %p", (void*) result);
   return result;
}


#ifdef UNUSED

// For creating a new Ddc_Error when the called functions
// return status codes not Ddc_Errors.


/** Creates a new #Error_Info instance, including references to multiple
 *  status codes from called functions that contribute to the current error.
 *  Each of the callee status codes is wrapped in a synthesized #Error_Info
 *  instance that is included as a cause.
 *
 *  \param  status_code
 *  \param  callee_status_codes    array of status codes
 *  \param  callee_status_code_ct  number of status codes in **callee_status_codes**
 *  \param  callee_func            name of function that returned **callee** status codes
 *  \param  func                   name of function generating new #Error_Info
 *  \return pointer to new instance
 */
Error_Info * errinfo_new_with_callee_status_codes(
      int    status_code,
      int *  callee_status_codes,
      int                   callee_status_code_ct,
      const char *          callee_func,
      const char *          func)
{
   Error_Info * result = errinfo_new(status_code, func);
   for (int ndx = 0; ndx < callee_status_code_ct; ndx++) {
      Error_Info * cause = errinfo_new(callee_status_codes[ndx],callee_func);
      errinfo_add_cause(result, cause);
   }
   return result;
}
#endif


//
// Reporting
//

/** Status code description function to be used if none was set
 *  by #errinfo_init()
 *
 *  \param  code  status code
 *  \return description of status code
 *
 *  The value returned is valid until the next call to this function
 *  in the current thread.
 */
static char *
default_status_code_desc(int rc) {
   static GPrivate  status_code_key     = G_PRIVATE_INIT(g_free);

   const int default_status_code_buffer_size = 20;

   char * buf = get_thread_fixed_buffer(&status_code_key, default_status_code_buffer_size);
   g_snprintf(buf, default_status_code_buffer_size, "%d",rc);

   return buf;
}


/** Appends a comma separated string of the status code names of the
 *  causes in an array of #Error_Info to an existing string.
 *  Multiple consecutive identical names are replaced with a
 *  single name and a parenthesized instance count.
 *
 *  \param  erec     pointer to array of pointers to #Error_Info instances
 *  \param  error_ct number of errors
 *  \return modified comma separated string
 */
static GString *
errinfo_array_summary_gs(
      struct error_info **  errors,    ///<  pointer to array of pointers to #Error_Info
      int                   error_ct,  ///<  number of causal errors
      GString *             gs)        ///<  append result here
{
   bool first = true;

   int ndx = 0;
   while (ndx < error_ct) {
      // printf("(%s) this error = %p\n", __func__, errors[ndx]);
      int this_psc = errors[ndx]->status_code;
      int cur_ct = 1;

      for (int i = ndx+1; i < error_ct; i++) {
         if (errors[i]->status_code != this_psc)
            break;
         cur_ct++;
      }

      if (first)
         first = false;
      else
         g_string_append(gs, ", ");
      if (errinfo_name_func)
         g_string_append(gs, errinfo_name_func(this_psc));
      else {
         char buf[20];
         snprintf(buf, 20, "%d",this_psc);
         buf[19] = '\0';
         g_string_append(gs, buf);
      }
      if (cur_ct > 1)
         g_string_append_printf(gs, "(%d)", cur_ct);
      ndx += cur_ct;
   }

   return gs;
}


/** Returns a comma separated string of the status code names of the
 *  causes in an array of #Error_Info.
 *  Multiple consecutive identical names are replaced with a
 *  single name and a parenthesized instance count.
 *
 *  \param  errors   pointer to array of pointers to #Error_Info instances
 *  \param  error_ct number of instances
 *  \return comma separated string, caller is responsible for freeing
 */
char *
errinfo_array_summary(
      struct error_info **  errors,    ///<  pointer to array of pointers to Error_Info
      int                   error_ct)
{
   GString * gs = g_string_new(NULL);
   // printf("(%s) errors=%p\n", __func__, errors);
   errinfo_array_summary_gs(errors, error_ct, gs);
   char * result = gs->str;
   g_string_free(gs, false);

   // DBGMSF(debug, "Done.  Returning: |%s|", result);
   return result;
}


/** Returns a comma separated string of the names of the status codes in the
 *  causes of the specified #Error_Info.
 *  Multiple consecutive identical names are replaced with a
 *  single name and a parenthesized instance count.
 *
 *  \param  erec  pointer to #Error_Info instance
 *  \return comma separated string, caller is responsible for freeing
 */
char *
errinfo_causes_string(Error_Info * erec) {
   // bool debug = false;

   GString * gs = g_string_new(NULL);

   if (erec) {
      assert(memcmp(erec->marker, ERROR_INFO_MARKER, 4) == 0);

      errinfo_array_summary_gs(erec->causes, erec->cause_ct, gs);
   }
   char * result = gs->str;
   g_string_free(gs, false);

   // DBGMSF(debug, "Done.  Returning: |%s|", result);
   return result;

#ifdef OLD
   GString * gs = g_string_new(NULL);

   if (erec) {
      assert(memcmp(erec->marker, ERROR_INFO_MARKER, 4) == 0);

      bool first = true;

      int ndx = 0;
      while (ndx < erec->cause_ct) {
         int this_psc = erec->causes[ndx]->status_code;
         int cur_ct = 1;

         for (int i = ndx+1; i < erec->cause_ct; i++) {
            if (erec->causes[i]->status_code != this_psc)
               break;
            cur_ct++;
         }

         if (first)
            first = false;
         else
            g_string_append(gs, ", ");
         if (errinfo_name_func)
            g_string_append(gs, errinfo_name_func(this_psc));
         else {
            char buf[20];
            snprintf(buf, 20, "%d",this_psc);
            buf[19] = '\0';
            g_string_append(gs, buf);
         }
         if (cur_ct > 1)
            g_string_append_printf(gs, "(%d)", cur_ct);
         ndx += cur_ct;
      }
   }

   char * result = gs->str;
   g_string_free(gs, false);

   // DBGMSF(debug, "Done.  Returning: |%s|", result);
   return result;
#endif
}


#ifdef ALT
 char * errinfo_causes_string_alt(Error_Info * erec) {
    // bool debug = false;
    // DBGMSF(debug, "history=%p, history->ct=%d", history, history->ct);

    GString * gs = g_string_new(NULL);

    if (erec) {
       assert(memcmp(erec->marker, ERROR_INFO_MARKER, 4) == 0);

       if (erec->causes_alt) {
          bool first = true;

          int ndx = 0;
          int cause_ct = erec->causes_alt->len;
          while (ndx < cause_ct) {
             Error_Info * this_cause = g_ptr_array_index( erec->causes_alt, ndx);
             int this_psc = this_cause->status_code;
             int cur_ct = 1;

             for (int i = ndx+1; i < cause_ct; i++) {
                Error_Info * next_cause = g_ptr_array_index( erec->causes_alt, i);
                if (next_cause->status_code != this_psc)
                   break;
                cur_ct++;
             }

             if (first)
                first = false;
             else
                g_string_append(gs, ", ");
             if (errinfo_name_func)
                g_string_append(gs, errinfo_name_func(this_psc));
             else {
                char buf[20];
                snprintf(buf, 20, "%d",this_psc);
                buf[19] = '\0';
                g_string_append(gs, buf);
             }
             if (cur_ct > 1)
                g_string_append_printf(gs, "(x%d)", cur_ct);
             ndx += cur_ct;
          }
       }
    }

   char * result = gs->str;
   g_string_free(gs, false);

   // DBGMSF(debug, "Done.  Returning: |%s|", result);
   return result;
}
#endif


/** Creates a full report of the contents of the specified #Error_Info,
 *  using report functions. if **collector** is non-null, the lines of
 *  the report are appended to the array.  If it is null, the lines
 *  are written the current report output destination.
 *
 *  \param  erec   pointer to #Error_Info
 *  \param  collector collects lines of the report
 *  \param  depth  logical indentation depth
 */
void
errinfo_report_collect(Error_Info * erec, GPtrArray* collector, int depth) {
   assert(erec);
   int d1 = depth+1;

   // rpt_vstring(depth, "(%s) Status code: %d", __func__, erec->status_code);
   // rpt_vstring(depth, "(%s) Location: %s", __func__, (erec->func) ? erec->func : "not set");
   // rpt_vstring(depth, "(%s) errinfo_name_func=%p, errinfo_desc_func=%p", __func__, errinfo_name_func, errinfo_desc_func);
   // rpt_push_output_dest(stderr);
   rpt_vstring_collect(depth, collector, "Exception in function %s: status=%s",
         (erec->func) ? erec->func : "not set",
         errinfo_desc_func(erec->status_code) );  // can't call psc_desc(), violates layering
   if (erec->detail)
      rpt_label_collect(depth+1, collector, erec->detail);
   // rpt_pop_output_dest();

   if (erec->cause_ct > 0) {
      rpt_vstring_collect(depth, collector, "Caused by: ");
      for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
         errinfo_report_collect(erec->causes[ndx], collector, d1);
      }
   }

#ifdef ALT
   if (erec->causes_alt && erec->causes_alt->len > 0) {
      rpt_vstring(depth, "Caused by: ");
      for (int ndx = 0; ndx < erec->causes_alt->len; ndx++) {
         errinfo_report( g_ptr_array_index(erec->causes_alt,ndx), d1);
      }
   }
#endif
   // rpt_vstring(depth, "(%s) Done", __func__);

}


/** Emits a full report of the contents of the specified #Error_Info,
 *  using report functions.
 *
 *  \param  erec   pointer to #Error_Info
 *  \param  depth  logical indentation depth
 */
void
errinfo_report(Error_Info * erec, int depth) {
      errinfo_report_collect(erec, NULL, depth);
}


/** Reports detail strings for a #Error_Info record and
 *  each of its contained errors.
 *
 *  \param erec  pointer = #Error_Info instance
 *  \param depth logical indentation depth
 */
void
errinfo_report_details(Error_Info * erec, int depth) {
   assert(erec);
   int d0 = depth;
   int d1 = depth+1;

   // rpt_vstring(depth, "Exception in function %s: status=%s",
   //       (erec->func) ? erec->func : "not set",
   //       errinfo_desc_func(erec->status_code) );  // can't call psc_desc(), violates layering
   if (erec->detail)
      rpt_label(d0, erec->detail);
   else
      rpt_vstring(d0, "Error %d in function %s",erec->status_code, erec->func);

   // printf("%s) cause_ct = %d\n", __func__, erec->cause_ct);
   if (erec->cause_ct > 0) {
      // rpt_vstring(depth, "Caused by: ");
      for (int ndx = 0; ndx < erec->cause_ct; ndx++) {
         errinfo_report_details(erec->causes[ndx], d1);
      }
   }
}


/** Returns a string summary of the specified #Error_Info.
 *  The returned value is valid until the next call to this function in the
 *  current thread, and should not be freed by the caller.
 *
 *  \param  erec  pointer to #Error_Info instance
 *  \return string summary of error
 */
char *
errinfo_summary(Error_Info * erec) {
   if (!erec)
      return "NULL";
   VALID_ERROR_INFO_PTR(erec);

   static GPrivate  esumm_key     = G_PRIVATE_INIT(g_free);
   static GPrivate  esumm_len_key = G_PRIVATE_INIT(g_free);

   // rpt_vstring(1, "(%s) errinfo_name_func=%p, errinfo_desc_func=%p", __func__, errinfo_name_func, errinfo_desc_func);

   char * desc = errinfo_name_func(erec->status_code);  // thread safe buffer owned by psc_desc(), do not free()

   gchar * buf1 = NULL;
   if (erec->cause_ct == 0) {
#ifdef ALT
   if (erec->causes_alt || erec->causes_alt->len == 0) {
#endif
      buf1 = g_strdup_printf("Error_Info[%s in %s]", desc, erec->func);
   }
   else {
      char * causes = errinfo_causes_string(erec);
      buf1 = g_strdup_printf("Error_Info[%s in %s, causes: %s]", desc, erec->func, causes);
      free(causes);
   }
   int required_size = strlen(buf1) + 1;

   char * buf = get_thread_dynamic_buffer(&esumm_key, &esumm_len_key, required_size);
   strcpy(buf, buf1);
   free(buf1);
   return buf;
}
