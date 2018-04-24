/* feature_value.h */

#ifndef FEATURE_VALUE_H
#define FEATURE_VALUE_H

#include <QtCore/QMetaType>

#include "ddcutil_types.h"


class FeatureValue
{

public:
    // no-arg constructor needed for Q_METATYPE
    FeatureValue() {}

    explicit     // ???
    FeatureValue(
            uint8_t                   feature_code,
            DDCA_Display_Ref          dref,      // dref or finfo?  which?
            DDCA_Feature_Metadata     finfo,
            DDCA_Cap_Vcp *            cap_vcp,
            DDCA_Non_Table_Vcp_Value  val);

    // need a destructor

    uint8_t                  featureCode() const;
    DDCA_Display_Ref         dref()   const;
    DDCA_MCCS_Version_Spec   vspec()  const;
    DDCA_Feature_Metadata    finfo()  const;
    DDCA_Feature_Flags       flags()  const;
    DDCA_Non_Table_Vcp_Value val()    const;
    DDCA_Monitor_Model_Key   mmid()   const;

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
