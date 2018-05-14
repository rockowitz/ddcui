/* value_std_widget.h - Widget for displaying a formatted VCP feature value */

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
