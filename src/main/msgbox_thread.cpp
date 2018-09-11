/* msgbox_thread.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <iostream>
#include <string.h>

#include <QtCore/QString>

#include "base/ddcui_globals.h"
#include "main/msgbox_thread.h"

using namespace std;

static bool debugThread = true;

static QSemaphore msgboxSemaphore(1);


MsgBoxThread::MsgBoxThread(
        MsgBoxQueue*    requestQueue)
    : QThread()
    , _requestQueue(requestQueue)
{}


void MsgBoxThread::msbgoxClosed(int result) {
   PRINTFTCMF(debugThread, "Releasing semaphore");
   msgboxSemaphore.release();
}


void MsgBoxThread::run() {
    forever {
        MsgBoxQueueEntry * rqst = this->_requestQueue->pop();
        PRINTFTCMF(debugThread, "Popped: _boxTitle: %s, _boxText: %s",  qs2s(rqst->_boxTitle), qs2s(rqst->_boxText));
#ifdef OLD
        QMessageBox * msgBox = new QMessageBox(_parent);
        msgBox->setText(rqst->_boxText);
        msgBox->setWindowTitle(rqst->_boxTitle);
        msgBox->setIcon(rqst->_boxIcon);
        msgBox->setModal(true);
        PRINTFTCM("Before msgBox->exec()");
           // std::cout << "Thread id: " << QThread::currentThreadId() << std::endl;
#endif
        msgboxSemaphore.acquire();
        PRINTFTCMF(debugThread, "acquired semaphore");
        // cause of "cannot create children for a parent that is in a different thread" ?  Yes
        // msgBox->open();
        emit postSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
#ifdef OLD
        msgBox->exec();
        PRINTFTCM("before release semaphore");
        msgboxSemaphore.release();
        PRINTFTCM("After msgBox->exec()");
           // std::cout << "Thread id: " << QThread::currentThreadId() << std::endl;
#endif
        delete rqst;
    }
}

