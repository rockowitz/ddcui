/* vcpthread.cpp */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <iostream>
#include <string.h>

#include "ddcutil_status_codes.h"  // using quotes allows Eclipse to find in workspace
#include "ddcutil_c_api.h"

#include <QtCore/QString>

#include "base/ddca_utils.h"
#include "base/ddcui_globals.h"
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

    // ddca_report_display_info(dinfo, 4);
    // ddca_dbgrpt_display_ref(_dref, 4);
}


void VcpThread::rpt_ddca_status(
      uint8_t      feature_code,
      const char * caller_name,
      const char * ddca_func_name,
      DDCA_Status  ddcrc)
{
    TRACE("In %s(), %s() for feature 0x%02x returned %d - %s",
           caller_name, ddca_func_name, feature_code, ddcrc, ddca_rc_name(ddcrc));

    // QString msg = "generic error msg";
    // emit signalDdcFeatureError(0, msg);
    // DdcFeatureError erec(feature_code, ddca_func_name, ddcrc);

    DdcFeatureError* perec = new DdcFeatureError(feature_code, ddca_func_name, ddcrc);

    // printf("(VcpThread::rpt_ddca_status) Woff\n"); fflush(stdout);
    // TRACE("Built DdcFeatureError.  srepr: %s, sexpl: %s", qs2s(perec->repr()), qs2s(perec->expl()));
    // just call function, no need to signal:
    // postDdcFeatureError(erec);
    _baseModel->onDdcFeatureError(perec);

    // postError(msg);
    // fflush(stdout);
}



void VcpThread::rpt_error_detail(
      DDCA_Error_Detail * erec,
      const char * caller_name,
      const char * ddca_func_name)
{
   bool debugFunc = debugThread;
   // debugFunc = true;
   TRACEF(debugFunc, "In %s(), %s()  returned DDCA_Error_Detail with status %d - %s",
                         caller_name, ddca_func_name,
                         erec->status_code, ddca_rc_name(erec->status_code));

   QString smooshed = format_error_detail(erec, QString(""), 3);
   // TRACE("%s", smooshed.toLatin1().data() );

   DdcDetailedError* detailedError = new
         DdcDetailedError(ddca_func_name, erec->status_code, smooshed);
       // DdcDetailedError(ddca_func_name, erec->status_code, smooshed.toLatin1().data());

   TRACEF(debugFunc, "Built DdcDetailedError.  srepr: %s, sexpl: %s",
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
   TRACEF(debug, "featureCode=0x%02x, expectedValue=0x%04x, observedValue=0x%04x", featureCode, expectedValue, observedValue);
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
      TRACEF(debug, "difference <= 1, suppressing error");
   }
   else {
      TRACEF(debug, "Calling baseModel->onDdcFeatureError()");
      _baseModel->onDdcFeatureError(perec);
   }
}


void VcpThread::loadDynamicFeatureRecords()
{
   bool debugFunc = debugThread;
   // debugFunc = true;
   TRACEF(debugFunc, "Starting. dref=%s", ddca_dref_repr(this->_dref));

   DDCA_Display_Handle dh;

   DDCA_Status ddcrc = ddca_open_display2(this->_dref, false, &dh);
   if (ddcrc != 0) {
       rpt_ddca_status(0, __func__, "ddca_open_display2", ddcrc);
   }
   else {
      ddcrc = ddca_dfr_check_by_dh(dh);
      if (ddcrc != 0) {
         if (ddcrc == DDCRC_NOT_FOUND) {
            TRACEF(debugFunc, "ddca_dfr_check_by_dh() returned DDCRC_NOT_FOUND");
         }
         else {
            TRACEF(debugFunc, "ddca_dfr_check_by_dh() returned %s", ddca_rc_name(ddcrc));
            DDCA_Error_Detail * erec = ddca_get_error_detail();
            ddca_report_error_detail(erec, 4);

            rpt_error_detail(erec, "loadDynamicFeatureRecords", "ddca_dfr_check_by_dh");
            ddca_free_error_detail(erec);
         }
      }

      ddcrc = ddca_close_display(dh);
      if (ddcrc != 0) {
          rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
      }
   }

   TRACEF(debugFunc, "Done. dref=%s", ddca_dref_repr(this->_dref));
}


