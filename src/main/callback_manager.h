// callback_manager.h

// Copyright (C) 2024 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CALLBACK_MANAGER_H_
#define CALLBACK_MANAGER_H_

#include <QtCore/QObject>

#include <ddcutil_c_api.h>

#include "config.h"

#include "main/mainwindow.h"


class CallbackManager : public QObject
{
    Q_OBJECT

public:
    static CallbackManager& instance();

    void registerCallbacks(MainWindow* mainWindow);
    void emitDisplayChanged(DDCA_Display_Status_Event evt);

private:
   CallbackManager();
   // No need for destructor.  Created once for life of the program.
   const char *                 _cls;


public:
   signals:
   void displayChanged(DDCA_Display_Status_Event evt);

};

#endif /* CALLBACK_MANAGER_H_ */
