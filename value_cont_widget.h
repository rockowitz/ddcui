#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

#include <QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>

#include "value_base_widget.h"


class ValueContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueContWidget(QWidget *parent = nullptr);

    void setFeatureValue(const FeatureValue &fv);    // virtual
    void setCurrentValue(uint16_t newval); // virtual

    uint16_t getCurrentValue() override;

    QSlider*    _curSlider;
    QSpinBox*   _curSpinBox;
    QLabel*     _maxTitle;
    QLabel*     _maxValue;
};

#endif // VALUE_CONT_WIDGET_H
