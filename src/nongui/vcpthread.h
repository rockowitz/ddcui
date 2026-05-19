/** vcpthread.h
 *
 * Thread for executing ddca_ API calls, which can perform I2C IO and so can be
 * slow.  There is one instance of this class, i.e. one thread, for each monitor.
 */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VCPTHREAD_H
#define VCPTHREAD_H

#include <QObject>
#include <QThread>

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
        DDCA_Display_Info2 *dinfo,
        VcpRequestQueue    *requestQueue,
        FeatureBaseModel   *baseModel,
        bool                estimate_x10 = false,
        bool                noverify_x10 = false);

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
#ifdef TEST_ADJUST_RETRIES
    void adjustRetries();
#endif
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
    DDCA_Status getMetadata(
          DDCA_Display_Handle     dh,
          uint8_t                 feature_code,
          DDCA_Feature_Metadata** finfo_loc);

private:       // member variables
    const char *         _cls = "VcpThread";  //= strdup(metaObject()->className());
    DDCA_Display_Ref     _dref;
    DDCA_Display_Info2*  _dinfo;
    VcpRequestQueue*     _requestQueue = NULL;
    FeatureBaseModel*    _baseModel;
//  DDCA_Display_Handle  _dh = NULL;     // unused
    DdcaSimulator *      _ddcaSimulator = NULL;
    bool                 _estimate_x10 = false;
    bool                 _noverify_x10 = false;
};

void init_vcpthread();

#endif // VCPTHREAD_H
