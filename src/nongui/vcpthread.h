/** \file vcpthread.h
 *
 * Thread for performing ddca_ API calls, which can perform I2C IO and so can be
 * slow.  There is one instance of this class, i.e. one thread, for each monitor.
 */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VCPTHREAD_H
#define VCPTHREAD_H

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "ddcutil_c_api.h"

#include "nongui/ddc_error.h"
#include "nongui/feature_base_model.h"
#include "nongui/vcprequest.h"

class QString;

class VcpThread : public QThread
{

public:
    VcpThread(
        QObject*            parent,
        DDCA_Display_Info  *dinfo,
        VcpRequestQueue    *requestQueue,
        FeatureBaseModel   *baseModel);

    void run() override;

    // Perhaps an init function not in the constructor that opens the display?

signals:
    void postDdcFeatureError(DdcFeatureError& erec);

private:  // member functions
    void getvcp(uint8_t feature_code, bool needMetadata);
    void setvcp(uint8_t feature_code, bool writeOnly, uint16_t newval);
    void adjustRetries();
    DDCA_Status perform_open_display(DDCA_Display_Handle * dh_loc);
    DDCA_Status perform_close_display(DDCA_Display_Handle dh);
    void capabilities();
    void loadDynamicFeatureRecords();
    void startInitialLoad(void);
    void endInitialLoad(void); 
    void rpt_ddca_status(
          uint8_t feature_code,
          const char * caller_name,
          const char * ddca_func_name,
          int ddcrc);
    void rpt_error_detail(
          DDCA_Error_Detail * erec,
          const char * caller_name,
          const char * ddcra_func_name);
    void rpt_verify_error(
          uint8_t      featureCode,
          const char * function,
          uint16_t      expectedValue,
          uint16_t      observedValue);
    void rpt_verify_error(
          uint8_t      featureCode,
                const char * function,
                uint8_t      expectedSh,
                uint8_t      expectedSl,
                uint8_t      observedSh,
                uint8_t      observedSl
                );

#ifdef FUTURE
    void rpt_ddca_error(
          const char * caller_name,
          const char * ddca_func_name,
          DDCA_Status  ddcrc,
          DDCA_Error_Detail * erec,
          int          feature_code           // -1 if no feature code
          );
#endif

private:       // member variables
    const char *         _cls = "VcpThread";  //= strdup(metaObject()->className());
    DDCA_Display_Ref     _dref;
    DDCA_Display_Info*   _dinfo;
    VcpRequestQueue*     _requestQueue = NULL;
    FeatureBaseModel*    _baseModel;
    DDCA_Display_Handle  _dh = NULL;
    bool                 _threadDescriptionPublished = false;
};

#endif // VCPTHREAD_H
