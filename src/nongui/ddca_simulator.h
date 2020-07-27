// ddca_simulator.h - Basic API call simulator for libddcutil

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCA_SIMULATOR_H_
#define DDCA_SIMULATOR_H_

#include <QtCore/QObject>

#include "ddcutil_types.h"

class DdcaSimulator: public QObject {
   Q_OBJECT;

public:
   DdcaSimulator();

   bool simulateGetNonTableVcpValue(
         DDCA_MCCS_Version_Spec      vspec,
         DDCA_Display_Handle        dh,
         uint8_t                    featureCode,
         DDCA_Non_Table_Vcp_Value*  valrec,
         DDCA_Status *              pddcrc);

   bool simulateSetNonTableVcpValue(
         DDCA_MCCS_Version_Spec     vspec,
         DDCA_Display_Handle        dh,
         uint8_t                    featureCode,
         uint8_t                    sh,
         uint8_t                    sl,
         DDCA_Status *              pddcrc);

private:
   const char *   _cls;    // className
   bool           simulationEnabled = false;  // eventually make this externally controllable
};

#endif /* DDCA_SIMULATOR_H_ */