// Process RQCapabilities
void VcpThread::capabilities() {
   bool debugFunc = debugThread;
   // debugFunc = false;
   TRACEF(debugFunc, "Starting. dref=%s", ddca_dref_repr(this->_dref));
   DDCA_Display_Handle dh;
   char *              caps = NULL;
   DDCA_Capabilities * parsed_caps = NULL;

   DDCA_Status ddcrc = ddca_open_display2(this->_dref, false, &dh);
   if (ddcrc != 0) {
         rpt_ddca_status(0, __func__, "ddca_open_display2", ddcrc);
   }
   else {
#ifdef MOVED
      // TEMPORARY LOCATION - SHOULD BE A SEPARATE RQCheckDFR
      ddcrc = ddca_dfr_check_by_dh(dh);
      if (ddcrc != 0) {
         TRACEF(debugFunc, "ddca_dfr_check_by_dh() returned %s", ddca_rc_name(ddcrc));
         DDCA_Error_Detail * erec = ddca_get_error_detail();
         ddca_report_error_detail(erec, 1);
         ddca_free_error_detail(erec);
      }
#endif

      // TRACEF(debugFunc,"Sleeping for 1000000 msec");
      // usleep(1000000);
      ddcrc = ddca_get_capabilities_string(dh, &caps);
      if (ddcrc != 0) {
         TRACE("Error getting capabilities string for %s", ddca_dref_repr(this->_dref));
         // DDCA_Error_Detail * err_detail =  ddca_get_error_detail();
         // ddca_report_error_detail(err_detail, 2);
         rpt_ddca_status(0, __func__, "ddca_get_capabilities_string", ddcrc);
      }
      else {
         ddcrc = ddca_parse_capabilities_string(caps, &parsed_caps);
         if (ddcrc != 0)
            rpt_ddca_status(0, __func__, "ddca_parse_capabilities_string", ddcrc);
      }

      ddcrc = ddca_close_display(dh);
      if (ddcrc != 0) {
          rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
      }
   }
   _baseModel->setCapabilities(ddcrc, caps, parsed_caps);

   TRACEF(debugFunc, "Done. dref=%s", ddca_dref_repr(this->_dref));
}


// Process RQGetVcp
void VcpThread::getvcp(uint8_t featureCode, bool needMetadata, bool reportUnsupported) {
    bool debugFunc = debugThread;
    // debugFunc = true;
    TRACEF(debugFunc||true, "Starting. featureCode=0x%02x, needMetadata = %s, reportUnsupported=%s",
                            featureCode, sbool(needMetadata), sbool(reportUnsupported));

    DDCA_Display_Handle                   dh;
    DDCA_Non_Table_Vcp_Value              valrec;
    DDCA_Feature_Metadata *               finfo;

    DDCA_Status ddcrc = ddca_open_display2(this->_dref, false, &dh);
    if (ddcrc != 0) {
          rpt_ddca_status(0, __func__, "ddca_open_display2", ddcrc);
    }
    else {
       QString msg;
       _baseModel->setStatusMsg(msg.sprintf("Reading feature 0x%02x",featureCode));

        ddcrc = ddca_get_non_table_vcp_value(dh, featureCode, &valrec);
        TRACEF(debugFunc, "feature_code=0x%02x, ddca_get_non_table_vcp_value() returned %d - %s",
                  featureCode, ddcrc, ddca_rc_name(ddcrc));
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

        if (ddcrc == 0) {
           // should this be here?
           ddcrc = ddca_get_feature_metadata_by_dh(
                      featureCode,
                      dh,
                      true,       // create_default_if_not_found
                      &finfo);
           TRACEF(debugFunc, "ddca_get_feature_metadata_by_dh() for feature 0x%02x returned %d - %s",
                     featureCode, ddcrc, ddca_rc_name(ddcrc));
           if (debugFunc && ddcrc == 0) {
              // ddcui_dbgrpt_ddca_feature_metadata(finfo);
              ddca_dbgrpt_feature_metadata(finfo, 1);
           }

           if (ddcrc != 0) {
              rpt_ddca_status(featureCode, __func__, "ddca_get_feature_metadata_by_dh",  ddcrc);
              // cout << "ddca_get_feature_metadata() returned " << ddcrc << endl;
           }
        }

        // if (ddcrc == 0) {
           _baseModel->modelVcpValueSet(featureCode, this->_dref, finfo, &valrec, ddcrc);
        // }
        _baseModel->setFeatureChecked(featureCode);

        ddcrc = ddca_close_display(dh);
        if (ddcrc != 0) {
           rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
        }
    }
    TRACEF(debugThread, "Done");
}


