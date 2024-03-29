/** vcpthread.h
 *
 * Thread for executing ddca_ API calls, which can perform I2C IO and so can be
 * slow.  There is one instance of this class, i.e. one thread, for each monitor.
 */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VCPTHREAD_H
#define VCPTHREAD_H

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "ddcutil_c_api.h"

#include "nongui/ddca_simulator.h"
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

    ~VcpThread();

    void run() override;

    // Perhaps an init function not in the constructor that opens the display?

    enum FeatureOp {
       FeatureRead,
       FeatureWrite,
       FeatureMetadata
    };

private:  // member functions
    void getvcp(uint8_t feature_code, bool needMetadata);
    void setvcp(uint8_t feature_code, bool writeOnly, uint16_t newval);
    void adjustRetries();
    DDCA_Status
         perform_open_display(DDCA_Display_Handle * dh_loc);
    DDCA_Status
         perform_close_display(DDCA_Display_Handle dh);
    void capabilities();
    void loadDynamicFeatureRecords();
    void startInitialLoad(void);
    void endInitialLoad(void);
    void rpt_feature_error(
          enum FeatureOp      op,
          uint8_t             featureCode,
          const char *        ddcaFuncName,
          DDCA_Status         ddcrc);
    void rpt_nonfeature_error(
          const char *        action,
          const char *        ddcaFuncName,
          DDCA_Status         ddcrc);
    void rpt_nonfeature_error(
          const char *        action,
          const char *        ddcaFuncName,
          DDCA_Status         ddcrc,
          DDCA_Error_Detail * erec);
    void rpt_verify_error(
          uint8_t             featureCode,
          const char *        function,
          uint16_t            expectedValue,
          uint16_t            observedValue);
    void rpt_verify_error(
          uint8_t             featureCode,
          const char *        function,
          uint8_t             expectedSh,
          uint8_t             expectedSl,
          uint8_t             observedSh,
          uint8_t             observedSl);

private:       // member variables
    const char *         _cls = "VcpThread";  //= strdup(metaObject()->className());
    DDCA_Display_Ref     _dref;
    DDCA_Display_Info*   _dinfo;
    VcpRequestQueue*     _requestQueue = NULL;
    FeatureBaseModel*    _baseModel;
    DDCA_Display_Handle  _dh = NULL;
    DdcaSimulator *      _ddcaSimulator = NULL;
};

#endif // VCPTHREAD_H
