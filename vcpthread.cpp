#include "vcpthread.h"
#include <iostream>
#include <string.h>
#include <QMessageBox>



using namespace std;

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
}

void VcpThread::getvcp(uint8_t feature_code) {
    // printf("(VcpThread::getvcp) Starting. feature_code=0x%02x\n", feature_code);
    DDCA_Display_Handle         dh;
    DDCA_Non_Table_Value        valrec;
    // char *                      formatted_value = NULL;
    // DDCA_Version_Feature_Info*  feature_info = NULL;
    DDCA_Simplified_Version_Feature_Info  finfo;
    // char *                      feature_name = NULL;

    DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
    if (ddcrc != 0) {
        cout << "ddca_open_display() returned " << ddcrc << endl;
        // how to handle?
    }

#ifdef FUTURE
        DDCA_Any_Vcp_Value * valrec;
        ddcrc = ddca_get_any_vcp_value(dh, feature_code, DDCA_UNSET_VCP_VALUE_TYPE_PARM, &valrec);
        if (ddcrc != 0) {
            cout << "ddca_get_any_vcp_value() returned " << ddcrc << endl;
            // how to handle?
        }
        else {
            printf("ddca_get_any_vcp_value() returned:\n");
            printf("  opcode:   0x%92x\n", valrec->opcode);
            printf("  value_type:  %d\n", valrec->value_type);
            if (valrec->value_type )

        }
#endif
    if (ddcrc == 0) {
        // TODO: get raw value
        ddcrc = ddca_get_nontable_vcp_value(dh, feature_code, &valrec);
        if (ddcrc != 0) {
            printf("(%s) ddca_get_nontable_vcp_value() returned %d - %s\n",
                   __func__, ddcrc, ddca_rc_name(ddcrc)); fflush(stdout);
            cout << "ddca_get_nontable_vcp_value() returned " << ddcrc << endl;
            // how to handle?
        }
        else {
            // printf("ddca_get_nontable_vcp_value() returned:\n");
            // printf("  opcode:   0x%02x, mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x\n",
            //        feature_code, valrec.mh, valrec.ml, valrec.sh, valrec.sl);
        }

    }

#ifdef NOT_HERE
    if (ddcrc == 0) {
        ddcrc = ddca_get_formatted_vcp_value(dh, feature_code, &formatted_value);
        if (ddcrc != 0) {
            cout << "ddca_get_formatted_vcp_value() returned " << ddcrc << endl;
            // how to handle?
            printf("ddca_get_formatted_vcp_value() returned %d - %s - %s\n",
                   ddcrc, ddca_rc_name(ddcrc), ddca_rc_desc(ddcrc));
        }
        else {
                printf("(VcpThread::getvcp) formatted_value = |%s|\n", formatted_value);
                // cout << formatted_value << endl;
        }
    }
#endif

    if (ddcrc == 0) {

       ddcrc = ddca_get_simplified_feature_info(feature_code, _dinfo->vcp_version,&finfo);
       if (ddcrc != 0) {
           cout << "ddca_get_simplified_feature_info() returned " << ddcrc << endl;
           // how to handle?
           printf("ddca_get_simplfied_feature_info() returned %d - %s - %s\n",
                  ddcrc, ddca_rc_name(ddcrc), ddca_rc_desc(ddcrc));
       }
    }

#ifdef ELSEWHERE
    if (ddcrc == 0) {
        feature_name = ddca_get_feature_name(feature_code);
        if (!feature_name) {
            printf("ddca_get_feature_name(0x%02x) failed.", feature_code);
            feature_name = (char *) "<missing>";
        }
    }
#endif

#ifdef OLD
    if (ddcrc == 0) {
       QString fv = QString(formatted_value);
       QString qname = QString(feature_name);
       // cout << "fv: " << fv << std::endl;
       _baseModel->modelVcpValueSet(feature_code, vspec, feature_info, qname, &valrec, fv);

       free(formatted_value);
       // do not free feature_name, it is part of ddcutil internal data structure
    }
#endif

    if (ddcrc == 0) {
        _baseModel->modelVcpValueSet(feature_code, _dinfo->vcp_version, finfo, &valrec);
    }

    ddcrc = ddca_close_display(dh);
    if (ddcrc != 0) {
        cout << "ddca_close_display() returned " << ddcrc << endl;
        // how to handle?
    }


    // printf("(VcpThread::getvcp) Done\n");
}



void VcpThread::setvcp(uint8_t feature_code, uint8_t sl) {
    // printf("(VcpThread::setvcp) Starting. feature_code=0x%02x\n", feature_code);
    DDCA_Display_Handle         dh;
    DDCA_Non_Table_Value        valrec;
    valrec.sh = 0;     // only setting low byte
    valrec.sl = sl;
    valrec.mh = 0;  //hack
    valrec.ml = 0;   // hack
        DDCA_Simplified_Version_Feature_Info  finfo;

    DDCA_Status ddcrc = ddca_open_display(this->_dref, &dh);
    if (ddcrc != 0) {
        cout << "ddca_open_display() returned " << ddcrc << endl;
        // how to handle?
    }

    ddca_enable_verify(true);

    // need to update mh, ml, use a valrec
    ddcrc = ddca_set_raw_vcp_value(dh, feature_code, 0, sl);
    if (ddcrc != 0) {
        printf("(VcpThread::%s) ddca_set_raw_vcp_value() returned %d - %s\n",
               __func__, ddcrc, ddca_rc_name(ddcrc)); fflush(stdout);
        // cout << "ddca_set_raw_vcp_value() returned " << ddcrc << endl;
        // how to handle?

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
            emit postError(QString("Verification failed"));
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


    ddcrc = ddca_close_display(dh);
    if (ddcrc != 0) {
        cout << "ddca_close_display() returned " << ddcrc << endl;
        // how to handle?
    }

    // printf("(VcpThread::getvcp) Done\n");
}


void VcpThread::startInitialLoad(void) {
    printf("(VcpThread::StartInitialLoad)\n");
    // _baseModel->beginResetModel();
    _baseModel->modelStartInitialLoad();
    _baseModel->modelMccsVersionSet(_dinfo->vcp_version);

}

void VcpThread::endInitialLoad(void) {
        printf("(VcpThread::EndInitialLoad)\n");
        _baseModel->report();
        // _baseModel->endResetModel();
        _baseModel->modelEndInitialLoad();
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
            printf("(VcpThread::run) VcpGetRequest. feature_code=0x%02x\n", getRqst->_featureCode);
            getvcp(getRqst->_featureCode);
            break;
        }
        case VcpRequestType::RQSetVcp:
        {
            VcpSetRequest* setRqst = static_cast<VcpSetRequest*>(rqst);
            // garbage output
            // cout << "VcpSetRequest.  feature_code = " << setRqst->_featureCode<<
            //         "newval = " << setRqst->_newval << endl;
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
        default:
            cout << "Unexpected request type: " << rqstType << endl;

        }
    }
}
