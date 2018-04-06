/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

#include <QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>

#include "feature_value_widgets/value_base_widget.h"


class ValueContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueContWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    QSlider*    _curSlider;
    QSpinBox*   _curSpinBox;
    QLabel*     _maxTitle;
    QLabel*     _maxValue;
};

#endif // VALUE_CONT_WIDGET_H
