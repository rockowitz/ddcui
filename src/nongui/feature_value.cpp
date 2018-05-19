/* feature_value.cpp */

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

// #include <QMetaType>

#include <assert.h>
#include <string.h>

#include "ddcutil_types.h"
#include "ddcutil_c_api.h"

#include "nongui/feature_value.h"


// FeatureValue::FeatureValue() {}

FeatureValue::FeatureValue(
        uint8_t                  feature_code,
        DDCA_Display_Ref         dref,
        DDCA_Feature_Metadata    finfo,
        DDCA_Cap_Vcp *           cap_vcp,
        DDCA_Non_Table_Vcp_Value val)
    : _featureCode(feature_code)
    , _dref(dref)
    , _finfo(finfo)
    , _capVcp(cap_vcp)
    , _value(val)
{
//    _feature_code    = feature_code;
//    _dref            = dref;
//    _finfo           = finfo;
//    _cap_vcp         = cap_vcp;
//    _value           = val;

    assert(_featureCode  == _finfo.feature_code);
}

FeatureValue::~FeatureValue() {
   ddca_free_display_ref(_dref);    // n. not checking return code
   ddca_free_feature_metadata_contents(_finfo);
}


uint8_t
FeatureValue::featureCode() const {
   return _featureCode;
}

DDCA_Display_Ref
FeatureValue::dref()   const {
   return _dref;
}

// DDCA_MCCS_Version_Spec   FeatureValue::vspec()  const {
//    return _finfo.vspec;
// }

DDCA_Feature_Metadata
FeatureValue::finfo() const {
   return _finfo;
}

DDCA_Feature_Flags
FeatureValue::flags() const {
   return _finfo.feature_flags;
}

DDCA_Cap_Vcp * FeatureValue::capVcp() const {
   return _capVcp;
}

DDCA_Non_Table_Vcp_Value
FeatureValue::val()    const {
   return _value;
}

void
FeatureValue::setCurrentValue(uint8_t sh, uint8_t sl) {
   _value.sh = sh;
   _value.sl = sl;
}

void
FeatureValue::setCurrentValue(uint16_t newval) {
   _value.sh = newval >> 8;
   _value.sl = newval & 0xff;
}



//DDCA_Monitor_Model_Key   FeatureValue::mmid()   const {
//  // return _mmid;
//   DDCA_Monitor_Model_Key result = DDCA_UNDEFINED_MONITOR_MODEL_KEY;
//   if (_finfo.mmid)
//      result = *_finfo.mmid;
//   return result;
//}


void FeatureValue::dbgrpt() const {
    printf("(FeatureValue::report) FeatureValue:\n");
    printf("   _feature_code:   0x%02x\n", _featureCode);
    printf("   dref:            %s\n",    ddca_dref_repr(dref()));
 // printf("   _vspec:          %d.%d\n", _finfo.vspec.major, _finfo.vspec.minor);
    printf("   _feature_flags:  0x%04x\n", _finfo.feature_flags);
    printf("   _value.mh:             0x%02x\n", _value.mh);
    printf("   _value.ml:             0x%02x\n", _value.ml);
    printf("   _value.sh:             0x%02x\n", _value.sh);
    printf("   _value.sl:             0x%02x\n", _value.sl);
    printf("   _cap_vcp:              %p\n",     _capVcp);

#ifdef NO
    if (_finfo.mmid) {
       printf("   _finfo.mmid->mfg_id:       %s\n", _finfo.mmid->mfg_id);
       printf("   _finfo.mmid->model_name:   %s\n", _finfo.mmid->model_name);
       printf("   _finfo.mmid->product_code: %u\n", _finfo.mmid->product_code);
    }
    else {
       printf("   _finfo.mmid:          Not set\n");
    }
#endif
    fflush(stdout);
}