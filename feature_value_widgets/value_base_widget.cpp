/* value_base_widget.cpp */

#include "feature_value_widgets/value_base_widget.h"

ValueBaseWidget::ValueBaseWidget(QWidget *parent)
    // : ValueAbstractWidget(parent)
{
   // will this get subclass name or "ValueBaseWidget"?  ValueBaseWidget
   _cls                    = strdup(metaObject()->className());
}


void ValueBaseWidget::setFeatureValue(const FeatureValue &fv) {
    _feature_code    = fv._feature_code;
    _vspec           = fv._vspec;
    _feature_flags   = fv._feature_flags;
    _mh              = fv._value.mh;
    _ml              = fv._value.ml;
    _sh              = fv._value.sh;
    _sl              = fv._value.sl;
    _value           = fv._value;
}


void ValueBaseWidget::setCurrentValue(uint16_t newval) {
    _sh = newval >> 8;
    _sl = newval & 0xff;
    _value.sh = _sh;
    _value.sl = _sl;
}


uint16_t ValueBaseWidget::getCurrentValue() {
    uint16_t result = (_sh << 8) | _sl;
    return result;
}

#ifdef NO
QSize ValueBaseWidget::sizeHint() const {
    int w = 300;
    int h = 20;
    printf("============> (ValueBaseWidget::sizeHint) returning (%d,%d)\n", w,h ); fflush(stdout);
    return QSize(w,h);
}
#endif
