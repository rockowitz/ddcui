/* feature_selector.h */

#ifndef FEATURE_SELECTOR_H
#define FEATURE_SELECTOR_H

#include "ddcutil_types.h"

#ifdef ALTERNATIVE
typedef struct {
    bool show_unsupported_features;
    bool include_table_features;
    DDCA_Feature_Subset_Id feature_list_id = DDCA_SUBSET_KNOWN;
} Feature_Selector;
#endif


class FeatureSelector {
public:
   //  Feature_Selector();

    DDCA_Feature_Subset_Id feature_list_id = DDCA_SUBSET_KNOWN;
    bool showUnsupportedFeatures = false;
    bool includeTableFeatures = false;
};


#endif // FEATURE_SELECTOR_H
