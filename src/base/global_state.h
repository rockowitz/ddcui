// global_state.h

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

// #include <QtWidgets/QtWidgets>   // to define Q_OBJECT

#include "base/other_options_state.h"
#include "base/user_interface_options_state.h"
#include "main/mainwindow.h"    // ugh - referencing out of base, to clean up


class GlobalState {
   // Q_OBJECT

public:
   static GlobalState& instance();

   OtherOptionsState *         _otherOptionsState = NULL;
   UserInterfaceOptionsState * _uiOptionsState    = NULL;

   MainWindow *   _mainWindow   = NULL;
   MsgBoxThread * _msgBoxThread = NULL;
   MsgBoxQueue *  _msgBoxQueue   = NULL;

private:
   GlobalState();
   // No need for destructor.  Created once for life of the program.
};

#endif /* GLOBAL_STATE_H_ */
