/* global_state.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
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

   // static GlobalState * _theOneAndOnly = nullptr;


};

#endif /* GLOBAL_STATE_H_ */
