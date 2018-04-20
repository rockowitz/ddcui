/* feature_selector.h */

#ifndef FEATURE_SELECTOR_H
#define FEATURE_SELECTOR_H

#include "ddcutil_types.h"

class FeatureSelector {
public:
   //  FeatureSelector();

    DDCA_Feature_Subset_Id feature_list_id         = DDCA_SUBSET_KNOWN;
    bool                   showUnsupportedFeatures = false;
    bool                   includeTableFeatures    = false;

   bool operator==(const FeatureSelector& sel2) const;
   bool operator!=(const FeatureSelector& sel2) const;

    void dbgrpt();

};

#ifdef ALTERNATIVE
typedef struct {
    bool show_unsupported_features;
    bool include_table_features;
    DDCA_Feature_Subset_Id feature_list_id = DDCA_SUBSET_KNOWN;
} Feature_Selector;
#endif

#endif // FEATURE_SELECTOR_H
