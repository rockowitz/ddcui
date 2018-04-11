#ifndef SIMPLE_FEATURE_VALUE_OBSERVER_H
#define SIMPLE_FEATURE_VALUE_OBSERVER_H

#include "simple_feature_value.h"

class SimpleFeatureValueObserver
{
public:

    virtual void simpleFeatureValueChanged(SimpleFeatureValue fv) = 0;
};

#endif // SIMPLE_FEATURE_VALUE_OBSERVER_H
