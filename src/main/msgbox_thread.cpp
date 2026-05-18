/** \file msgbox_thread.cpp
 *
 *  This thread reads from a queue of error messages to be displayed in a
 *  dialog box. This avoids a flurry of simultaneous dialog boxes.
 */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QString>
#include <QThread>
#include <QSemaphore>

#include "base/ddcui_core.h"
#include "c_util/debug_util.h"
#include "base/ddcui_rtti.h"

#include "main/msgbox_thread.h"

static bool debugThread = false;

MsgBoxThread::MsgBoxThread(MsgBoxQueue*    requestQueue)
    : QThread()
    , _requestQueue(requestQueue)
{
   // bool debug = false;
   TRACECF(debugThread, "Executing");
   _semaphore = new QSemaphore(1);
}

MsgBoxThread::~MsgBoxThread() {
   delete _semaphore;
}

void MsgBoxThread::msbgoxClosed(int result) {
   TRACECF_STARTING(debugThread, "Releasing semaphore");
   _semaphore->release();
}

void MsgBoxThread::run() {
#ifdef NO
    // If this thread starts reading before the QApplication's event loop,
    // the dialog box will not appear on top of the main window
    // Trying to delay by triggering off a signal that the main event loop
    // has started
    // DIDN'T SOLVE PROBLEM.
#endif
    // Crude but effective.  Just sleep before starting loop reading messages and
    // displaying SerialMsgBox
    long initial_sleep_millis =  MSGBOX_THREAD_RUN_DELAY_MILLIS;
    TRACECF_STARTING(debugThread, "Sleeping for %d milliseconds", initial_sleep_millis);
    QThread::msleep(initial_sleep_millis);
    TRACECF(debugThread, "Initial sleep complete");

    while (true) {    // eclipse doesn't recognize keyword forever
        TRACECF(debugThread, "Waiting to pop"); fflush(stdout);
        MsgBoxQueueEntry * rqst = this->_requestQueue->pop();
        TRACECF(debugThread, "Popped: _boxTitle: %s, _boxText: %s",
                                QS2S(rqst->_boxTitle), QS2S(rqst->_boxText));
        TRACECF(debugThread, "_lastText: %s", QS2S(_lastText));
        // Duplicate message can occur becuase UDEV doesn't report disconnection
        // events immediately, but instead just before a subsequent connection.
        // In the meantime, a DDCRC_DISCONNECTED status code on a feature request
        // causes a disconnected message to be posted
        if (rqst->_boxText != _lastText) {
           _semaphore->acquire();
           TRACECF(debugThread, "Acquired semaphore");
           emit postSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
           // requires MainWindow; clearer since MainWindow::showSerialMsgBox is what gets called
           // but would require knowing MainWindow
           // showSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
           _lastText = rqst->_boxText;
        }
        else {
           TRACECF(debugThread, "Skipping duplicate message");
        }
        delete rqst;
    }
}

#ifdef NO
void MsgBoxThread::showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon) {
// #ifdef DIALOG_BOX_STILL_ON_SEPARATE_SCREEN
   QMessageBox * serialMbox2 = new QMessageBox(parent);
   serialMbox2->setStandardButtons(QMessageBox::Ok);
   serialMbox2->setWindowModality(Qt::WindowModal);
   serialMbox2->setModal(true);
//    serialMbox2->setFont(_ui->mainMenuFont);
   serialMbox2->setText(text);
   serialMbox2->setWindowTitle(title);
   serialMbox2->setIcon(icon);

   QObject::connect(
         serialMbox2, &QMessageBox::finished,
         this, &MsgBoxThread::msbgoxClosed
         );

   serialMbox2->exec();

}
#endif


void init_msgbox_thread() {
   bool debug = false;
   DBGF(debug, "Starting");
   RTTI_ADD_METHOD(MsgBoxThread::MsgBoxThread);
   RTTI_ADD_METHOD(MsgBoxThread::msbgoxClosed);
   DBGF(debug, "Done");
}
