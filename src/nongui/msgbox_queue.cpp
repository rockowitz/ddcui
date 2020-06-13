/* msgbox_queue.cpp - MsgBoxQueue and the MsgBoxQueueEntry class that populates it */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

#include <assert.h>
#include <iostream>

#include "base/core.h"

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
     // _type = type;
   // printf("(MsgBoxQueueEntry::MsgBoxQueueEntry)\n");  fflush(stdout);
   // printf("(MsgBoxQueueEntry::MsgBoxQueueEntry) debug = %s\n", sbool(debugClass));  fflush(stdout);
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
    _queue = QQueue<MsgBoxQueueEntry*>();
    // _mutex = QMutex(QMutex::Recursive);
}


void MsgBoxQueue::put(MsgBoxQueueEntry * request) {
    bool debugFunc = debugClass;
    // debugFunc = true;

    assert(request);
    TRACECF(debugFunc, "-> Before lock. request: %s", QS2S(request->repr()));

    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();

    // dbgrpt_nolock();

    _mutex.unlock();
    TRACECF(debugClass, "Done");
}


MsgBoxQueueEntry * MsgBoxQueue::pop() {
    bool debugFunc = debugClass;
    // debugFunc = true;
    TRACECF(debugFunc, "Starting. Before wait");
    _mutex.lock();
    if (_queue.empty())
        _queueNonempty.wait(&_mutex);

    MsgBoxQueueEntry * rqst = _queue.dequeue();

    // this->dbgrpt_nolock();

    _mutex.unlock();
#ifdef NO
    if (debugClass) {
        TRACEC("  rqst = %p", rqst);
        TRACEC("  title=%s", qs2s(rqst->_boxTitle));
        TRACEC("  text=%s", qs2s(rqst->_boxText));
        TRACEC("  icon=%d", rqst->_boxIcon);
        fflush(stdout);
     }
#endif
    TRACECF(debugFunc, "<- Done. Returning request: %s", QS2S(rqst->repr()) );
    return rqst;
}


void MsgBoxQueue::dbgrpt_nolock() {
   int ct = _queue.size();
   TRACECF(debugClass, "Queue contains %d entries", ct);
   for (int ndx = 0; ndx < ct; ndx++) {
      MsgBoxQueueEntry * rqst = _queue.at(ndx);
      TRACECF(debugClass, "   %s", QS2S(rqst->repr()) );
   }
}


void MsgBoxQueue::dbgrpt() {
   _mutex.lock();
   dbgrpt_nolock();
   _mutex.unlock();
}
