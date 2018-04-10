/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

// #include <QtWidgets/QWidget>
// #include <QtWidgets/QSlider>
// #include <QtWidgets/QSpinBox>
// #include <QtWidgets/QLabel>

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_base_widget.h"

class QSlider;
class QSpinBox;
class QLabel;


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

private slots:

    void onSliderValueChanged(int value);

    void onSliderReleased();

    void onSpinBoxValueChanged(int value);

    void onSpinBoxEditingFinished();
};

#endif // VALUE_CONT_WIDGET_H
