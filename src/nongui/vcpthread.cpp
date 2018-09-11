/* vcpthread.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <iostream>
#include <string.h>

#include <ddcutil_status_codes.h>

#include <QtCore/QString>

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
    PRINTFTCM("In %s(), %s() for feature 0x%02x returned %d - %s",
           caller_name, ddca_func_name, feature_code, ddcrc, ddca_rc_name(ddcrc));

    // QString msg = "generic error msg";
    // emit signalDdcError(0, msg);
    // DdcError erec(feature_code, ddca_func_name, ddcrc);
    DdcError* perec = new DdcError(feature_code, ddca_func_name, ddcrc);
    // printf("(VcpThread::rpt_ddca_status) Woff\n"); fflush(stdout);
    PRINTFTCM("Built DdcError.  srepr: %s, sexpl: %s", perec->srepr(), perec->sexpl());
    // just call function, no need to signal:
    // postDdcError(erec);
    _baseModel->onDdcError(perec);

    // postError(msg);
    // fflush(stdout);
}

void VcpThread::rpt_verify_error(
      uint8_t      featureCode,
      const char * function,
      uint8_t      expectedValue,
      uint8_t      observedValue)
{
   PRINTFTCM("featureCode=0x%02x, expectedValue=0x%02x, observedValue=0x%02x", featureCode, expectedValue, observedValue);
   // DdcVerifyError erec(featureCode, function, expectedValue, observedValue);
   DdcVerifyError* perec = new DdcVerifyError(featureCode, function, expectedValue, observedValue);
   // cout << erec.srepr() << endl;
   // cout << erec.sexpl() << endl;
   _baseModel->onDdcError(perec);
}


// Process RQCapabilities
void VcpThread::capabilities() {
   PRINTFTCMF(debugThread, "Starting. dref=%s", ddca_dref_repr(this->_dref));
   DDCA_Display_Handle dh;
   char *              caps = NULL;
   DDCA_Capabilities * parsed_caps = NULL;

   // need better way to report failure
   DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
   if (ddcrc != 0) {
         rpt_ddca_status(0, __func__, "ddca_open_display", ddcrc);
         // how to handle?
   }
   else {
      ddcrc = ddca_get_capabilities_string(dh, &caps);
      if (ddcrc != 0) {
         rpt_ddca_status(0, __func__, "ddca_get_capabilities_string", ddcrc);
      }
      else {
         ddcrc = ddca_parse_capabilities_string(caps, &parsed_caps);
         if (ddcrc != 0)
            rpt_ddca_status(0, __func__, "ddca_parse_capabilities_string", ddcrc);
      }

      // how to report failure?
      ddcrc = ddca_close_display(dh);
      if (ddcrc != 0) {
          rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
          // how to handle?
      }
   }
   _baseModel->setCapabilities(ddcrc, caps, parsed_caps);

   PRINTFTCMF(debugThread, "Done. dref=%s", ddca_dref_repr(this->_dref));
}


// Process RQGetVcp
void VcpThread::getvcp(uint8_t feature_code) {
    PRINTFTCMF(debugThread, "Starting. feature_code=0x%02x", feature_code);

    DDCA_Display_Handle                   dh;
    DDCA_Non_Table_Vcp_Value              valrec;
    DDCA_Feature_Metadata                 finfo;

    DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
    if (ddcrc != 0) {
          rpt_ddca_status(0, __func__, "ddca_open_display", ddcrc);
          // how to handle?
    }
    else {
       QString msg;
       _baseModel->setStatusMsg(msg.sprintf("Reading feature 0x%02x",feature_code));

        ddcrc = ddca_get_non_table_vcp_value(dh, feature_code, &valrec);
        PRINTFTCMF(debugThread, "feature_code=0x%02x, ddca_get_non_table_vcp_value() returned %d - %s",
                  feature_code, ddcrc, ddca_rc_name(ddcrc));
        if (ddcrc != 0) {
           if (ddcrc != DDCRC_REPORTED_UNSUPPORTED)
            rpt_ddca_status(feature_code, __func__, "ddca_get_nontable_vcp_value", ddcrc);
            // how to handle?
        }
        else {
            // printf("ddca_get_nontable_vcp_value() returned:\n");
            // printf("  opcode:   0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
            //        feature_code, valrec.mh, valrec.ml, valrec.sh, valrec.sl);
        }

        if (ddcrc == 0) {
           // should this be here?
           ddcrc = ddca_get_feature_metadata_by_dh(
                      feature_code,
                      dh,
                      true,       // create_default_if_not_found
                      &finfo);
           if (ddcrc != 0) {
              rpt_ddca_status(feature_code, __func__, "ddca_get_feature_metadata_by_dh",  ddcrc);
              // cout << "ddca_get_feature_metadata() returned " << ddcrc << endl;
           }
        }

        if (ddcrc == 0) {
           _baseModel->modelVcpValueSet(feature_code, this->_dref, finfo, &valrec);
        }
        _baseModel->setFeatureChecked(feature_code);

        ddcrc = ddca_close_display(dh);
        if (ddcrc != 0) {
           rpt_ddca_status(0, __func__, "ddca_close_display", ddcrc);
           // how to handle?
        }
    }
    // PRINTFTCMF(debugThread, "Done");
}


// Process RQSetVcp
void VcpThread::setvcp(uint8_t feature_code, bool writeOnly, uint8_t sl)
{
    bool debugFunc = debugThread;
    debugFunc = true;
    PRINTFTCMF(debugFunc,
              "Starting. feature_code=0x%02x. sl=0x%02x, writeOnly=%s",
              feature_code, sl, sbool(writeOnly));

    // rpt_ddca_status(feature_code, __func__, "ddca_bogus", 0);

    DDCA_Display_Handle         dh;
    DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
    if (ddcrc != 0) {
        PRINTFTCM("ddca_open_display() returned %d", ddcrc);
        // cout << "ddca_open_display() returned " << ddcrc << endl;
        rpt_ddca_status(feature_code, __func__, "ddca_open_display", ddcrc);
        goto bye;
    }

    ddca_enable_verify(false);
    // PRINTFTCM( "ddca_is_verify_enabled() returned: %s", sbool( ddca_is_verify_enabled()));
    // ddca_enable_verify(!writeOnly);
    // uint8_t verified_hi_byte = 0;   // unused
    // uint8_t verified_lo_byte = 0;   // unused
    // need to update mh, ml, use a valrec
    // ddcrc = ddca_set_non_table_vcp_value_verify(dh, feature_code, 0, sl, &verified_hi_byte, &verified_lo_byte);

    ddcrc = ddca_set_non_table_vcp_value(dh, feature_code, 0, sl);
    if (ddcrc != 0) {
        PRINTFTCM("ddca_set_non_table_vcp_value() returned %d - %s", ddcrc, ddca_rc_name(ddcrc));
        rpt_ddca_status(feature_code, __func__, "ddca_set_non_table_vcp_value", ddcrc);
    }
    else {
       if (!writeOnly) {
            DDCA_Non_Table_Vcp_Value  valrec;
            ddcrc = ddca_get_non_table_vcp_value(dh, feature_code, &valrec);
            if (ddcrc != 0) {
                rpt_ddca_status(feature_code, __func__, "ddca_get_nontable_vcp_value", ddcrc);
            }
            else {
                PRINTFTCM("ddca_get_nontable_vcp_value() after ddca_set_non_table_vcp_value():");
                PRINTFTCM("  opcode: 0x%02x, requested sl=0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x",
                         feature_code, sl, valrec.mh, valrec.ml, valrec.sh, valrec.sl);
                if (sl != valrec.sl) {
                   rpt_verify_error(feature_code, "ddca_set_non_table_vcp_value", sl, valrec.sl);
                }
                PRINTFTCM("Calling _baseModel->modelVcpValueUpdate()");
                _baseModel->modelVcpValueUpdate(feature_code, valrec.sh, valrec.sl);
            }
       }
    }

    ddcrc = ddca_close_display(dh);
    if (ddcrc != 0) {
        cout << "ddca_close_display() returned " << ddcrc << endl;
        // how to handle?
    }

bye:
   ;
    // printf("(VcpThread::getvcp) Done\n");
}


// Process RQStartInitailLoad
void VcpThread::startInitialLoad(void) {
    PRINTFTCMF(debugThread, "Executing");
    // _baseModel->beginResetModel();
    _baseModel->modelStartInitialLoad();
#ifdef UNUSED
    _baseModel->modelMccsVersionSet(_dinfo->vcp_version);
#endif

}


// Process RQEndInitialLoad
void VcpThread::endInitialLoad(void) {
    PRINTFTCMF(debugThread, "Starting");
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
            getvcp(getRqst->_featureCode);
            break;
        }
        case VcpRequestType::RQSetVcp:
        {
            VcpSetRequest* setRqst = static_cast<VcpSetRequest*>(rqst);
            // if (debugThread)
            //     printf("(VcpThread::run) RQSetVcp. feature code=0x%02x, newval=%d\n",
            //            setRqst->_featureCode, setRqst->_newval);  fflush(stdout);
            setvcp(setRqst->_featureCode, setRqst->_writeOnly, setRqst->_newval & 0xff);
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
        default:
            cout << "Unexpected request type: " << rqst->_type << endl;
        }
    }
}

