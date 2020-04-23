/** \file msgbox_thread.cpp
 *
 *  This thread reads from a queue of error messages to be displayed in a
 *  dialog box. This avoids a flurry of simultaneous dialog boxes.
 */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <iostream>
#include <string.h>

#include <QtCore/QString>
#include <QtCore/QThread>

#include "base/core.h"
#include "main/msgbox_thread.h"

static bool debugThread = false;

// dangerous, would clobber if multiple instances of this class,
// but can't get it to compile as a member variable;
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
    TRACECF(debugThread, "Sleeping for %d milliseconds", initial_sleep_millis);
     QThread::msleep(initial_sleep_millis);
     TRACECF(debugThread, "Initial sleep complete");

    forever {
        TRACECF(debugThread, "Waiting to pop"); fflush(stdout);
        MsgBoxQueueEntry * rqst = this->_requestQueue->pop();
        TRACECF(debugThread, "Popped: _boxTitle: %s, _boxText: %s",
                                QS2S(rqst->_boxTitle), QS2S(rqst->_boxText));

        msgboxSemaphore.acquire();
        TRACECF(debugThread, "Acquired msgboxSemaphore");
        emit postSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
        // requires MainWindow; clearer since MainWindow::showSerialMsgBox is what gets called
        // but would require knowing MainWindow
        // showSerialMsgBox(rqst->_boxTitle, rqst->_boxText, rqst->_boxIcon);
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


