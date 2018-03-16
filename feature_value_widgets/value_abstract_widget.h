/* value_abstract_widget.h - Abstract Base Class for Value..Widgets */

#ifndef VALUE_ABSTRACT_WIDGET_H
#define VALUE_ABSTRACT_WIDGET_H

#include <QWidget>

#include "nongui/feature_value.h"

class ValueAbstractWidget : public QWidget
{
    Q_OBJECT

public:
    // explicit ValueAbstractWidget(QWidget *parent = nullptr);   // should constructor exist for pure virtual class

    virtual void setFeatureValue(const FeatureValue &fv) = 0;

    virtual void setCurrentValue(uint16_t curval) = 0;

    virtual uint16_t getCurrentValue() = 0;

   //  QSize sizeHint() const override = 0; needed?

signals:

public slots:

};

#endif // VALUE_ABSTRACT_WIDGET_H
