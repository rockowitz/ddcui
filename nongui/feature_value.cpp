/* feature_value.cpp */

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
        DDCA_Non_Table_Vcp_Value val)  // use DDCA_Non_Table_Value or individual bytes?
{
    _feature_code    = feature_code;
    _dref            = dref;
    _finfo           = finfo;
    _cap_vcp         = cap_vcp;
    _value           = val;

    assert(_feature_code  == _finfo.feature_code);
}

uint8_t                  FeatureValue::featureCode() const {
   return _feature_code;
}

DDCA_Display_Ref         FeatureValue::dref()   const {
   return _dref;
}


DDCA_MCCS_Version_Spec   FeatureValue::vspec()  const {
   return _finfo.vspec;
}

DDCA_Feature_Metadata    FeatureValue::finfo()  const {
   return _finfo;
}

DDCA_Feature_Flags       FeatureValue::flags()  const {
   return _finfo.feature_flags;
}

DDCA_Non_Table_Vcp_Value FeatureValue::val()    const {
   return _value;
}

DDCA_Monitor_Model_Key   FeatureValue::mmid()   const {
  // return _mmid;
   DDCA_Monitor_Model_Key result = DDCA_UNDEFINED_MONITOR_MODEL_KEY;
   if (_finfo.mmid)
      result = *_finfo.mmid;
   return result;
}


void FeatureValue::dbgrpt() {
    printf("(FeatureValue::report) FeatureValue:\n");
    printf("   _feature_code:   0x%02x\n", _feature_code);
    printf("   dref:            %s\n",    ddca_dref_repr(dref()));
    printf("   _vspec:          %d.%d\n", _finfo.vspec.major, _finfo.vspec.minor);
    printf("   _feature_flags:  0x%04x\n", _finfo.feature_flags);
    printf("   _value.mh:             0x%02x\n", _value.mh);
    printf("   _value.ml:             0x%02x\n", _value.ml);
    printf("   _value.sh:             0x%02x\n", _value.sh);
    printf("   _value.sl:             0x%02x\n", _value.sl);
    printf("   _cap_vcp:              %p\n",     _cap_vcp);

    if (_finfo.mmid) {
       printf("   _finfo.mmid->mfg_id:       %s\n", _finfo.mmid->mfg_id);
       printf("   _finfo.mmid->model_name:   %s\n", _finfo.mmid->model_name);
       printf("   _finfo.mmid->product_code: %u\n", _finfo.mmid->product_code);
    }
    else {
       printf("   _finfo.mmid:          Not set\n");
    }
    fflush(stdout);
}
