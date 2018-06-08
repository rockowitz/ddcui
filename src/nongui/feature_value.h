/* feature_value.h */

/* <copyright>
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
            DDCA_Feature_Metadata     finfo,
            DDCA_Cap_Vcp *            capVcp,
            DDCA_Non_Table_Vcp_Value  val);

    ~FeatureValue();

    uint8_t                  featureCode() const;
    DDCA_Display_Ref         dref()        const;
    DDCA_Feature_Metadata    finfo()       const;
    DDCA_Feature_Flags       flags()       const;
    DDCA_Cap_Vcp *           capVcp()      const;
    DDCA_Non_Table_Vcp_Value val()         const;
    void                     setCurrentValue(uint8_t sh, uint8_t sl);
    void                     setCurrentValue(uint16_t newval);
    void                     dbgrpt()      const;

 // DDCA_MCCS_Version_Spec   vspec()       const;
 // DDCA_Monitor_Model_Key   mmid()        const;

private:
    uint8_t                  _featureCode = 0;
    DDCA_Display_Ref         _dref = NULL;
    DDCA_Feature_Metadata    _finfo;
    DDCA_Cap_Vcp *           _capVcp = NULL;
    DDCA_Non_Table_Vcp_Value _value;     // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
};

Q_DECLARE_METATYPE(FeatureValue)

#endif // FEATURE_VALUE_H

