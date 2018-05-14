/* value_reset_widget.h - Widget containing Reset button */

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

#ifndef VALUE_RESET_WIDGET_H
#define VALUE_RESET_WIDGET_H

#include <QtWidgets/QPushButton>

#include "value_base_widget.h"

class QPushButton;

class ValueResetWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueResetWidget(QWidget *parent = nullptr);

private slots:
    void on_resetButton_pressed();

private:
    QPushButton * _resetButton;
};

#endif // VALUE_RESET_WIDGET_H
