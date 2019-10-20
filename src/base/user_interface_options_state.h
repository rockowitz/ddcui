// user_interface_options_state.h

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef USER_INTERFACE_OPTIONS_STATE_H_
#define USER_INTERFACE_OPTIONS_STATE_H_

#include <QtCore/QObject>


class UserInterfaceOptionsState : public QObject {
   Q_OBJECT

public:
   static const bool CkrDefault = false;

   bool controlKeyRequired = CkrDefault;

   void setControlKeyRequired(bool onoff);

signals:
   void ckrChanged(bool onoff);
};

#endif /* USER_INTERFACE_OPTIONS_STATE_H_ */
