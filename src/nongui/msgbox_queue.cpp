/* msgbox_queue.cpp - MsgBoxQueue and the MsgBoxQueueEntry class that populates it */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QMessageBox>

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
   TRACECF(debugClass, "Constructor. title=%s, text=%s, icon=%d",
                       qs2s(title), qs2s(text), icon);
}


MsgBoxQueueEntry::~MsgBoxQueueEntry()
{
   TRACECF(debugClass, "Executing");
}


QString MsgBoxQueueEntry::repr() {
   // printf("(MsgBoxQueueEntry::repr)\n"); fflush(stdout);
   // if (debugClass) {
   //     TRACEC("  title=%s", qs2s(_boxTitle));
   //     TRACEC("  text=%s", qs2s(_boxText));
   //     TRACEC("  icon=%d", _boxIcon);
   //  }

   QString msg = QString("[title=0x%1, text=%2, icon=%3]")
                    .arg(_boxTitle)
                    .arg(_boxText)
                    .arg(_boxIcon);

   // printf("(MsgBoxQueueEntry::repr) returning: %s\n", qs2s(msg)); fflush(stdout);
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

    QString r = request->repr();

    char * s = strdup(qs2s(request->repr()));
    TRACECF(debugFunc, "=> Starting. request: %s, about to lock", s);
    free(s);

    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();

    // dbgrpt_nolock();

    _mutex.unlock();
    TRACECF(debugClass, "Done");
}


MsgBoxQueueEntry * MsgBoxQueue::pop() {
    TRACECF(debugClass, "Starting");
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
    const char * s = qs2s(rqst->repr());
    TRACECF(debugClass, "<- Done. Returning request: %s", s );
    return rqst;
}


void MsgBoxQueue::dbgrpt_nolock() {
   int ct = _queue.size();
   printf("Queue contains %d entries", ct);
   for (int ndx = 0; ndx < ct; ndx++) {
      MsgBoxQueueEntry * rqst = _queue.at(ndx);
      printf("   %s\n", qs2s( rqst->repr() ) );
   }
}


void MsgBoxQueue::dbgrpt() {
   _mutex.lock();
   dbgrpt_nolock();
   _mutex.unlock();

}
