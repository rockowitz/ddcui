/* value_2button_widget.h - Widget containing 2 named buttons */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

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
          QWidget *parent = nullptr);

    void setButtonDetail(
          QString  name1,
          uint8_t  val1,
          QString  name2,
          uint8_t  val2);

private slots:
    void on_button1_pressed();
    void on_button2_pressed();

private:     // methods
    void layoutWidget();

private:     // member variables
    const char *   _cls;
    QPushButton *  _button1;
    QPushButton *  _button2;
    uint8_t        _val1;
    uint8_t        _val2;
};

#endif // VALUE_2BUTTON_WIDGET_H
