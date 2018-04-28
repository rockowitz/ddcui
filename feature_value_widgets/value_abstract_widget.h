/* value_abstract_widget.h - Abstract Base Class for Value..Widgets */

#ifndef VALUE_ABSTRACT_WIDGET_H
#define VALUE_ABSTRACT_WIDGET_H

// #include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>

#include "nongui/feature_value.h"

class ValueAbstractWidget :
      public QFrame
{
    Q_OBJECT

protected:
    explicit ValueAbstractWidget(QWidget *parent = nullptr);   // should constructor exist for pure virtual class

public:
    virtual void setFeatureValue(const FeatureValue &fv) = 0;

    virtual void setCurrentValue(uint16_t curval) = 0;

    virtual uint16_t getCurrentValue() = 0;

   //  QSize sizeHint() const override = 0; needed?

signals:

public slots:

};

#endif // VALUE_ABSTRACT_WIDGET_H
