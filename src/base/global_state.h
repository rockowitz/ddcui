/** @file global_state.h
 *  Singleton class maintaining pointers to key class instances
 */

// Copyright (C) 2018-2024 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

#include "base/user_interface_options_state.h"
#include "base/nc_values_state.h"

 // ugh - referencing out of base, to clean up:
#include "cmdline/ddcui_parsed_cmd.h"
#include "main/callback_manager.h"
#include "main/mainwindow.h"

class GlobalState {
   // Q_OBJECT

public:
   static GlobalState& instance();

   Parsed_Ddcui_Cmd *          _parsed_cmd     = nullptr;
   NcValuesState *             _ncValuesState  = nullptr;
   UserInterfaceOptionsState * _uiOptionsState = nullptr;

   QApplication *    _application  = nullptr;
   MainWindow *      _mainWindow   = nullptr;
   MsgBoxThread *    _msgBoxThread = nullptr;
   MsgBoxQueue *     _msgBoxQueue  = nullptr;
   CallbackManager * _callbackManager = nullptr;

private:
   GlobalState();
   // No need for destructor.  Created once for life of the program.
};

#endif /* GLOBAL_STATE_H_ */
