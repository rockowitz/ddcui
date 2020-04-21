/* msgbox_thread.h */

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MSGBOX_THREAD_H
#define MSGBOX_THREAD_H

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtWidgets/QMessageBox>

#include "nongui/msgbox_queue.h"

class QString;

class MsgBoxThread : public QThread
{
   Q_OBJECT

public:
    MsgBoxThread(MsgBoxQueue *requestQueue);
    void run() override;



public slots:
    void msbgoxClosed(int result);

signals:
    void postSerialMsgBox(QString boxTitle, QString boxText, QMessageBox::Icon boxIcon);

private:
    void showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon);

    MsgBoxQueue*          _requestQueue = NULL;
    // QWidget*               _parent = NULL;
    const char *          _cls = "MsgBoxThread";
    const int             n = 1;
    // how to create a QSemaphore member variable?
    // QSemaphore            msgboxSemaphore2(1);
};

#endif // MSGBOX_THREAD_H
