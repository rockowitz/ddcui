/* vcprequest.h - MsgBoxQueue and the VcpRequest classes that populate the queue */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MSGBOX_QUEUE_H
#define MSGBOX_QUEUE_H

#include <stdint.h>

#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QWaitCondition>
#include <QtWidgets/QMessageBox>


/* MsgBoxQueue entries */

class MsgBoxQueueEntry
{
public:
    MsgBoxQueueEntry(QString title, QString text, QMessageBox::Icon icon);
    ~MsgBoxQueueEntry();

    QString repr();

    QString           _boxTitle;
    QString           _boxText;
    QMessageBox::Icon _boxIcon;



private:
    const char * _cls = "MsgBoxQueueEntry";
};


/* MsgBoxQueue */

class MsgBoxQueue
{
    // Q_OBJECT

public:
    MsgBoxQueue();

    MsgBoxQueueEntry* pop();
    void              put(MsgBoxQueueEntry* request);

private:
    const char *              _cls = "MsgBoxQueue";
    QMutex                    _mutex;
    QQueue<MsgBoxQueueEntry*> _queue;
    QWaitCondition            _queueNonempty;
};

#endif // MSGBOX_QUEUE_H