// Process RQSetVcp
void VcpThread::setvcp(uint8_t feature_code, bool writeOnly, uint16_t shsl)
{
    bool debugFunc = debugThread;
    // debugFunc = true;
    TRACEF(debugFunc,
              "Starting. feature_code=0x%02x.  shsl=0x%04x, writeOnly=%s",
              feature_code, shsl, sbool(writeOnly));

    uint8_t sh = (shsl >> 8);
    uint8_t sl = (shsl & 0xff);
    TRACEF(debugFunc, "sh: 0x%02x, sl: 0x%02x", sh, sl);
    // rpt_ddca_status(feature_code, __func__, "ddca_bogus", 0);

    DDCA_Display_Handle dh;
    DDCA_Status ddcrc = ddca_open_display2(this->_dref, false, &dh);
    if (ddcrc != 0) {
        TRACEF(debugFunc, "ddca_open_display2() returned %d", ddcrc);
        rpt_ddca_status(feature_code, __func__, "ddca_open_display2", ddcrc);
        goto bye;
    }

    ddca_enable_verify(false);
    // TRACE( "ddca_is_verify_enabled() returned: %s", sbool( ddca_is_verify_enabled()));
    // ddca_enable_verify(!writeOnly);
    // uint8_t verified_hi_byte = 0;   // unused
    // uint8_t verified_lo_byte = 0;   // unused
    // need to update mh, ml, use a valrec
    // ddcrc = ddca_set_non_table_vcp_value_verify(dh, feature_code, 0, sl, &verified_hi_byte, &verified_lo_byte);

    ddcrc = ddca_set_non_table_vcp_value(dh, feature_code, sh, sl);
    if (ddcrc != 0) {
        TRACEF(debugFunc, "ddca_set_non_table_vcp_value() returned %d - %s", ddcrc, ddca_rc_name(ddcrc));
        rpt_ddca_status(feature_code, __func__, "ddca_set_non_table_vcp_value", ddcrc);
        goto bye;
    }
//    else {
       if (!writeOnly) {
            DDCA_Non_Table_Vcp_Value  valrec;
            ddcrc = ddca_get_non_table_vcp_value(dh, feature_code, &valrec);
            if (ddcrc != 0) {
                rpt_ddca_status(feature_code, __func__, "ddca_get_nontable_vcp_value", ddcrc);
            }
            else {
                TRACEF(debugFunc, "ddca_get_nontable_vcp_value() after ddca_set_non_table_vcp_value():");
                TRACEF(debugFunc, "  opcode: 0x%02x, requested: sh=0x%02x, sl=0x%02x, observed: mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x",
                         feature_code, sh, sl, valrec.mh, valrec.ml, valrec.sh, valrec.sl);

                if ((sl != valrec.sl || sh != valrec.sh)) {
                   // TRACE("Calling rpt_verify_error()");
                   rpt_verify_error(feature_code, "ddca_set_non_table_vcp_value", sh, sl, valrec.sh, valrec.sl);
                }
                // 10/2019 coming here even if verify error???
                TRACEF(debugFunc, "Calling _baseModel->modelVcpValueUpdate()");
                _baseModel->modelVcpValueUpdate(feature_code, valrec.sh, valrec.sl);
            }
       }
  //  }

    ddcrc = ddca_close_display(dh);
    if (ddcrc != 0) {
        TRACEF(debugFunc, "ddca_close_display() returned %d", ddcrc);
        rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
    }

bye:
   ;
    TRACEF(debugFunc, "Done");
}


// Process RQStartInitailLoad
void VcpThread::startInitialLoad(void) {
    TRACEF(debugThread, "Executing");
    // _baseModel->beginResetModel();
    _baseModel->modelStartInitialLoad();
#ifdef UNUSED
    _baseModel->modelMccsVersionSet(_dinfo->vcp_version);
#endif

}


// Process RQEndInitialLoad
void VcpThread::endInitialLoad(void) {
    TRACEF(debugThread, "Starting");
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
            TRACE("VcpGetRequest. feature code = 0s%02x", getRqst->_featureCode);
            getvcp(getRqst->_featureCode, getRqst->_needMetadata, getRqst->_reportUnsupported);
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

