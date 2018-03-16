/* vcpthread.h */

#ifndef VCPTHREAD_H
#define VCPTHREAD_H

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "ddcutil_c_api.h"

#include "nongui/feature_base_model.h"
#include "nongui/vcprequest.h"


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

    const char * cls = "VcpThread";

private:
    void getvcp(uint8_t feature_code);
    void setvcp(uint8_t feature_code, uint8_t newval);
    void startInitialLoad(void);
    void endInitialLoad(void); 
    void rpt_ddca_status(const char * caller_name, const char *ddca_func_name, int ddcrc);

    DDCA_Display_Ref     _dref;
    DDCA_Display_Info*   _dinfo;
    VcpRequestQueue*     _requestQueue = NULL;
    FeatureBaseModel*    _baseModel;
    DDCA_Display_Handle  _dh = NULL;

signals:
    void postError(QString);
};

#endif // VCPTHREAD_H
