/* VcpRequestQueue and the VcpRequest classes that populate the queue */

#ifndef VCPREQUEST_H
#define VCPREQUEST_H

#include <stdint.h>

// #include <QObject>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

#include <ddcutil_c_api.h>


enum VcpRequestType {RQGetVcp, RQSetVcp, RQStartInitialLoad, RQEndInitialLoad};

/* VcpRequest classes */

class VcpRequest
{
public:
    VcpRequest(VcpRequestType type);
    ~VcpRequest();

    VcpRequestType _type;
};


class VcpGetRequest: public VcpRequest {
public:
    VcpGetRequest(uint8_t featureCode);
    ~VcpGetRequest();

    DDCA_Vcp_Feature_Code _featureCode;
};


class VcpSetRequest: public VcpRequest {
public:
    VcpSetRequest(uint8_t featureCode, uint8_t newval);
    ~VcpSetRequest();

    DDCA_Vcp_Feature_Code _featureCode;
    uint8_t _newval;
};


class VcpStartInitialLoadRequest : public VcpRequest {
public:
    VcpStartInitialLoadRequest();
    ~VcpStartInitialLoadRequest();
};


class VcpEndInitialLoadRequest : public VcpRequest {
public:
    VcpEndInitialLoadRequest(void);
};


/* VcpRequestQueue */

class VcpRequestQueue
{
    // Q_OBJECT

public:
    VcpRequestQueue();


    VcpRequest* pop();
//  VcpRequest* popCondensed();   // future

    QMutex _mutex;

// public slots:
        void        put(VcpRequest* request);

private:
    // Monitor* monitor;

    QQueue<VcpRequest*> _queue;
    QWaitCondition     _queueNonempty;
};

#endif // VCPREQUEST_H
