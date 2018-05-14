/* value_2button_widget.h - Widget containing Reset button */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

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
