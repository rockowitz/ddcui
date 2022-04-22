/** @file global_state.h
 *  Singleton class maintaining pointers to key class instances
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

// #include <QtWidgets/QtWidgets>   // to define Q_OBJECT

#include "cmdline/ddcui_parsed_cmd.h"
#include "base/user_interface_options_state.h"
#include "main/mainwindow.h"    // ugh - referencing out of base, to clean up
#include "nc_values_state.h"


class GlobalState {
   // Q_OBJECT

public:
   static GlobalState& instance();

   Parsed_Ddcui_Cmd *          _parsed_cmd        = nullptr;
   OtherOptionsState *         _otherOptionsState = nullptr;
   UserInterfaceOptionsState * _uiOptionsState    = nullptr;

   QApplication * _application  = nullptr;
   MainWindow *   _mainWindow   = nullptr;
   MsgBoxThread * _msgBoxThread = nullptr;
   MsgBoxQueue *  _msgBoxQueue  = nullptr;

private:
   GlobalState();
   // No need for destructor.  Created once for life of the program.
};

#endif /* GLOBAL_STATE_H_ */
