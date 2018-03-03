/* feature_value.h */

#ifndef FEATURE_VALUE_H
#define FEATURE_VALUE_H

#include <QtCore/QMetaType>
// #include <QString>
#include "ddcutil_types.h"


class FeatureValue
{

public:
    FeatureValue() {}

    explicit     // ???
    FeatureValue(
            uint8_t                feature_code,
            DDCA_MCCS_Version_Spec vspec,
            DDCA_Feature_Flags     feature_flags,
            DDCA_Non_Table_Value   val);  // use DDCA_Non_Table_Value or individual bytes?

    void report();

    uint8_t                _feature_code;
    DDCA_MCCS_Version_Spec _vspec;
    DDCA_Non_Table_Value   _value;    // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
    uint8_t                _mh;
    uint8_t                _ml;
    uint8_t                _sh;
    uint8_t                _sl;
    DDCA_Feature_Flags     _feature_flags;  // should this be here or looked up?
};

Q_DECLARE_METATYPE(FeatureValue)

#endif // FEATURE_VALUE_H
