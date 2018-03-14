#include "feature_change_observer.h"

#include <string.h>
#include <stdio.h>

FeatureChangeObserver::FeatureChangeObserver() {}

FeatureChangeObserver::~FeatureChangeObserver() {}

void FeatureChangeObserver::featureChanged(uint8_t feature_code) {
    printf("(FeatureChangeObserver::%s) feature_code: 0x%02x\n", __func__, feature_code ); fflush(stdout);

}
