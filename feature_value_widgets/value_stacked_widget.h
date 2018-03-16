#ifndef VALUE_STACKED_WIDGET_H
#define VALUE_STACKED_WIDGET_H

#include "value_abstract_widget.h"

#include <QWidget>
#include <QtWidgets/QStackedWidget>

class ValueStackedWidget : public ValueAbstractWidget, QStackedWidget
{
public:
    explicit ValueStackedWidget(QWidget *parent = nullptr);

    virtual void setFeatureValue(const FeatureValue &fv);

    virtual void setCurrentValue(uint16_t newval);

    virtual uint16_t getCurrentValue();

    QSize sizeHint() const override;

    QStackedWidget *stacked = new QStackedWidget();

};

#endif // VALUE_STACKED_WIDGET_H
