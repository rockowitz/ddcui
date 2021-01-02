/* vcpthread.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <errno.h>
#include <glib-2.0/glib.h>
#include <iostream>
#include <string.h>

#include "ddcutil_status_codes.h"  // using quotes allows Eclipse to find in workspace
#include "ddcutil_c_api.h"

#include <QtCore/QString>

#include "base/core.h"
#include "base/ddca_utils.h"
#include "nongui/ddc_error.h"
#include "nongui/vcpthread.h"

using namespace std;

static bool debugThread = false;

VcpThread::VcpThread(
        QObject*            parent,
        DDCA_Display_Info * dinfo,
        VcpRequestQueue*    requestQueue,
        FeatureBaseModel *  baseModel)
    : QThread(parent)
    , _dinfo(dinfo)
    , _requestQueue(requestQueue)
    , _baseModel(baseModel)
{
    _dref         = dinfo->dref;   // transitional

    _ddcaSimulator = new DdcaSimulator();

    // ddca_report_display_info(dinfo, 4);
    // ddca_dbgrpt_display_ref(_dref, 4);
}


#ifdef FUTURE
// a unified error report method
void VcpThread::rpt_ddca_error(
      const char * caller_name,
      const char * ddca_func_name,
      DDCA_Status  ddcrc,
      DDCA_Error_Detail * erec,
      int          feature_code           // -1 if no feature code
      )
{
   bool debug = true;
   if (feature_code >= 0) {
      uint8_t vco_code = feature_code & 0xff;
      TRACECF(debug, "In %s(), %s() for feature 0x%02x returned %d - %s",
                     caller_name, ddca_func_name, feature_code, ddcrc, ddca_rc_name(ddcrc));
   }
   else {
      TRACECF(debug, "In %s(), %s() returned %d - %s",
                     caller_name, ddca_func_name, ddcrc, ddca_rc_name(ddcrc));
   }

      DdcFeatureError* perec = new DdcFeatureError(feature_code, ddca_func_name, ddcrc);


}
#endif



void VcpThread::rpt_ddca_status(
      uint8_t      feature_code,
      const char * caller_name,
      const char * ddca_func_name,
      DDCA_Status  ddcrc)
{
    bool debug = false;
    TRACECF(debug, "In %s(), %s() for feature 0x%02x returned %d - %s",
                   caller_name, ddca_func_name, feature_code, ddcrc, ddca_rc_name(ddcrc));

    // QString msg = "generic error msg";
    // emit signalDdcFeatureError(0, msg);
    // DdcFeatureError erec(feature_code, ddca_func_name, ddcrc);

    DdcFeatureError* perec = new DdcFeatureError(feature_code, ddca_func_name, ddcrc);

    TRACECF(debug, "Built DdcFeatureError.  srepr: %s, sexpl: %s", QS2S(perec->repr()), QS2S(perec->expl()));
    // just call function, no need to signal:
    // postDdcFeatureError(erec);
    _baseModel->onDdcFeatureError(perec);
}


void VcpThread::rpt_error_detail(
      DDCA_Error_Detail * erec,
      const char * caller_name,
      const char * ddca_func_name)
{
   bool debugFunc = false;
   debugFunc = debugFunc ||debugThread;
   TRACECF(debugFunc, "In %s(), %s()  returned DDCA_Error_Detail with status %d - %s",
                         caller_name, ddca_func_name,
                         erec->status_code, ddca_rc_name(erec->status_code));

   QString smooshed = ddcutil_format_error_detail(erec, QString(""), 3);
   // TRACE("%s", smooshed.toLatin1().data() );

   DdcDetailedError* detailedError = new
         DdcDetailedError(ddca_func_name, erec->status_code, smooshed);
       // DdcDetailedError(ddca_func_name, erec->status_code, smooshed.toLatin1().data());

   TRACECF(debugFunc, "Built DdcDetailedError.  srepr: %s, sexpl: %s",
                         detailedError->srepr(), detailedError->sexpl());
   // just call function, no need to signal:
   _baseModel->onDdcDetailedError(detailedError);
}

#ifdef IN_PROGRESS
void VcpThread::rpt_getvcp_error(
      uin8_t              feature_code,
      DDCA_Error_Detail * erec,
      const char *        caller_name,
      const char *        ddca_func_name)
{
   QString("Error reading feature x%1.  Status code: %2"))
         .arg( QString::number(feature_code, 2, 16) )
         .arg( QString::fromAscii( ddca_rc_desc(erec->status_code) );

}
#endif


static uint8_t abs8(uint8_t v1, uint8_t v2) {
   uint8_t result;
   if (v1 >= v2)
      result = v1 - v2;
   else
      result = v2 - v1;
   return result;
}

static uint8_t max8(uint8_t v1, uint8_t v2) {
   if (v1 > v2)
      return v1;
   return v2;
}

void VcpThread::rpt_verify_error(
      uint8_t      featureCode,
      const char * function,
      uint8_t     expectedSh,
      uint8_t     expectedSl,
      uint8_t     observedSh,
      uint8_t     observedSl
      )
{
   VcpThread::rpt_verify_error(
         featureCode,
         function,
         expectedSh << 8 | expectedSl,
         observedSh << 8 | observedSl);
}


void VcpThread::rpt_verify_error(
      uint8_t      featureCode,
      const char * function,
      uint16_t     expectedValue,
      uint16_t     observedValue)
{
   bool debug = false;
   debug = debug || debugThread;
   TRACECF(debug, "featureCode=0x%02x, expectedValue=0x%04x, observedValue=0x%04x", featureCode, expectedValue, observedValue);
   // DdcVerifyError erec(featureCode, function, expectedValue, observedValue);
   DdcVerifyError* perec = new DdcVerifyError(featureCode, function, expectedValue, observedValue);
   // cout << erec.srepr() << endl;
   // cout << erec.sexpl() << endl;

   // uint8_t  deltaSh = 0;   // unused
   // uint8_t  deltaSl = 0;
   if (  max8(abs8(expectedValue >> 8,  observedValue >> 8),
              abs8(expectedValue & 0xff, observedValue & 0xff)
             ) <= 1)
   {
      TRACECF(debug, "difference <= 1, suppressing error");
   }
   else {
      TRACECF(debug, "Calling baseModel->onDdcFeatureError()");
      _baseModel->onDdcFeatureError(perec);
   }
}


DDCA_Status VcpThread::perform_open_display(DDCA_Display_Handle * dh_loc) {
   bool debugFunc = false;
   debugFunc = debugFunc || debugThread;

   TRACECF(debugFunc, "Starting. dref=%s", ddca_dref_repr(this->_dref));

   // DDCA_Display_Handle dh;

   DDCA_Status ddcrc = ddca_open_display2(this->_dref, false, dh_loc);
   if (ddcrc != 0) {
       rpt_ddca_status(0, __func__, "ddca_open_display2", ddcrc);
   }

   if (ddcrc == 0  && !_threadDescriptionPublished) {
      TRACECF(debugFunc, "calling ddca_set_thread_description()");
      // ddca_register_thread_dref(this->_dref);
      ddca_append_thread_description( ddca_dref_repr(this->_dref) );
      _threadDescriptionPublished = true;
   }
   TRACECF(debugFunc, "Returning %d", ddcrc);
   return ddcrc;
}


DDCA_Status VcpThread::perform_close_display(DDCA_Display_Handle dh) {
   bool debugFunc = false;
   debugFunc = debugFunc || debugThread;

   DDCA_Status ddcrc = ddca_close_display(dh);
   TRACECF(debugFunc, "ddca_close_display() returned %d, dref=%s", ddcrc, ddca_dref_repr(this->_dref));
   if (ddcrc != 0) {
      // n. returns DDCRC_INVALID_STATE if already closed
      rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
   }
   return ddcrc;
}


void VcpThread::loadDynamicFeatureRecords()
{
   bool debugFunc = debugThread;
   debugFunc = debugFunc || debugThread;
   TRACECF(debugFunc, "Starting. dref=%s", ddca_dref_repr(this->_dref));

   DDCA_Display_Handle dh;

   DDCA_Status ddcrc = perform_open_display(&dh);
   if (ddcrc == 0) {
      ddcrc = ddca_dfr_check_by_dh(dh);
      if (ddcrc != 0) {
         if (ddcrc == DDCRC_NOT_FOUND) {
            TRACECF(debugFunc, "ddca_dfr_check_by_dh() returned DDCRC_NOT_FOUND");
         }
         else {
            TRACECF(debugFunc, "ddca_dfr_check_by_dh() returned %s", ddca_rc_name(ddcrc));
            DDCA_Error_Detail * erec = ddca_get_error_detail();
            ddca_report_error_detail(erec, 4);

            rpt_error_detail(erec, "loadDynamicFeatureRecords", "ddca_dfr_check_by_dh");
            ddca_free_error_detail(erec);
         }
      }

      ddcrc = perform_close_display(dh);
   }

   TRACECF(debugFunc, "Done. dref=%s", ddca_dref_repr(this->_dref));
}


#ifdef TEST_ADJUST_RETRIES
void VcpThread::adjustRetries() {
   bool debugFunc = true;
   // TRACECF(debugFunc, "Starting");
   // quick and dirty just to test the functionality
   // TODO: Wrap in reads/writes in lock
   uint16_t writeReadMax = ddca_get_max_tries(DDCA_WRITE_READ_TRIES);
   uint16_t multiMax     = ddca_get_max_tries(DDCA_MULTI_PART_READ_TRIES);
   uint16_t newWriteReadMax  = (writeReadMax/2) + 1;
   uint16_t newMultiMax = (multiMax/2) + 1;
   TRACECF(debugFunc, "writeReadMax %d -> %d, multiMax %d -> %d",
                   writeReadMax, newWriteReadMax, multiMax, newMultiMax);
   ddca_set_max_tries(DDCA_WRITE_READ_TRIES, newWriteReadMax);
   ddca_set_max_tries(DDCA_MULTI_PART_READ_TRIES, newMultiMax);
}
#endif


// Process RQCapabilities
void VcpThread::capabilities() {
   bool debugFunc = debugThread;
   debugFunc = false;
   bool debugRetry = false;
   debugFunc = debugFunc || debugThread;
   debugRetry = debugRetry || debugFunc;
   // debugFunc = false;
   TRACECF(debugFunc, "Starting. dref=%s", ddca_dref_repr(this->_dref));
   DDCA_Display_Handle dh;
   char *              caps = NULL;
   DDCA_Capabilities * parsed_caps = NULL;
   int retry_count = 0;

   DDCA_Status ddcrc = perform_open_display(&dh);
   if (ddcrc == 0) {
      // TRACEF(debugFunc,"Sleeping for 1000000 msec");
      // usleep(1000000);
      bool retryable = true;

      // Why retrying here?  Retries already occurred in libddcutil? (4/2020)
      while (retryable) {
         retryable = false;
         ddcrc = ddca_get_capabilities_string(dh, &caps);
         if (ddcrc != 0) {
            TRACECF(debugRetry, "Error getting capabilities string for %s, ddcrc=%s",
                                ddca_dref_repr(this->_dref), ddca_rc_name(ddcrc));
            // DDCA_Error_Detail * err_detail =  ddca_get_error_detail();
            // ddca_report_error_detail(err_detail, 2);
            double curmult = ddca_get_sleep_multiplier();
            if (ddcrc == -EBADF) {
               TRACEC("EBADF");
               continue;
            }

            if (curmult <= 2.0f) {    // retry possible
               curmult = curmult * 2;
               TRACECF(debugRetry, "Adjusting thread sleep multiplier for %s to %5.2f",
                                  ddca_dref_repr(this->_dref), curmult);
               // todo: output message in status bar that retrying
               // RETRYING DOESN"T HELP
               // ddca_set_sleep_multiplier(curmult);
               // adjustRetries();
               retryable = true;
               retry_count++;
            }   // end, ddca_get_capabilities() failure, retry possible
            else {  // failure, can't retry
               if (retry_count > 0)
                  TRACECF(debugRetry || true, "Capabilities check failed after %d retries, retries exhausted", retry_count);
               rpt_ddca_status(0, __func__, "ddca_get_capabilities_string", ddcrc);
            }  // end, failure, can't retry
         }  // ddca_get_capabilities() failed
         else if (retry_count > 0) {
            TRACECF(debugRetry || true, "Capabilities check succeeded after %d retries", retry_count);
         }
      } // end of while() loop calling ddca_get_capabilies()

      if (ddcrc == 0) {   // ddca_get_capabilities() succeeded, try to parse
         ddcrc = ddca_parse_capabilities_string(caps, &parsed_caps);
         if (ddcrc != 0)
            rpt_ddca_status(0, __func__, "ddca_parse_capabilities_string", ddcrc);
      }
      // rpt_ddca_status(0, __func__, "dummy function", -99);   // *** TEMP ***
      // if ddcrc != 0, caps may be NULL, parsed_caps definitely NULL
      // whatever the case, tell _baseModel the result
      _baseModel->setCapabilities(ddcrc, caps, parsed_caps);

      if (ddcrc == -EBADF) {
         TRACECF(debugFunc, "ddca_get_capabilities_string() returned -EBADF. Skipping close. dh=%s",
                            ddca_dref_repr(this->_dref));
      }
      else {
         TRACECF(debugFunc, "Closing %s",  ddca_dref_repr(this->_dref) );
         ddcrc = perform_close_display(dh);
      }
   }  // open succeeded

   TRACECF(debugFunc, "Done.     dref=%s, retry_count=%d", ddca_dref_repr(this->_dref), retry_count);
} // function


// Process RQGetVcp
void VcpThread::getvcp(uint8_t featureCode, bool needMetadata) {
    bool debugFunc = false;  //  || (featureCode == 0x14);
    debugFunc = debugFunc || debugThread;
    TRACECF(debugFunc, "Starting. featureCode=0x%02x, needMetadata = %s",
                      featureCode, SBOOL(needMetadata));

    DDCA_Display_Handle                   dh;
    DDCA_Non_Table_Vcp_Value              valrec;
    DDCA_Feature_Metadata *               finfo;

    DDCA_Status ddcrc = perform_open_display(&dh);
    if (ddcrc == 0) {
       QString msg;
       _baseModel->setStatusMsg(msg.asprintf("Reading feature 0x%02x",featureCode));

       bool simulated = _ddcaSimulator->simulateGetNonTableVcpValue(
                           _dinfo->vcp_version,
                           dh,
                           featureCode,
                           &valrec,
                           &ddcrc);
       if (!simulated) {
          // TRACEC("non-simulated");
          ddcrc = ddca_get_non_table_vcp_value(dh, featureCode, &valrec);
       }
       TRACECF(debugFunc, "feature_code=0x%02x, ddca_get_non_table_vcp_value() returned %d - %s",
                  featureCode, ddcrc, ddca_rc_name(ddcrc));
       // don't need to call rpt_ddca_status() if error, error will be reported
       // in the value field for the feature code
#ifdef OLD
        if (ddcrc != 0) {

           if (ddcrc != DDCRC_REPORTED_UNSUPPORTED) {
              rpt_ddca_status(featureCode, __func__, "ddca_get_nontable_vcp_value", ddcrc);
           }
        }
        else {
            // printf("ddca_get_nontable_vcp_value() returned:\n");
            // printf("  opcode:   0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
            //        feature_code, valrec.mh, valrec.ml, valrec.sh, valrec.sl);
        }
#endif
      // if (ddcrc == 0) {  // if get_nontable_vcp_value() succeeded
           // TODO:  get metadata once and cache
           DDCA_Status ddcrc2 = ddca_get_feature_metadata_by_dh(
                      featureCode,
                      dh,
                      true,       // create_default_if_not_found
                      &finfo);
           TRACECF(debugFunc, "ddca_get_feature_metadata_by_dh() for feature 0x%02x returned %d - %s",
                     featureCode, ddcrc2, ddca_rc_name(ddcrc2));
           // if (featureCode == 0xdf || featureCode == 0xf4 || featureCode == 0xf5)
           //       ddca_dbgrpt_feature_metadata(finfo, 1);

           // if (debugFunc && ddcrc2 == 0) {
           //     // OLD: ddcui_dbgrpt_ddca_feature_metadata(finfo);
           //    ddca_dbgrpt_feature_metadata(finfo, 1);
           // }
           if (ddcrc2 != 0) {
              rpt_ddca_status(featureCode, __func__, "ddca_get_feature_metadata_by_dh",  ddcrc);
              // cout << "ddca_get_feature_metadata() returned " << ddcrc << endl;
           }

           // whether or not succeeded, set feature info in  _baseModel so FeatureValueWidget can display error
           _baseModel->modelVcpValueSet(featureCode, this->_dref, finfo, &valrec, ddcrc);

           _baseModel->setFeatureChecked(featureCode);
       // }  // end, get value succeeded


       ddcrc = perform_close_display(dh);
    }  // open succeeded
    TRACECF(debugThread, "Done");
}


// Process RQSetVcp
void VcpThread::setvcp(uint8_t feature_code, bool writeOnly, uint16_t shsl)
{
    bool debugFunc = false; // || (feature_code == 0x14);
    debugFunc = debugFunc || debugThread;
    TRACECF(debugFunc, "Starting. feature_code=0x%02x.  shsl=0x%04x, writeOnly=%s",
                       feature_code, shsl, SBOOL(writeOnly));

    uint8_t sh = (shsl >> 8);
    uint8_t sl = (shsl & 0xff);
    TRACECF(debugFunc, "sh: 0x%02x, sl: 0x%02x", sh, sl);
    // rpt_ddca_status(feature_code, __func__, "ddca_bogus", 0);

    DDCA_Display_Handle dh;
    DDCA_Status ddcrc = perform_open_display(&dh);
    if (ddcrc == 0) {
       ddca_enable_verify(false);
       // TRACE( "ddca_is_verify_enabled() returned: %s", SBOOL( ddca_is_verify_enabled()));
       // ddca_enable_verify(!writeOnly);
       // uint8_t verified_hi_byte = 0;   // unused
       // uint8_t verified_lo_byte = 0;   // unused
       // need to update mh, ml, use a valrec
       // ddcrc = ddca_set_non_table_vcp_value_verify(dh, feature_code, 0, sl, &verified_hi_byte, &verified_lo_byte);

       bool simulated = _ddcaSimulator->simulateSetNonTableVcpValue(
                           _dinfo->vcp_version,
                           dh,
                           feature_code,
                           sh,
                           sl,
                           &ddcrc);
       if (!simulated) {
          // TRACEC("non-simulated");
          ddcrc = ddca_set_non_table_vcp_value(dh, feature_code, sh, sl);
       }
       if (ddcrc != 0) {
          TRACECF(debugFunc, "ddca_set_non_table_vcp_value() returned %d - %s", ddcrc, ddca_rc_name(ddcrc));
          rpt_ddca_status(feature_code, __func__, "ddca_set_non_table_vcp_value", ddcrc);
          goto bye;
       }
       if (!writeOnly) {
           DDCA_Non_Table_Vcp_Value  valrec;

           bool simulated = _ddcaSimulator->simulateGetNonTableVcpValue(
                                   _dinfo->vcp_version,
                                   dh,
                                   feature_code,
                                   &valrec,
                                   &ddcrc);
           if (!simulated) {
               // TRACEC("non-simulated");
               ddcrc = ddca_get_non_table_vcp_value(dh, feature_code, &valrec);
           }
           if (ddcrc != 0) {
                rpt_ddca_status(feature_code, __func__, "ddca_get_nontable_vcp_value", ddcrc);
           }
           else {
              TRACECF(debugFunc, "ddca_get_nontable_vcp_value() after ddca_set_non_table_vcp_value():");
              TRACECF(debugFunc, "  opcode: 0x%02x, requested: sh=0x%02x, sl=0x%02x, observed: mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x",
                         feature_code, sh, sl, valrec.mh, valrec.ml, valrec.sh, valrec.sl);

              if ((sl != valrec.sl || sh != valrec.sh)) {
                 // TRACE("Calling rpt_verify_error()");
                 rpt_verify_error(feature_code, "ddca_set_non_table_vcp_value", sh, sl, valrec.sh, valrec.sl);
              }    // ddca_set_non_table_vcp_value() succeeded
              // 10/2019 coming here even if verify error???
              TRACECF(debugFunc, "Calling _baseModel->modelVcpValueUpdate()");
              _baseModel->modelVcpValueUpdate(feature_code, valrec.sh, valrec.sl);
           }  // ddca_get_non_table_vcp_value() succeeded
       }     // !writeOnly

bye:
       ddcrc = ddca_close_display(dh);
       TRACECF(debugFunc, "ddca_close_display() returned %d", ddcrc);
       if (ddcrc != 0) {
           TRACECF(debugFunc, "ddca_close_display() returned %d", ddcrc);
           rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
       }
    }   // open succeeded
    TRACECF(debugFunc, "Done");
}


// Process RQStartInitailLoad
void VcpThread::startInitialLoad(void) {
    TRACECF(debugThread, "Executing");
    // _baseModel->beginResetModel();
    _baseModel->modelStartInitialLoad();
#ifdef UNUSED
    _baseModel->modelMccsVersionSet(_dinfo->vcp_version);
#endif

}


// Process RQEndInitialLoad
void VcpThread::endInitialLoad(void) {
    TRACECF(debugThread, "Starting");
    // _baseModel->report();
    // _baseModel->endResetModel();
    _baseModel->modelEndInitialLoad();
    // emit signalStatusMsg(QString("Loading complete"));
    _baseModel->setStatusMsg(QString("Loading complete"));
}


void VcpThread::run() {
    forever {
        VcpRequest * rqst = this->_requestQueue->pop();
        switch(rqst->_type) {
        case VcpRequestType::RQGetVcp:
        {
            VcpGetRequest* getRqst = static_cast<VcpGetRequest*>(rqst);
            // printf("(VcpThread::run) VcpGetRequest. feature_code=0x%02x\n", getRqst->_featureCode);
            // TRACE("VcpGetRequest. feature code = 0x%02x", getRqst->_featureCode);
            getvcp(getRqst->_featureCode, getRqst->_needMetadata);
            break;
        }
        case VcpRequestType::RQSetVcp:
        {
            VcpSetRequest* setRqst = static_cast<VcpSetRequest*>(rqst);
            // if (debugThread)
            //     printf("(VcpThread::run) RQSetVcp. feature code=0x%02x, newval=%d\n",
            //            setRqst->_featureCode, setRqst->_newval);  fflush(stdout);
            uint16_t newval = (setRqst->_newSh << 8) | (setRqst->_newSl );
            setvcp(setRqst->_featureCode, setRqst->_writeOnly, newval);
            break;
        }
        case VcpRequestType::RQStartInitialLoad:
            startInitialLoad();
            break;
        case VcpRequestType::RQEndInitialLoad:
            endInitialLoad();
            break;
        case VcpRequestType::RQCapabilities:
            capabilities();
            break;
        case VcpRequestType::RQLoadDfr:
            loadDynamicFeatureRecords();
            break;
        default:
            cout << "Unexpected request type: " << rqst->_type << endl;
        }
    }
}

