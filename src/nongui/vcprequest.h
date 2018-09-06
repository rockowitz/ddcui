/* vcprequest.h - VcpRequestQueue and the VcpRequest classes that populate the queue */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VCPREQUEST_H
#define VCPREQUEST_H

#include <stdint.h>

#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QWaitCondition>

#include <ddcutil_c_api.h>


enum VcpRequestType {RQGetVcp, RQSetVcp, RQStartInitialLoad, RQEndInitialLoad, RQCapabilities};

/* VcpRequest classes */

class VcpRequest
{
public:
    VcpRequest(VcpRequestType type);
    ~VcpRequest();

    VcpRequestType _type;
};


class VcpCapRequest: public VcpRequest {
public:
    VcpCapRequest();
    ~VcpCapRequest();

    // DDCA_Vcp_Feature_Code _featureCode;
};


class VcpGetRequest: public VcpRequest {
public:
    VcpGetRequest(uint8_t featureCode);
    ~VcpGetRequest();

    DDCA_Vcp_Feature_Code _featureCode;
};


class VcpSetRequest: public VcpRequest {
public:
    VcpSetRequest(uint8_t featureCode, uint8_t newval, bool writeOnly=false);
    ~VcpSetRequest();

    DDCA_Vcp_Feature_Code _featureCode = 0;
    uint8_t               _newval = 0;
    bool                  _writeOnly = false;
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

