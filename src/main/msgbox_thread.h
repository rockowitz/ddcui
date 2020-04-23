/** \file msgbox_thread.h
 *
 * This thread reads from a queue of error messages to be displayed in a
 *  dialog box. This avoids a flurry of simultaneous dialog boxes.
 */

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

signals:
    void postSerialMsgBox(QString boxTitle, QString boxText, QMessageBox::Icon boxIcon);

public slots:
    void msbgoxClosed(int result);


private:
     // void showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon);

    MsgBoxQueue*          _requestQueue = NULL;
    // QWidget*               _parent = NULL;
    const char *          _cls = "MsgBoxThread";
    const int             n = 1;
};

#endif // MSGBOX_THREAD_H
