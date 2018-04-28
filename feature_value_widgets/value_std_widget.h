/* value_std_widget.h - Widget for displaying a formatted VCP feature value */

#ifndef VALUE_STD_WIDGET_H
#define VALUE_STD_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

#include "feature_value_widgets/value_base_widget.h"


class ValueStdWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueStdWidget(QWidget *parent = nullptr);

    void setFeatureValue(const FeatureValue  &fv) override;
    void setCurrentValue(uint16_t newval) override;

#ifdef DEBUG_PAINT
    void paintEvent(QPaintEvent *event) override;
#endif

private:
    void setValueField();

    QLabel * _valueField;
};

#endif // VALUE_STD_WIDGET_H
