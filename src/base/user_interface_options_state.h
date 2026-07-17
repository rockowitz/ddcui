/** @file user_interface_options_state.h
 *  Maintains the current state of the User Interface Options dialog
 */
// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef USER_INTERFACE_OPTIONS_STATE_H_
#define USER_INTERFACE_OPTIONS_STATE_H_

#include <QObject>

#include "cmdline/ddcui_parsed_cmd.h"

class UserInterfaceOptionsState : public QObject {
   Q_OBJECT
   Q_DISABLE_COPY(UserInterfaceOptionsState)   // a QObject is not meaningfully copyable

public:
   UserInterfaceOptionsState();
   UserInterfaceOptionsState(Parsed_Ddcui_Cmd* parsed_cmd);

   static const bool CkrDefault = false;

   bool _controlKeyRequired = CkrDefault;

   void setControlKeyRequired(bool onoff);

signals:
   void controlKeyRequired_changed(bool onoff);

private:
   const char * _cls;
};

void init_user_interface_options_state();

#endif /* USER_INTERFACE_OPTIONS_STATE_H_ */
