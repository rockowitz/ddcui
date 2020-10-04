// other_options_state.h
//
// Maintains the current state of the NC Feature Values dialog

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef OTHER_OPTIONS_STATE_H_
#define OTHER_OPTIONS_STATE_H_

#include <QtCore/QObject>

#include "cmdline/parsed_cmd.h"

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
   static const bool           DefaultUseMaximalNcValueNames = false;

   OtherOptionsState();
   OtherOptionsState(Parsed_Cmd * parsedCmd);
   OtherOptionsState(const OtherOptionsState &other);

   void changeNcValuesSource(NcValuesSource mode, bool useLatestNcValueNames);

   NcValuesSource _ncValuesSource = DefaultNcValuesSource;
   bool           _useLatestNcValueNames = DefaultUseMaximalNcValueNames;

signals:
   void ncValuesSourceChanged(NcValuesSource valuesSource, bool latestNcNameValues);

};

const char * ncValuesSourceName(NcValuesSource source) ;

#endif /* OPTIONS_STATE_H_ */
