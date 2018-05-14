/* vcprequest.cpp - VcpRequestQueue and the classes that populate it */

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

#include "nongui/vcprequest.h"

/* VcpRequest and subclasses */

VcpRequest::VcpRequest(VcpRequestType type) {
     _type = type;
}

VcpRequest::~VcpRequest() {
}


VcpCapRequest::VcpCapRequest()
    : VcpRequest::VcpRequest(VcpRequestType::RQCapabilities)
{
}



VcpGetRequest::VcpGetRequest(DDCA_Vcp_Feature_Code feature_code)
    : VcpRequest::VcpRequest(VcpRequestType::RQGetVcp)
{
    _featureCode = feature_code;
}


VcpSetRequest::VcpSetRequest(DDCA_Vcp_Feature_Code feature_code, uint8_t newval, bool writeOnly)
    :  VcpRequest::VcpRequest(VcpRequestType::RQSetVcp)
{
    _featureCode = feature_code;
    _newval = newval;
    _writeOnly = writeOnly;
}


VcpStartInitialLoadRequest::VcpStartInitialLoadRequest(void)
    : VcpRequest::VcpRequest(VcpRequestType::RQStartInitialLoad)
{
}


VcpEndInitialLoadRequest::VcpEndInitialLoadRequest(void)
    : VcpRequest::VcpRequest(VcpRequestType::RQEndInitialLoad)
{
}


/* VcpRequestQueue */

VcpRequestQueue::VcpRequestQueue()
{
    _queue = QQueue<VcpRequest*>();
}

void VcpRequestQueue::put(VcpRequest * request) {
    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();
    _mutex.unlock();
}

VcpRequest * VcpRequestQueue::pop() {
    _mutex.lock();
    if (_queue.empty())
        _queueNonempty.wait(&_mutex);

    VcpRequest * rqst = _queue.dequeue();
    // to do: scan queue for superceding requests
    // if (this->m_queue.empty())
    // do something with queueNonempty?

    _mutex.unlock();
    return rqst;
}

// VcpRequest VcpRequestQueue::peek() { }

