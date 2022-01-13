// ddca_simulator.cpp

// Copyright (C) 2020-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ddcutil_status_codes.h"
#include "ddcutil_types.h"
#include "ddcutil_c_api.h"

#include <QtCore/QHash>

#include "base/core.h"

#include "ddca_simulator.h"

typedef struct {
   uint8_t feature_code;
   const char * feature_name;
   DDCA_MCCS_Version_Spec vspec;
   DDCA_Status ddcrc;
   uint8_t mh;
   uint8_t ml;
   uint8_t sh;
   uint8_t sl;
} SimTableEntry;

SimTableEntry simTable[] = {
      {0x62, "Volume",       DDCA_VSPEC_V21, DDCRC_OK, 0,100, 0, 50},
      {0x62, "Volume",       DDCA_VSPEC_V22, DDCRC_OK, 0,0,0,20},
      {0x8f, "Audio treble", DDCA_VSPEC_V21, DDCRC_OK, 0,100,0,30},
      {0x8f, "Audio treble", DDCA_VSPEC_V22, DDCRC_OK, 0,0,0,50},
      {0x91, "Audio bass",   DDCA_VSPEC_V21, DDCRC_DETERMINED_UNSUPPORTED, 0,0,0,0},
      {0x91, "Audio bass",   DDCA_VSPEC_V22, DDCRC_DETERMINED_UNSUPPORTED, 0,0,0,0},
//    {0x10, "Brightness",   DDCA_VSPEC_V21, DDCRC_DDC_DATA, 0,0,0,0}
      {0x1a, "Blue Gain",   DDCA_VSPEC_V21, DDCRC_DDC_DATA, 0,0,0,0}
};
int simTableCt = sizeof(simTable)/sizeof(SimTableEntry);

// Maintains values that have been set by simulated ddca_set_non_table_vcp_value():
QHash<uint8_t,uint16_t> simVals;


static bool vspec_eq(DDCA_MCCS_Version_Spec vspec1, DDCA_MCCS_Version_Spec vspec2) {
   bool result = false;
   if (vspec1.major == vspec2.major && vspec1.minor == vspec2.minor)
      result = true;
   return result;
}


int findSimTableEntry(uint8_t feature_code, DDCA_MCCS_Version_Spec vspec) {
   int result = -1;
   for (int ndx = 0; ndx < simTableCt; ndx++) {
      if (simTable[ndx].feature_code == feature_code &&
          vspec_eq( simTable[ndx].vspec, vspec) )
      {
         result = ndx;
         break;
      }
   }
   // printf("(%s) feature_code=0x%02x, vspec=%d.%d, returning %d\n",
   //        __func__, feature_code, vspec.major, vspec.minor, result);
   return result;
}


DdcaSimulator::DdcaSimulator() {
   _cls = strdup(metaObject()->className());
}

DdcaSimulator::~DdcaSimulator() {
   free((void*)_cls);
}


bool  DdcaSimulator::DdcaSimulator::simulateGetNonTableVcpValue(
      DDCA_MCCS_Version_Spec     vspec,
      DDCA_Display_Handle        dh,
      uint8_t                    featureCode,
      DDCA_Non_Table_Vcp_Value*  valrec,
      DDCA_Status *              pddcrc)
{
   bool debug = true;
   if (!simulationEnabled)
      return false;

   bool simulated = false;
   DDCA_Status ddcrc = DDCRC_DETERMINED_UNSUPPORTED;
   int ndx = findSimTableEntry(featureCode, vspec);
   if (ndx >= 0) {
      valrec->mh = simTable[ndx].mh;
      valrec->ml = simTable[ndx].ml;
      valrec->sh = simTable[ndx].sh;
      valrec->sl = simTable[ndx].sl;
      ddcrc      = simTable[ndx].ddcrc;

      if (ddcrc == DDCRC_OK) {
         if (simVals.contains(featureCode)) {
            // override with value set by a previous call
            uint16_t savedVal = simVals.value(featureCode);
            valrec->sh =  savedVal >> 8;
            valrec->sl =  savedVal & 0xff;
            // TRACEC("feature 0x%02x, returning simulated sh=0x%02x, sl=0x%02x",
            //       featureCode, valrec->sh, valrec->sl);
         }
      }
      *pddcrc = ddcrc;
      simulated = true;
   }
   if (simulated)
      TRACECF(debug,
           "Feature 0x%02x, vspec=%d.%d, returning %s. ddcrc=%s. mh=0x%02x, ml=0x%02x, sh=0x%02x, sl=0x%02x",
            featureCode, vspec.major, vspec.minor,
            SBOOL(simulated),  ddca_rc_name(ddcrc), valrec->mh, valrec->ml, valrec->sh, valrec->sl);
   return simulated;
}


bool DdcaSimulator::simulateSetNonTableVcpValue(
      DDCA_MCCS_Version_Spec     vspec,
      DDCA_Display_Handle        dh,
      uint8_t                    featureCode,
      uint8_t                    sh,
      uint8_t                    sl,
      DDCA_Status *              pddcrc)
{
   bool debug = true;
   if (!simulationEnabled)
      return false;

   bool simulated = false;
   *pddcrc = DDCRC_OK;
   int ndx = findSimTableEntry(featureCode, vspec);
   if (ndx >= 0) {
      uint16_t shsl = sh << 8 | sl;
      simVals.insert(featureCode, shsl);
      simulated = true;
      TRACEC("Simulating set feature 0x%02x, shsl=0x%04x", featureCode, shsl);
      *pddcrc = DDCRC_OK;
   }
   if (simulated)
      TRACECF(debug, "Feature 0x%02x, vspec=%d.%d. returning %s. ddcrc=%s",
           featureCode, vspec.major, vspec.minor, SBOOL(simulated),  ddca_rc_name(*pddcrc) );
    return simulated;
}

