/* msgbox_queue.cpp - MsgBoxQueue and the classes that populate it */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "base/ddcui_globals.h"
#include "nongui/msgbox_queue.h"

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
   PRINTFTCM("Executing");
}

MsgBoxQueueEntry::~MsgBoxQueueEntry()
{
}


QString MsgBoxQueueEntry::repr() {
   // printf("(MsgBoxQueueEntry::repr)\n"); fflush(stdout);
   QString msg = QString("[title=0x%1, text=%2]")
                    .arg(_boxTitle)
                    .arg(_boxText)
                    ;
   return msg;
}


/* MsgBoxQueue */

MsgBoxQueue::MsgBoxQueue()
{
    _queue = QQueue<MsgBoxQueueEntry*>();
}

void MsgBoxQueue::put(MsgBoxQueueEntry * request) {
    // PRINTFTCM("Starting");
    // QString r = request->repr();
    char * s = strdup(qs2s(request->repr()));
    PRINTFTCM("Starting. request: %s, about to lock", s);
    free(s);
    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();
    _mutex.unlock();
    PRINTFTCM("Done");
}

MsgBoxQueueEntry * MsgBoxQueue::pop() {
    _mutex.lock();
    if (_queue.empty())
        _queueNonempty.wait(&_mutex);

    MsgBoxQueueEntry * rqst = _queue.dequeue();

    _mutex.unlock();
    return rqst;
}

// MsgBoxQueueEntry MsgBoxQueue::peek() { }

