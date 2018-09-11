/* msgbox_queue.cpp - MsgBoxQueue and the MsgBoxQueueEntry class that populates it */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "base/ddcui_globals.h"
#include "nongui/msgbox_queue.h"


static bool debug = false;

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
   PRINTFTCMF(debug, "Executing");
}


MsgBoxQueueEntry::~MsgBoxQueueEntry()
{
   PRINTFTCMF(debug, "Executing");
}


QString MsgBoxQueueEntry::repr() {
   // printf("(MsgBoxQueueEntry::repr)\n"); fflush(stdout);
   QString msg = QString("[title=0x%1, text=%2, icon=%3]")
                    .arg(_boxTitle)
                    .arg(_boxText)
                    .arg(_boxIcon);
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
    PRINTFTCMF(debug, "Starting. request: %s, about to lock", s);
    free(s);
    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();
    _mutex.unlock();
    PRINTFTCMF(debug, "Done");
}


MsgBoxQueueEntry * MsgBoxQueue::pop() {
    PRINTFTCMF(debug, "Starting");
    _mutex.lock();
    if (_queue.empty())
        _queueNonempty.wait(&_mutex);

    MsgBoxQueueEntry * rqst = _queue.dequeue();

    _mutex.unlock();
    PRINTFTCMF(debug, "Done. Returning request: %s", qs2s(rqst->repr()) );
    return rqst;
}


