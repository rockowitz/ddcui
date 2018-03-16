#ifndef FEATURE_CHANGE_OBSERVER_H
#define FEATURE_CHANGE_OBSERVER_H

#include <inttypes.h>

class FeatureChangeObserver
{
public:
    // FeatureChangeObserver();
    // virtual ~FeatureChangeObserver();

    virtual void featureChanged(uint8_t feature_code) = 0;
};

#endif // FEATURE_CHANGE_OBSERVER_H
