/* msgbox_thread.h */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MSGBOX_THREAD_H
#define MSGBOX_THREAD_H

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "nongui/msgbox_queue.h"

class QString;

class MsgBoxThread : public QThread
{
   Q_OBJECT

public:
    MsgBoxThread(
        MsgBoxQueue    *requestQueue);
    void run() override;

public slots:
    void msbgoxClosed(int result);

signals:
    void postSerialMsgBox(QString boxTitle, QString boxText, QMessageBox::Icon boxIcon);

private:
    MsgBoxQueue*          _requestQueue = NULL;
    const char *          _cls = "MsgBoxThread";
};

#endif // MSGBOX_THREAD_H
