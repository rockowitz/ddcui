/* msgbox_thread.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <iostream>
#include <string.h>

#include <QtCore/QString>

#include "base/core.h"
#include "main/msgbox_thread.h"

static bool debugThread = false;

static QSemaphore msgboxSemaphore(1);


MsgBoxThread::MsgBoxThread(
        MsgBoxQueue*    requestQueue)
    : QThread()
    , _requestQueue(requestQueue)
{}


void MsgBoxThread::msbgoxClosed(int result) {
   TRACECF(debugThread, "Releasing semaphore");
   msgboxSemaphore.release();
}


void MsgBoxThread::run() {
    forever {
        TRACECF(debugThread, "Waiting to pop"); fflush(stdout);
        MsgBoxQueueEntry * rqst = this->_requestQueue->pop();

        //TRACECF(debugThread, "post pop()");
        //   TRACECF(debugThread, "rqst=%p", rqst);
        // TRACECF(debugThread, "Popped: _boxTitle: %s, _boxText: %s",
        //                         qs2s(rqst->_boxTitle), qs2s(rqst->_boxText));

        TRACECF(debugThread, "Popped");
        msgboxSemaphore.acquire();
        TRACECF(debugThread, "Acquired msgboxSemaphore");
        emit postSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
        delete rqst;
    }
}
