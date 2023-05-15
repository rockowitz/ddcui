/** @file value_2button_widget.cpp
 *  Widget containing 2 named buttons
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_2button_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QPalette>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

#include <ddcutil_c_api.h>

#include "../base/ddcui_core.h"
#include "base/widget_debug.h"

static bool showDimensionReport = false;

void Value2ButtonWidget::layoutWidget()
{
   _featureValueWidgetHeight = 20;     // set it here for now

   uint8_t buttonHeight = _featureValueWidgetHeight;        // seems to work
   QFont nonMonoFont;
   nonMonoFont.setPointSize(9);

   _button1 = new QPushButton();
   _button1->setMaximumSize(60,buttonHeight);

   _button2 = new QPushButton();
   _button2->setMaximumSize(60,buttonHeight);

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _button1->setSizePolicy(*sizePolicy);
    _button1->setFont(FeatureValueButtonFont);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    // _button1->setStyleSheet("background-color:white;");
    _button2->setSizePolicy(*sizePolicy);
    _button2->setFont(FeatureValueButtonFont);
    // _button2->setStyleSheet("background-color:white;color:black;");
    _savedBackgroundColor = _button1->backgroundRole();

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addSpacing(5);
    layout->addWidget(_button1);
    layout->addWidget(_button2);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    delete sizePolicy;

    if (debugLayout) {
       this->setStyleSheet("background-color:cyan;");

       static bool dimensionReportShown = false;
       if (showDimensionReport && !dimensionReportShown) {
          printf("(%s::%s) ------------------------------------------>\n", _cls, __func__); fflush(stdout);
          reportWidgetDimensions(_button1, _cls, __func__, "_button1");
          reportWidgetDimensions(this, _cls, __func__, "Value2ButtonWidget");
          dimensionReportShown = true;
       }
    }
}


Value2ButtonWidget::Value2ButtonWidget(
     QWidget *parent)
    : ValueBaseWidget(parent)
    , _val1(0)      // set in setButtonDetail()
    , _val2(0)
{
   bool debug = false;
   _cls = strdup(metaObject()->className());
   TRACEMCF(debug, "Starting. _id=%d, _featureCode=0x%02x", _id, _featureCode );
   layoutWidget();

   QObject::connect(_button1, SIGNAL(released()),  this, SLOT(  on_button1_pressed()) );
   QObject::connect(_button2, SIGNAL(released()),  this, SLOT(  on_button2_pressed()) );

   TRACEMCF(debug, "Done");
}


Value2ButtonWidget::~Value2ButtonWidget() {
   bool debug = false;
   TRACEMCF(debug, "Executing. this._id = %d", _id);
   free((void*) _cls);
}


void Value2ButtonWidget::setButtonDetail(
      QString   name1,
      uint8_t   val1,
      QString   name2,
      uint8_t   val2)
{
   _val1  = val1;
   _val2  = val2;
   _button1->setText(name1);
   _button2->setText(name2);
}


void Value2ButtonWidget::setEnabled(bool onoff) {
   bool debug = false;
   TRACEMCF(debug, "Starting. onoff=%s", SBOOL(onoff));
   ValueBaseWidget::setEnabled(onoff);
#ifdef OUT
   ValueBaseWidget::setEnabled(onoff);
   if (onoff) {
      _button1->setBackgroundRole(_savedBackgroundColor);
      _button2->setBackgroundRole(_savedBackgroundColor);
   }
   else {
      _button1->setBackgroundRole(QPalette::Dark);
      _button2->setBackgroundRole(QPalette::Dark);
   }
#endif
   TRACEMCF(debug, "Done.");
}


void Value2ButtonWidget::on_button1_pressed() {
   bool debug = false;
//   if (_base_ctrl_key_is_pressed || !classControlKeyRequired) {
      TRACEMCF(debug || debugValueWidgetSignals, "Button pressed. Emitting featureValueChanged()");
      emit featureValueChanged(_featureCode, 0, _val1);
//   }
//   else
//      TRACEMCF(debug|| debugValueWidgetSignals, "Button pressed.  NOT emitting featureValueChanged()");
}


void Value2ButtonWidget::on_button2_pressed() {
   bool debug = false;
//   if (_base_ctrl_key_is_pressed || !classControlKeyRequired) {
      TRACEMCF(debug || debugValueWidgetSignals, "Button pressed. Emitting featureValueChanged()");
      emit featureValueChanged(_featureCode, 0, _val2);
//   }
//   else
//      TRACEMCF(debug|| debugValueWidgetSignals, "Button pressed.  NOT emitting featureValueChanged()");
}


