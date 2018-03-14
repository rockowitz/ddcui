#ifndef VALUE_STD_WIDGET_H
#define VALUE_STD_WIDGET_H

#include <QWidget>
#include <QtWidgets>

#include "value_base_widget.h"

class ValueStdWidget : public ValueBaseWidget
{

        Q_OBJECT

public:
    ValueStdWidget(QWidget *parent = nullptr);

    void setFeatureValue(const FeatureValue  &fv);    // virtual
    void setCurrentValue(uint16_t newval) override;

private:
    void setValueField();

    QLabel * _valueField;


};

#endif // VALUE_STD_WIDGET_H
