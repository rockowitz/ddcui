/* value_abstract_widget.h - Abstract Base Class for Value..Widgets */

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
