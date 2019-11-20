// user_interfacee_options_state.cpp

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// NB was originally named ui_options_state.h etc, but the ui prefix is
// special, causes the user interface compile to look for a dialog named ui_options_state_dialog

#include "user_interface_options_state.h"
#include "base/ddcui_globals.h"


 UserInterfaceOptionsState::UserInterfaceOptionsState()
     : _cls(metaObject()->className())
 {
 }


 void UserInterfaceOptionsState::setControlKeyRequired(bool onoff) {
    bool old =   controlKeyRequired;
    bool newControlKeyRequired = onoff;
    TRACEC("old = %s, new = %s", sbool(controlKeyRequired), sbool(onoff));

    if (newControlKeyRequired != old) {
        controlKeyRequired = newControlKeyRequired;
        TRACEC("emitting ckrChanged(%s)", sbool(controlKeyRequired));
        emit controlKeyRequired_changed(controlKeyRequired);
    }
 }
