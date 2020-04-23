// user_interface_options_state.h
//
// Maintains the current state of the User Interface Options dialog

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef USER_INTERFACE_OPTIONS_STATE_H_
#define USER_INTERFACE_OPTIONS_STATE_H_

#include <QtCore/QObject>

#include "cmdline/parsed_cmd.h"

class UserInterfaceOptionsState : public QObject {
   Q_OBJECT

public:
   UserInterfaceOptionsState();
   UserInterfaceOptionsState(Parsed_Cmd* parsed_cmd);
   UserInterfaceOptionsState(UserInterfaceOptionsState &other);

   static const bool CkrDefault = false;

   bool _controlKeyRequired = CkrDefault;

   void setControlKeyRequired(bool onoff);

signals:
   void controlKeyRequired_changed(bool onoff);

private:
   const char * _cls;
};

#endif /* USER_INTERFACE_OPTIONS_STATE_H_ */
