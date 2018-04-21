/* vcpthread.cpp */

#include <iostream>
#include <string.h>

#include <ddcutil_status_codes.h>

#include <QtCore/QString>

#include "nongui/ddc_error.h"
#include "nongui/vcpthread.h"

using namespace std;

static bool debugThread;

VcpThread::VcpThread(
        QObject*            parent,
        DDCA_Display_Info * dinfo,
        VcpRequestQueue*    requestQueue,
        FeatureBaseModel *  baseModel)
    : QThread(parent)
{
    _dinfo        = dinfo;
    _dref         = dinfo->dref;   // transitional
    _baseModel    = baseModel;
    _requestQueue = requestQueue;
    // open the display, raise exception if error


    ddca_dbgrpt_display_info(dinfo, 4);
    ddca_dbgrpt_display_ref(_dref, 4);
}


void VcpThread::rpt_ddca_status(
      uint8_t      feature_code,
      const char * caller_name,
      const char * ddca_func_name,
      DDCA_Status  ddcrc)
{
    printf("(VcpThread::%s) %s() returned %d - %s\n",
           caller_name, ddca_func_name, ddcrc, ddca_rc_name(ddcrc));
    QString msg = "generic error msg";
    // emit signalDdcError(0, msg);
    DdcError erec(feature_code, ddca_func_name, ddcrc);
    // just call function, no need to signal:
    // postDdcError(erec);
    _baseModel->onDdcError(erec);

    // postError(msg);
    fflush(stdout);
}

