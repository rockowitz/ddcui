/* value_std_widget.cpp */

#include "feature_value_widgets/value_std_widget.h"

#include "ddcutil_c_api.h"



ValueStdWidget::ValueStdWidget(QWidget *parent):
        ValueBaseWidget(parent)
{

    _valueField = new QLabel();

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _valueField->setSizePolicy(*sizePolicy);
    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_valueField);
    setLayout(layout);
}

void ValueStdWidget::setValueField() {
    char * s_formatted = NULL;
    DDCA_Status rc = ddca_format_non_table_vcp_value(_feature_code, _vspec, &_value, &s_formatted);
    if (rc != 0)
        s_formatted = (char*) "invalid formatted value";   // explicit cast to avoid compiler warning

    _valueField->setText(QString::fromUtf8(s_formatted));
}

void ValueStdWidget::setFeatureValue(const FeatureValue &fv) {
    ValueBaseWidget::setFeatureValue(fv);

    setValueField();
}

void ValueStdWidget::setCurrentValue(uint16_t newval) {
    ValueBaseWidget::setCurrentValue(newval);

    setValueField();
}
