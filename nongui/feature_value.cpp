/* feature_value.cpp */

// #include <QMetaType>
#include "nongui/feature_value.h"

#include "assert.h"
#include <string.h>
#include "ddcutil_types.h"
#include "ddcutil_c_api.h"


// FeatureValue::FeatureValue() {}

FeatureValue::FeatureValue(
        uint8_t                  feature_code,
        DDCA_Display_Ref         dref,
        DDCA_Feature_Metadata    finfo,
        DDCA_MCCS_Version_Spec   vspec,
        DDCA_Monitor_Model_Key * mmid,
        DDCA_Feature_Flags       feature_flags,
        DDCA_Non_Table_Vcp_Value val)  // use DDCA_Non_Table_Value or individual bytes?
{
    _feature_code    = feature_code;
    _dref            = dref;
    _finfo           = finfo;
    _vspec           = vspec;
    if (mmid)
       _mmid            = *mmid;
    else
       _mmid             = DDCA_UNDEFINED_MONITOR_MODEL_KEY;

    _feature_flags   = feature_flags;

    assert(_feature_code  == _finfo.feature_code);
    assert(_feature_flags == _finfo.feature_flags);
    // assert(_vspec         == _finfo.vspec);   // no operator== for DDCA_MCCS_Version_Spec

  //   _mh              = val.mh;
  //   _ml              = val.ml;
  //   _sh              = val.sh;
  //   _sl              = val.sl;
    _value           = val;
    // assert(false);
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
   return _mmid;
}



void FeatureValue::report() {
    printf("(FeatureValue::report) FeatureValue:\n");
    printf("   _feature_code:   0x%02x\n", _feature_code);
  //printf("   _dref:           %s\n",    ddca_dref_repr(_dref));
    printf("   _vspec:          %d.%d\n", _vspec.major, _vspec.minor);
    printf("   _feature_flags:  0x%04x\n", _feature_flags);
    printf("   _value.mh:             0x%02x\n", _value.mh);
    printf("   _value.ml:             0x%02x\n", _value.ml);
    printf("   _value.sh:             0x%02x\n", _value.sh);
    printf("   _value.sl:             0x%02x\n", _value.sl);
    if (_mmid.defined) {
       printf("   _mmid->mfg_id:       %s\n", _mmid.mfg_id);
       printf("   _mmid->model_name:   %s\n", _mmid.model_name);
       printf("   _mmid->product_code: %u\n", _mmid.product_code);
    }
    else {
       printf("   _mmid:          Not set\n");
    }
    fflush(stdout);
}
