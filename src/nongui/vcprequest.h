/* vcprequest.h - VcpRequestQueue and the VcpRequest classes that populate the queue */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

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

    DDCA_Vcp_Feature_Code _featureCode;
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

    DDCA_Vcp_Feature_Code _featureCode;
    uint8_t               _newval;
    bool                  _writeOnly;
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
