/* value_2button_widget.cpp */

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

#include "feature_value_widgets/value_2button_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include <ddcutil_c_api.h>


static bool dimensionReportShown = false;

Value2ButtonWidget::Value2ButtonWidget(
#ifdef NO
      QString& name1,
      uint8_t val1,
      QString& name2,
      uint8_t val2,
#endif
      QWidget *parent)
:         ValueBaseWidget(parent)    // , _val1(val1), _val2(val2)
{
   _cls = strdup(metaObject()->className());
    // printf("(Value2ButtonWidget::Value2ButtonWidget) Starting.\n" ); fflush(stdout);

   QFont nonMonoFont;
   nonMonoFont.setPointSize(9);

   _button1 = new QPushButton();
   _button1->setMaximumSize(60,20);

   _button2 = new QPushButton();
   _button2->setMaximumSize(60,20);

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _button1->setSizePolicy(*sizePolicy);
    _button1->setFont(nonMonoFont);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    _button1->setStyleSheet("background-color:white;");
    _button2->setSizePolicy(*sizePolicy);
    _button2->setFont(nonMonoFont);
    _button2->setStyleSheet("background-color:white;");

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_button1);
    layout->addWidget(_button2);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    // setVisible(true);

    if (!dimensionReportShown  && debugLayout ) {
        printf("(%s::%s) ------------------------------------------>\n", _cls, __func__); fflush(stdout);
        reportWidgetDimensions(_button1, _cls, __func__, "_button1");
        reportWidgetDimensions(this, _cls, __func__, "Value2ButtonWidget");
        dimensionReportShown = true;
    }
    if (debugLayout)
       this->setStyleSheet("background-color:cyan;");

    // QObject::connect(_button1,  &QAbstractButton::released),
    //                  this,          &Value2ButtonWidget::on_button1_pressed );

    QObject::connect(_button1,  SIGNAL(released()),
                     this,      SLOT(  on_button1_pressed()) );
    QObject::connect(_button2,  SIGNAL(released()),
                     this,      SLOT(  on_button2_pressed()) );
}

void Value2ButtonWidget::setButtonDetail(
      QString   name1,
      uint8_t   val1,
      QString   name2,
      uint8_t   val2)
{
   _val1  = val1;
   _val1  = val2;
   _button1->setText(name1);
   _button2->setText(name2);
   // setVisible(true);
}



void Value2ButtonWidget::on_button1_pressed() {
   if (debugValueWidgetSignals)
   printf("(%s::%s)\n", _cls, __func__); fflush(stdout);
   emit featureValueChanged(_featureCode, 0, _val1);
}

void Value2ButtonWidget::on_button2_pressed() {
   if (debugValueWidgetSignals)
   printf("(%s::%s)\n", _cls, __func__); fflush(stdout);
   emit featureValueChanged(_featureCode, 0, _val2);
}


