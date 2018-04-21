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
        //  DDCA_MCCS_Version_Spec    vspec,
        //  DDCA_Monitor_Model_Key *  mmid,
        //  DDCA_Feature_Flags        feature_flags,
            DDCA_Non_Table_Vcp_Value  val);  // or use individual bytes?

    // need a destructor

    uint8_t                  featureCode() const;
    DDCA_Display_Ref         dref()   const;
    DDCA_MCCS_Version_Spec   vspec()  const;
    DDCA_Feature_Metadata    finfo()  const;
    DDCA_Feature_Flags       flags()  const;
    DDCA_Non_Table_Vcp_Value val()    const;
    DDCA_Monitor_Model_Key   mmid()   const;

    void report();

    uint8_t                  _feature_code;
    DDCA_Display_Ref         _dref;
    DDCA_Feature_Metadata    _finfo;
//  DDCA_MCCS_Version_Spec   _vspec;
//  DDCA_Monitor_Model_Key   _mmid;
    DDCA_Non_Table_Vcp_Value _value;    // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
    // uint8_t                  _mh;
    // uint8_t                  _ml;
    // uint8_t                  _sh;
    // uint8_t                  _sl;
 // DDCA_Feature_Flags       _feature_flags;  // should this be here or looked up?
};

Q_DECLARE_METATYPE(FeatureValue)

#endif // FEATURE_VALUE_H
