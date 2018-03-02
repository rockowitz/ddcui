/* feature_value.cpp */

#include <QMetaType>
#include <string.h>
#include "ddcutil_types.h"

#include "feature_value.h"

FeatureValue::FeatureValue() {}

FeatureValue::FeatureValue(
        uint8_t                feature_code,
        DDCA_MCCS_Version_Spec vspec,
        DDCA_Feature_Flags     feature_flags,
        DDCA_Non_Table_Value   val)  // use DDCA_Non_Table_Value or individual bytes?
{
    _feature_code    = feature_code;
    _vspec           = vspec;
    _feature_flags   = feature_flags;
    _mh              = val.mh;
    _ml              = val.ml;
    _sh              = val.sh;
    _sl              = val.sl;
    _value           = val;
}

void FeatureValue::report() {
    printf("(FeatureValue::report) FeatureValue:\n");
    printf("   _feature_code:   0x%02x\n", _feature_code);
    printf("   _vspec:          %d.%d\n", _vspec.major, _vspec.minor);
    printf("   _feature_flags:  0x%04x\n", _feature_flags);
    printf("   _mh:             0x%02x\n", _mh);
    printf("   _ml:             0x%02x\n", _ml);
    printf("   _sh:             0x%02x\n", _sh);
    printf("   _sl:             0x%02x\n", _sl);
    fflush(stdout);
}
