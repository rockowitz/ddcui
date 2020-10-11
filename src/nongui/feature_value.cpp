/* feature_value.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// #include <QMetaType>

#include <assert.h>
#include <string.h>

#include "ddcutil_types.h"
#include "ddcutil_c_api.h"

#include "base/core.h"
#include "nongui/feature_value.h"

int FeatureValue::nextId = 0;

FeatureValue::FeatureValue()
{
   // memset(&_finfo, 0, sizeof(_finfo));  // avoid -Wmissing-field-initializers
   _finfo = nullptr;
   _value.mh = _value.ml = _value.sh = _value.sl = 0;
   _id = ++nextId;
}

FeatureValue::FeatureValue(
        uint8_t                  feature_code,
        DDCA_Display_Ref         dref,
        DDCA_Feature_Metadata *  finfo,
        DDCA_Cap_Vcp *           cap_vcp,
        DDCA_Non_Table_Vcp_Value val,
        DDCA_Status              getvcpStatus)
    : _featureCode(feature_code)
    , _dref(dref)
    , _finfo(finfo)
    , _capVcp(cap_vcp)
    , _value(val)
    , _getvcpStatus(getvcpStatus)
 //   , _cls( strdup(metaObject()->className()) )
{
     //  _cls             = strdup(metaObject()->className());
//    _feature_code    = feature_code;
//    _dref            = dref;
//    _finfo           = finfo;
//    _cap_vcp         = cap_vcp;
//    _value           = val;

   _cls = "FeatureValue";
   _id = ++nextId;

   bool debugFunc = false;
   // debugFunc = debugFunc || (_featureCode == 0x14);
   if (debugFunc) {
      TRACEC("_id=%d, feature_code=0x%02x, finfo=%p, cap_vcp=%p, sh=0x%02x, sl=0x%02x",
              _id, _featureCode, _finfo, cap_vcp, val.sh, val.sl);
      // ddca_dbgrpt_feature_metadata(_finfo, 2);
      // fflush(stdout);
   }
   if (_finfo) {  // when would it be NULL ? if getvcp failed
       assert(_featureCode  == _finfo->feature_code);

       // alt: if (_finfo->feature_flags & DDCA_SIMPLE_NC )
       if (_finfo->sl_values) {
          _observedNcValues = bs256_add(EMPTY_BIT_SET_256, val.sl);
       }
   }
   TRACECF(debugFunc, "Done. _observedNcValues = %s", bs256_to_string(_observedNcValues, "", " "));
}


FeatureValue::~FeatureValue() {
   bool debugFunc = false;
   debugFunc = debugFunc || (_featureCode == 0x14);
   TRACEC("Executing. _id=%d, _featureCode=0x%02x", _id, _featureCode);

   ddca_free_display_ref(_dref);    // n. not checking return code
   ddca_free_feature_metadata(_finfo);
}


uint8_t
FeatureValue::featureCode() const {
   return _featureCode;
}

DDCA_Display_Ref
FeatureValue::dref()   const {
   return _dref;
}

DDCA_MCCS_Version_Spec
FeatureValue::vspec()  const {
   return _finfo->vcp_version;
}

DDCA_Feature_Metadata *
FeatureValue::finfo() const {
   return _finfo;
}

DDCA_Feature_Flags
FeatureValue::flags() const {
   return _finfo->feature_flags;
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
   bool debugFunc = false;  // (_featureCode == 0x14);
   _value.sh = sh;
   _value.sl = sl;

   if (_finfo->sl_values) {
      _observedNcValues = bs256_add(_observedNcValues, _value.sl);
   }

   TRACECF(debugFunc, "Finished. _observedNcValues = %s", bs256_to_string(_observedNcValues, "", " "));
}


void
FeatureValue::setCurrentValue(uint16_t newval) {
   setCurrentValue(newval >> 8, newval & 0xff);
   ///_value.sh = newval >> 8;
   //_value.sl = newval & 0xff;
   //if (_finfo->sl_values) {
   //   _observedNcValues = bs256_add(_observedNcValues, _value.sl);
   // }
}

DDCA_Status FeatureValue::ddcrc() const {
   return _getvcpStatus;
}

// void FeatureValue::setObservedValues(Bit_Set_256 values) {
//    _observedValues = values;
// }

Bit_Set_256 FeatureValue::observedNcValues() const {
   return _observedNcValues;
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
    printf("   _id:              %d\n",    _id);
    printf("   _feature_code:    0x%02x\n", _featureCode);
    printf("   dref:             %s\n",    ddca_dref_repr(dref()));
 // printf("   _vspec:           %d.%d\n", _finfo.vspec.major, _finfo.vspec.minor);
    printf("   _feature_flags:   0x%04x\n", _finfo->feature_flags);
    printf("   _value.mh:        0x%02x\n", _value.mh);
    printf("   _value.ml:        0x%02x\n", _value.ml);
    printf("   _value.sh:        0x%02x\n", _value.sh);
    printf("   _value.sl:        0x%02x\n", _value.sl);
    printf("   _cap_vcp:         %p\n",     _capVcp);
    printf("   _observedNcValues: %s", bs256_to_string(_observedNcValues, ""," "));
    ddca_dbgrpt_feature_metadata(_finfo, 1);

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
