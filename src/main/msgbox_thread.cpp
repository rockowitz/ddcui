/* msgbox_thread.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <iostream>
#include <string.h>

#include <QtCore/QString>

#include "main/msgbox_thread.h"
#include "../base/core.h"


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
        MsgBoxQueueEntry * rqst = this->_requestQueue->pop();
        TRACECF(debugThread, "Popped: _boxTitle: %s, _boxText: %s",
                                qs2s(rqst->_boxTitle), qs2s(rqst->_boxText));
        msgboxSemaphore.acquire();
        TRACECF(debugThread, "acquired semaphore");
        emit postSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
        delete rqst;
    }
}
