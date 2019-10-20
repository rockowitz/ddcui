// global_state.h

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

#include "base/other_options_state.h"
#include "user_interface_options_state.h"

class GlobalState {

public:
   static GlobalState& instance();

   OtherOptionsState * _otherOptionsState = NULL;
   UserInterfaceOptionsState * _uiOptionsState= NULL;

private:
   GlobalState();
   // No need for destructor.  Created once for life of the program.
};

#endif /* GLOBAL_STATE_H_ */
