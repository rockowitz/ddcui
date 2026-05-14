/** @file nc_values_state.h
 *  Maintains the current state of the NC Feature Values dialog
 */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NC_VALUES_STATE_H_
#define NC_VALUES_STATE_H_

#include <QObject>

#include "cmdline/ddcui_parsed_cmd.h"

// move inside class?
typedef enum {
   NcValuesFromMccs,
   NcValuesFromCapabilities,
   NcValuesFromBoth,
} NcValuesSource;


class NcValuesState : public QObject {
   Q_OBJECT

public:
   static const NcValuesSource DefaultNcValuesSource = NcValuesFromCapabilities;
   static const bool           DefaultUseMaximalMccsNcValues = false;

   NcValuesState();
   NcValuesState(Parsed_Ddcui_Cmd * parsedCmd);
   NcValuesState(const NcValuesState &other);

   void changeNcValuesSource(NcValuesSource mode, bool useLatestNcValues);

   NcValuesSource _ncValuesSource = DefaultNcValuesSource;
   // bool           _useLatestNcValues = DefaultUseMaximalMccsNcValues;

signals:
   void ncValuesSourceChanged(NcValuesSource valuesSource, bool latestNcValues);
};

const char * ncValuesSourceName(NcValuesSource source) ;

#endif /* NC_VALUES_STATE_H_ */
