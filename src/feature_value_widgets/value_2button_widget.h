/** @file value_2button_widget.h
 *  Widget containing 2 named buttons
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_2BUTTON_WIDGET_H
#define VALUE_2BUTTON_WIDGET_H

#include <QtWidgets/QWidget>

#include "value_base_widget.h"

class QPushButton;
class QPalette;

class Value2ButtonWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    Value2ButtonWidget(
          QWidget *parent = nullptr);

    ~Value2ButtonWidget();

    void setButtonDetail(
          QString  name1,
          uint8_t  val1,
          QString  name2,
          uint8_t  val2);

public slots:
    void setEnabled(bool onoff) override;

private slots:
    void on_button1_pressed();
    void on_button2_pressed();

private:     // methods
    void layoutWidget();

private:     // member variables
    const char *        _cls;
    QPushButton *       _button1;
    QPushButton *       _button2;
    uint8_t             _val1;
    uint8_t             _val2;
    QPalette::ColorRole _savedBackgroundColor;
};

#endif // VALUE_2BUTTON_WIDGET_H
