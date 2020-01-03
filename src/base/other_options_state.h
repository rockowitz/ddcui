// other_options_state.h
//
// Maintains the current state of the NC Feature Values dialog

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef OTHER_OPTIONS_STATE_H_
#define OTHER_OPTIONS_STATE_H_

#include <QtCore/QObject>

// move inside class?
typedef enum {
   NcValuesFromMccs,
   NcValuesFromCapabilities,
   NcValuesFromBoth,
} NcValuesSource;

class OtherOptionsState : public QObject {
   Q_OBJECT

public:
   static const NcValuesSource DefaultNcValuesSource = NcValuesFromCapabilities;

   NcValuesSource ncValuesSource = DefaultNcValuesSource;
};

const char * ncValuesSourceName(NcValuesSource source) ;

#endif /* OPTIONS_STATE_H_ */
