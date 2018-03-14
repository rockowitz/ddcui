/* misc.h */

#ifndef MISC_H
#define MISC_H

#include "ddcutil_types.h"

typedef struct {
    bool show_unsupported_featuers;
    bool include_table_features;
    DDCA_Feature_Subset_Id feature_list_id = DDCA_SUBSET_KNOWN;
} Feature_Selector;


class FeatureSelector {
public:
   //  Feature_Selector();

    DDCA_Feature_Subset_Id feature_list_id = DDCA_SUBSET_KNOWN;
    bool showUnsupportedFeatures = false;
    bool includeTableFeatures = false;
};


#endif // MISC_H
