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
    FeatureValue() {}

    FeatureValue(
            uint8_t                   feature_code,
            DDCA_Display_Ref          dref,      // dref or finfo?  which?
            DDCA_Feature_Metadata     finfo,
            DDCA_Cap_Vcp *            cap_vcp,
            DDCA_Non_Table_Vcp_Value  val);

    // need a destructor

    uint8_t                  featureCode() const;
    DDCA_Display_Ref         dref()   const;
 // DDCA_MCCS_Version_Spec   vspec()  const;
    DDCA_Feature_Metadata    finfo()  const;
    DDCA_Feature_Flags       flags()  const;
    DDCA_Non_Table_Vcp_Value val()    const;
 // DDCA_Monitor_Model_Key   mmid()   const;

    void dbgrpt();

    // eventually private?
    uint8_t                  _feature_code;
    DDCA_Display_Ref         _dref;
    DDCA_Feature_Metadata    _finfo;
    DDCA_Cap_Vcp *           _cap_vcp;
    DDCA_Non_Table_Vcp_Value _value;    // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
};

Q_DECLARE_METATYPE(FeatureValue)

#endif // FEATURE_VALUE_H
