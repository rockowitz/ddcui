#include "simple_feature_value.h"

SimpleFeatureValue::SimpleFeatureValue(
        uint8_t  feature_code,
        uint8_t  hiByte,
        uint8_t  loByte)
   : featureCode(feature_code), hiByte(hiByte), loByte(loByte)

{
   // this->featureCode = feature_code;
   //  this->hiByte = hiByte;
   //  this->loByte = loByte;
}
