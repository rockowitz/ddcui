/* feature_value.h */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURE_VALUE_H
#define FEATURE_VALUE_H

#include <atomic>

#include "ddcutil_types.h"

#include "c_util/data_structures.h"

void init_feature_value();


class FeatureValue
{

public:
    // no-arg constructor (previously required by Q_DECLARE_METATYPE)
    FeatureValue();

    FeatureValue(
            uint8_t                   featureCode,
            DDCA_Display_Ref          dref,
            DDCA_Feature_Metadata *   finfo,
            DDCA_Cap_Vcp *            capVcp,
            DDCA_Non_Table_Vcp_Value  val,
            DDCA_Status               getvcpStatus=0  // experiment
            );

    ~FeatureValue();

    uint8_t                  featureCode() const;
    DDCA_Display_Ref         dref()        const;
    DDCA_MCCS_Version_Spec   vspec()       const;
    DDCA_Feature_Metadata*   finfo()       const;
    DDCA_Feature_Flags       flags()       const;
    DDCA_Cap_Vcp *           capVcp()      const;
    DDCA_Non_Table_Vcp_Value val()         const;
    DDCA_Status              ddcrc()       const;
    void                     setCurrentValue(uint8_t sh, uint8_t sl);
    void                     setCurrentValue(uint16_t newval);
    void                     dbgrpt()      const;
    // void                     setObservedValues(Bit_Set_256 values);  // *** EXPERIMENTAL ***
    Bit_Set_256              observedNcValues() const;
    void                     delete_finfo();

 // DDCA_Monitor_Model_Key   mmid()        const;

private:
    // Non-copyable: this class owns _finfo and frees it in the destructor,
    // so a shallow copy would double-free.  (Copyability was previously forced
    // by Q_DECLARE_METATYPE, now removed.)
    FeatureValue(const FeatureValue& original) = delete;
    FeatureValue& operator=(const FeatureValue& original) = delete;


public:
    Bit_Set_256              _observedNcValues = EMPTY_BIT_SET_256;
    int                      _id;

private:
    // atomic: FeatureValue instances are constructed on each monitor's VcpThread
    static std::atomic<int>  nextId;
    const char *             _cls = "FeatureValue";   // the no-arg constructor does not set it
    uint8_t                  _featureCode = 0;
    DDCA_Display_Ref         _dref   = NULL;
    DDCA_Feature_Metadata *  _finfo  = NULL;
    DDCA_Cap_Vcp *           _capVcp = NULL;
    DDCA_Non_Table_Vcp_Value _value;     // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
    DDCA_Status              _getvcpStatus = 0; // an experiment
};

#endif // FEATURE_VALUE_H

