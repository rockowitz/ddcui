/* msgbox_queue.cpp - MsgBoxQueue and the MsgBoxQueueEntry class that populates it */

// Copyright (C) 2018-2025 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

#include <assert.h>
#include <iostream>

#include "base/ddcui_core.h"

#include "nongui/msgbox_queue.h"

static bool debugClass = false;

/* MsgBoxQueueEntry and subclasses */

MsgBoxQueueEntry::MsgBoxQueueEntry(
      QString            title,
      QString            text,
      QMessageBox::Icon  icon)
   : _boxTitle(title)
   , _boxText(text)
   , _boxIcon(icon)
{
   TRACECF(debugClass, "Constructor. title=%s, text=%s, icon=%d",
                       QS2S(_boxTitle), QS2S(_boxText), _boxIcon);
}


MsgBoxQueueEntry::~MsgBoxQueueEntry()
{
   TRACECF(debugClass, "Executing");
}


QString MsgBoxQueueEntry::repr() {
   // qDebug() << "_boxTitle: " << _boxTitle;
   // qDebug() << "_boxText: "  << _boxText;
   // qDebug() << "_boxIcon: "  << _boxIcon;

   QString msg = QString("[title=0x%1, text=%2, icon=%3]")
                    .arg(_boxTitle)
                    .arg(_boxText)
                    .arg(_boxIcon);
   // qDebug() << "Assembled: " << msg;
   // printf("(MsgBoxQueueEntry::repr) returning: %s\n", QS2S(msg)); fflush(stdout);
   return msg;
}


/* MsgBoxQueue */

MsgBoxQueue::MsgBoxQueue()
{
   bool debugFunc = debugClass;
   debugFunc = false;
   _queue = QQueue<MsgBoxQueueEntry*>();
#ifdef USE_MUTEX
    // _mutex = QMutex(QMutex::Recursive);
#else
    _freeBytes = new QSemaphore(9999);
    _usedBytes = new QSemaphore();
    // -Wnon-pod-varrags says will abort at runtime:
    // TRACECF(debugFunc, "this=%p, queue=%p, _usedBytes=%p, _freeBytes=%p", this, _queue, _usedBytes, _freeBytes);
#endif
}


void MsgBoxQueue::put(MsgBoxQueueEntry * request) {
    bool debug = false || debugClass;
    TRACECF(debug, "-> Starting. request: |%s|", QS2S(request->repr()));
    assert(request);

#ifdef USE_MUTEX
    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();
    _mutex.unlock();
#else
    _freeBytes->acquire();
    _queue.enqueue(request);
    _usedBytes->release();
#endif
    // dbgrpt_nolock();
    TRACECF(debug, "Done");
}


void MsgBoxQueue::putMessages(QString qstitle, QMessageBox::Icon icon, char** msgs) {
   bool debug = false || debugClass;
   TRACECF(debug, "Starting");
   if (msgs) {
      if (debug) {
         int ct = 0;
         while(msgs[ct]) ct++;
         TRACEC("%d error messages", ct);
      }
      for (int ndx = 0; msgs[ndx]; ndx++) {
         QString qsexpl = QString::asprintf("%s", msgs[ndx]);
         MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qstitle,qsexpl,icon);
         TRACECF(debug, "Calling _msgboxQueue.put() for qe: %s", __func__, QS2S(qe->repr()) );
         put(qe);
      }
   }
   TRACECF(debug, "Done");
}


MsgBoxQueueEntry * MsgBoxQueue::pop() {
    bool debug = false || debugClass;
    TRACECF(debug, "Starting");
#ifdef USE_MUTEX
    _mutex.lock();
    TRACECF(debug, "After lock, before wait");
    if (_queue.empty())
        _queueNonempty.wait(&_mutex);
    MsgBoxQueueEntry * rqst = _queue.dequeue();
    // this->dbgrpt_nolock();
    _mutex.unlock();
#else
    _usedBytes->acquire();
    MsgBoxQueueEntry * rqst = _queue.dequeue();
    _freeBytes->release();

    // TRACECF(debug, "-> After releasing  _freeBytes. available=%d, request: |%s|",
    //       _freeBytes->available(), QS2S(rqst->repr()));
#endif
    TRACECF(debug, "<- Done. Returning request: %s", QS2S(rqst->repr()) );
    return rqst;
}


void MsgBoxQueue::dbgrpt_nolock() {
   int ct = _queue.size();
   TRACEC("Queue contains %d entries", ct);
   for (int ndx = 0; ndx < ct; ndx++) {
      MsgBoxQueueEntry * rqst = _queue.at(ndx);
      TRACEC("   %s", QS2S(rqst->repr()) );
   }
}


void MsgBoxQueue::dbgrpt() {
   _mutex.lock();
   dbgrpt_nolock();
   _mutex.unlock();
}
