// global_state.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "global_state.h"

GlobalState::GlobalState() {
   // TODO Auto-generated constructor stub
}

#ifdef PRE_CPP11
GlobalState * GlobalState::getInstance() {
   if (!_theOneAndOnly) {
      _theOneAndOnly = new GlobalState();
   }
   return _theOneAndOnly;
}
#endif

// Static C++11 thread safe way
GlobalState& GlobalState::instance() {
   static GlobalState _instance;
   return _instance;
}

