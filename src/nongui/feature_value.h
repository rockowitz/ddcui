/* feature_value.h */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURE_VALUE_H
#define FEATURE_VALUE_H

#include <QtCore/QMetaType>

#include "ddcutil_types.h"


class FeatureValue
{

public:
    // no-arg constructor needed for Q_METATYPE
    FeatureValue();

    FeatureValue(
            uint8_t                   featureCode,
            DDCA_Display_Ref          dref,
            DDCA_Feature_Metadata *   finfo,
            DDCA_Cap_Vcp *            capVcp,
            DDCA_Non_Table_Vcp_Value  val,
            DDCA_Status               getvcpStatus=0  // experiment
            );

    ~FeatureValue();

    uint8_t                  featureCode() const;
    DDCA_Display_Ref         dref()        const;
    DDCA_MCCS_Version_Spec   vspec()       const;
    DDCA_Feature_Metadata*   finfo()       const;
    DDCA_Feature_Flags       flags()       const;
    DDCA_Cap_Vcp *           capVcp()      const;
    DDCA_Non_Table_Vcp_Value val()         const;
    DDCA_Status              ddcrc()       const;
    void                     setCurrentValue(uint8_t sh, uint8_t sl);
    void                     setCurrentValue(uint16_t newval);
    void                     dbgrpt()      const;

 // DDCA_Monitor_Model_Key   mmid()        const;

private:
    // const char *             _cls;
    uint8_t                  _featureCode = 0;
    DDCA_Display_Ref         _dref   = NULL;
    DDCA_Feature_Metadata *  _finfo  = NULL;
    DDCA_Cap_Vcp *           _capVcp = NULL;
    DDCA_Non_Table_Vcp_Value _value;     // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
    DDCA_Status              _getvcpStatus = 0; // an experiment
};

Q_DECLARE_METATYPE(FeatureValue)

#endif // FEATURE_VALUE_H

