/* value_reset_widget.h - Widget containing Reset button */

#ifndef VALUE_2BUTTON_WIDGET_H
#define VALUE_2BUTTON_WIDGET_H

#include <QtWidgets/QPushButton>

#include "value_base_widget.h"

class QPushButton;

class Value2ButtonWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    Value2ButtonWidget(
#ifdef NO
          QString& name1,
          uint8_t  val1,
          QString& name2,
          uint8_t  val1,
#endif
          QWidget *parent = nullptr);

    void setButtonDetail(
          QString  name1,
          uint8_t   val1,
          QString  name2,
          uint8_t   val2);

private slots:
    void on_button1_pressed();
    void on_button2_pressed();

private:
    QPushButton * _button1;
    QPushButton * _button2;

    uint8_t   _val1;
    uint8_t   _val2;
};

#endif // VALUE_2BUTTON_WIDGET_H
