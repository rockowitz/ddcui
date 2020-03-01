/* vcprequest.h - VcpRequestQueue and the VcpRequest classes that populate the queue */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VCPREQUEST_H
#define VCPREQUEST_H

#include <stdint.h>

#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QWaitCondition>

#include <ddcutil_c_api.h>

enum VcpRequestType {
   RQGetVcp,
   RQSetVcp,
   RQStartInitialLoad,
   RQEndInitialLoad,
   RQCapabilities,
   RQLoadDfr,
#ifdef NOT_NEEDED
   RQDumpStats,
   RQResetStats
#endif
};

/* VcpRequest classes */

class VcpRequest
{
public:
    VcpRequest(VcpRequestType type);
    ~VcpRequest();

    VcpRequestType _type;
};

#ifdef NOT_NEEDED
class VcpDumpStatsRequest: public VcpRequest {
public:
   VcpDumpStatsRequest(DDCA_Stats_Type stats_type);
   ~VcpDumpStatsRequest();

   DDCA_Stats_Type _stats_type;
};


class VcpResetStatsRequest: public VcpRequest {
public:
   VcpResetStatsRequest();
   ~VcpResetStatsRequest();
};
#endif



class VcpCapRequest: public VcpRequest {
public:
    VcpCapRequest();
    ~VcpCapRequest();
};

class LoadDfrRequest: public VcpRequest {
public:
    LoadDfrRequest();
    ~LoadDfrRequest();
};


class VcpGetRequest: public VcpRequest {
public:
    VcpGetRequest(uint8_t featureCode, bool needMetadata);
    ~VcpGetRequest();

    DDCA_Vcp_Feature_Code _featureCode;
    bool                  _needMetadata;
};


class VcpSetRequest: public VcpRequest {
public:
    VcpSetRequest(uint8_t featureCode, uint8_t newSh, uint8_t newSl, bool writeOnly=false);
    ~VcpSetRequest();

    DDCA_Vcp_Feature_Code _featureCode = 0;
    uint8_t               _newSh = 0;
    uint8_t               _newSl = 0;
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
    void dbgrpt();



// public slots:
    void        put(VcpRequest* request);

private:
    // Monitor* monitor;
    QMutex _mutex;

    QQueue<VcpRequest*> _queue;
    QWaitCondition     _queueNonempty;
};

#endif // VCPREQUEST_H

