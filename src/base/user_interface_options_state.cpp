// user_interfacee_options_state.cpp
//
// Maintains the current state of the User Interface Options dialog

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// NB was originally named ui_options_state.h etc, but the ui prefix is
// special, causes the user interface compiler to look for a dialog named ui_options_state_dialog

#include "user_interface_options_state.h"
#include "cmdline/parsed_cmd.h"

#include "core.h"


 UserInterfaceOptionsState::UserInterfaceOptionsState()
     : _cls(metaObject()->className())
 {
 }

 UserInterfaceOptionsState::UserInterfaceOptionsState(Parsed_Cmd* parsed_cmd)
     : _cls(metaObject()->className())
 {
    _controlKeyRequired = parsed_cmd->flags & CMD_FLAG_UI_REQUIRE_CONTROL_KEY;
 }

 UserInterfaceOptionsState::UserInterfaceOptionsState(UserInterfaceOptionsState &other)
     : QObject()
     , _cls(metaObject()->className())
 {
    _controlKeyRequired = other._controlKeyRequired;
 }


 void UserInterfaceOptionsState::setControlKeyRequired(bool onoff) {
    bool debug = false;
    bool old =   _controlKeyRequired;
    bool newControlKeyRequired = onoff;
    TRACECF(debug, "old = %s, new = %s", SBOOL(_controlKeyRequired), SBOOL(onoff));

    if (newControlKeyRequired != old) {
        _controlKeyRequired = newControlKeyRequired;
        TRACECF(debug, "emitting ckrChanged(%s)", SBOOL(_controlKeyRequired));
        emit controlKeyRequired_changed(_controlKeyRequired);
    }
}