// Process RQCapabilities
void VcpThread::capabilities() {
   printf("(VcpThread::capabilities) Starting. dref=%s\n", ddca_dref_repr(this->_dref));
   DDCA_Display_Handle dh;
   char *              caps = NULL;
   DDCA_Capabilities * parsed_caps = NULL;

   // need better way to report failure
   DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
   if (ddcrc != 0) {
         rpt_ddca_status(0, __func__, "ddca_open_display", ddcrc);
         // how to handle?
   }
   if (ddcrc == 0) {
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

   printf("(VcpThread::capabilities) Done. dref=%s\n", ddca_dref_repr(this->_dref));
}


// Process RQGetVcp
void VcpThread::getvcp(uint8_t feature_code) {
    // printf("(VcpThread::getvcp) Starting. feature_code=0x%02x\n", feature_code);
    DDCA_Display_Handle                   dh;
    DDCA_Non_Table_Vcp_Value              valrec;
    DDCA_Feature_Metadata                 finfo;

    DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
    if (ddcrc != 0) {
          rpt_ddca_status(0, __func__, "ddca_open_display", ddcrc);
          // how to handle?
    }

    if (ddcrc == 0) {
       QString msg;
       //  msg.sprintf("Reading feature 0x%02x",feature_code);
       //  _baseModel->setStatusMsg(msg);
       _baseModel->setStatusMsg(msg.sprintf("Reading feature 0x%02x",feature_code));

        ddcrc = ddca_get_non_table_vcp_value(dh, feature_code, &valrec);
        printf("(%s::%s) feature_code=0x%02x, ddca_get_non_table_vcp_value() returned %d - %s\n",
               _cls, __func__, feature_code, ddcrc, ddca_rc_name(ddcrc));  fflush(stdout);
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
    }

    if (ddcrc == 0) {
       // should this be here?
       ddcrc = ddca_get_feature_metadata_by_dh(feature_code, dh, false, &finfo);
       if (ddcrc != 0) {
           rpt_ddca_status(feature_code, __func__, "ddca_get_feature_metadata",  ddcrc);
           cout << "ddca_get_feature_metadata() returned " << ddcrc << endl;
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

    // printf("(VcpThread::getvcp) Done\n");
}


// Process RQSetVcp
void VcpThread::setvcp(uint8_t feature_code, uint8_t sl) {
    // printf("(VcpThread::setvcp) Starting. feature_code=0x%02x\n", feature_code);
    DDCA_Display_Handle         dh;
    //  DDCA_Non_Table_Vcp_Value    valrec; // unused
    // valrec.sh = 0;     // only setting low byte
    // valrec.sl = sl;
    // valrec.mh = 0;  //hack
    // valrec.ml = 0;   // hack

    // DDCA_Feature_Metadata  finfo;    // unused

    DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
    if (ddcrc != 0) {
        cout << "ddca_open_display() returned " << ddcrc << endl;
        // how to handle?
    }

    ddca_enable_verify(true);
    // uint8_t verified_hi_byte = 0;   // unused
    // uint8_t verified_lo_byte = 0;   // unused

    // need to update mh, ml, use a valrec
    // ddcrc = ddca_set_non_table_vcp_value_verify(dh, feature_code, 0, sl, &verified_hi_byte, &verified_lo_byte);
    ddcrc = ddca_set_non_table_vcp_value(dh, feature_code, 0, sl);
    if (ddcrc != 0) {
        printf("(VcpThread::%s) ddca_set_raw_vcp_value() returned %d - %s\n",
               __func__, ddcrc, ddca_rc_name(ddcrc)); fflush(stdout);
        // cout << "ddca_set_raw_vcp_value() returned " << ddcrc << endl;
        // how to handle?
        rpt_ddca_status(feature_code, __func__, "ddca_set_non_table_vcp_value", ddcrc);

        if ( strcmp(ddca_rc_name(ddcrc), "DDCRC_VERIFY") == 0)  {
            printf("(VcpThread::%s) Verification failed\n", __func__);
            // read current value
            // put up dialog box with:
            //   original valueS
            //   requested value
            //   current value

            // Cannot create children for a parent in a different thread
            // QMessageBox::critical(nullptr,
            //                                    QString("Error"),      // title
            //                                    QString("Verification failed"));

            // undefined reference to VcpThread::postError(Qstring)
            // emit postError(QString("Verification failed"));

// #ifdef MAY_NOT_BE_NEEDED
            DDCA_Non_Table_Vcp_Value              valrec;
                DDCA_Status
                ddcrc2 = ddca_get_non_table_vcp_value(dh, feature_code, &valrec);
                if (ddcrc2 != 0) {
                    rpt_ddca_status(feature_code, __func__, "ddca_get_nontable_vcp_value", ddcrc);
                    // how to handle?
                }
                else {
                    // printf("ddca_get_nontable_vcp_value() returned:\n");
                    // printf("  opcode:   0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
                    //        feature_code, valrec.mh, valrec.ml, valrec.sh, valrec.sl);
                   _baseModel->modelVcpValueUpdate(feature_code, valrec.sh, valrec.sl);
                }
// #endif

       }
    }

#ifdef OLD
    // should not be necessary for setvcp
    if (ddcrc == 0) {

       ddcrc = ddca_get_simplified_feature_info(feature_code, _dinfo->vcp_version,&finfo);
       if (ddcrc != 0) {
           cout << "ddca_get_simplified_feature_info() returned " << ddcrc << endl;
           // how to handle?
           printf("ddca_get_simplfied_feature_info() returned %d - %s - %s\n",
                  ddcrc, ddca_rc_name(ddcrc), ddca_rc_desc(ddcrc));
       }
    }
#endif

    if (ddcrc == 0) {
        // _baseModel->modelVcpValueSet(feature_code, _dinfo->vcp_version, finfo, &valrec);
        _baseModel->modelVcpValueUpdate(feature_code, 0, sl);
    }
    else if (ddcrc == DDCRC_VERIFY) {
      //   _baseModel->modelVcpValueUpdate(feature_code, verified_hi_byte, verified_lo_byte);
    }


    ddcrc = ddca_close_display(dh);
    if (ddcrc != 0) {
        cout << "ddca_close_display() returned " << ddcrc << endl;
        // how to handle?
    }

    // printf("(VcpThread::getvcp) Done\n");
}


// Process RQStartInitailLoad
void VcpThread::startInitialLoad(void) {
    printf("(VcpThread::StartInitialLoad)\n");  fflush(stdout);
    // _baseModel->beginResetModel();
    _baseModel->modelStartInitialLoad();
    _baseModel->modelMccsVersionSet(_dinfo->vcp_version);

}


// Process RQEndInitialLoad
void VcpThread::endInitialLoad(void) {
        printf("(VcpThread::EndInitialLoad)\n");  fflush(stdout);
        // _baseModel->report();
        // _baseModel->endResetModel();
        _baseModel->modelEndInitialLoad();
       // emit signalStatusMsg(QString("Loading complete"));
        _baseModel->setStatusMsg(QString("Loading complete"));
}



void VcpThread::run() {

    forever {
        VcpRequest * rqst = this->_requestQueue->pop();

        // temporarily, just write out request

#ifdef OLD
        VcpGetRequest* getRqst = static_cast<VcpGetRequest*>(rqst);
        VcpSetRequest* setRqst = static_cast<VcpSetRequest*>(rqst);

        if (getRqst) {
            // cout << "VcpGetRequest.  feature_code = " << getRqst->feature_code << endl;
            printf("(VcpThread::run) VcpGetRequest. feature_code=0x%02x\n", getRqst->feature_code);
            getvcp(getRqst->feature_code);
        }
        else if (setRqst) {
            cout << "VcpSetRequest.  feature_code = " << setRqst->feature_code<<
                    "newval = " << setRqst->newval << endl;
        }
        else {
            cout << "Unexpected request type" << endl;
        }
#endif
        VcpRequestType rqstType = rqst->_type;
        switch(rqstType) {
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
            if (debugThread)
                printf("(VcpThread::run) RQSetVcp. feature code=0x%02x, newval=%d\n",
                       setRqst->_featureCode, setRqst->_newval);  fflush(stdout);
            setvcp(setRqst->_featureCode, setRqst->_newval & 0xff);
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
            cout << "Unexpected request type: " << rqstType << endl;

        }
    }
}
