#ifndef SIMPLE_FEATURE_VALUE_H
#define SIMPLE_FEATURE_VALUE_H

#include <QtCore/QObject>

class SimpleFeatureValue
{
public:
    SimpleFeatureValue(
            uint8_t  feature_code,
            uint8_t  hiByte,
            uint8_t  loByte);

    const uint8_t   featureCode;
    const uint8_t   hiByte;
    const uint8_t   loByte;
};

#endif // SIMPLE_FEATURE_VALUE_H
