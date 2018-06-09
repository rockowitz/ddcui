/* global_state.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </copyright>
 */

#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

#include "base/other_options_state.h"

class GlobalState {

public:
   static GlobalState& instance();

   OtherOptionsState * _otherOptionsState = NULL;

private:
   GlobalState();
   // No need for destructor.  Created once for life of the program.
};

#endif /* GLOBAL_STATE_H_ */
