/** @file value_special_widget_x62.cpp
 *  Adjust audio volume
 */

// Copyright (C) 2020-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>

#include "base/ddcui_parms.h"
#include "value_simple_cont_widget.h"
#include "value_special_widget_x62.h"

static bool debugWidget = true;

void ValueSpecialWidgetX62::createInitialWidgets() {
   bool debug = false;
   TRACECF(debug, "Starting.  ValueSpecialWidgetX62::createInitialWidgets()");
   _cb = createFormattedComboBox();

   if (debugLayout)
      _cb->setStyleSheet("background-color:cyan;");

   TRACECF(debug, "Done.  ValueSpecialWidgetX62::createInitialWidgets()");
}


void ValueSpecialWidgetX62::layoutWidget(QHBoxLayout * layout) {
   bool debug = false;
   TRACECF(debug, "Starting. ValueSpecialWidgetX62::layoutWidget" );

   layout->addSpacing(5);
   _cb->setVisible(true);
   layout->addWidget(_cb);

   layout->addWidget(_spinSlider);
   layout->addSpacing(5+10);
   layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
   layout->setContentsMargins(0,0,0,0);
   setLayout(layout);
   TRACECF(debug, "Done. ValueSpecialWidgetX62::layoutWidget" );
}


ValueSpecialWidgetX62::ValueSpecialWidgetX62(QWidget *parent)
    : ValueSimpleContWidget(parent)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACEMCF(debug, "Starting.  _cls=%s, _id=%d, _featureCode=0x%02x", _cls, _id, _featureCode );
    setRange(0x01, 0xfe);

    createInitialWidgets();
    delete layout();   // delete layout created by superclass constructor
    QHBoxLayout * layout = new QHBoxLayout();
    layoutWidget(layout);

    loadComboBox();

    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );

    TRACEMCF(debug, "Done.");
}

ValueSpecialWidgetX62::~ValueSpecialWidgetX62() {
   bool debug = false;
   TRACEMCF(debug, "Executing. this._id = %d", _id);
   free((void*) _cls);
}


void ValueSpecialWidgetX62::setFeatureValue(const FeatureValue &fv) {
   bool debug = false;

   TRACEMCF(debug, "ValueSpecialWidgetX62. featureCode=0x%02x, capVcp=%p, sh=0x%02x, sl=0x%02x, ddcrc=%d",
                 fv.featureCode(), fv.capVcp(), fv.ddcrc());
   ValueSimpleContWidget::setFeatureValue(fv);

   TRACEMCF(debug, "Done");
}


void ValueSpecialWidgetX62::loadComboBox() {

   _cb->addItem(QString("0: Mute"),             QVariant(  0) );
   _cb->addItem(QString("1-254: Level"),        QVariant(  1) );
   _cb->addItem(QString("255 - Default Level"), QVariant(255) );

   _cb->setCurrentIndex(2);
}


void ValueSpecialWidgetX62::setCurrentShSl(uint16_t newval) {
   bool debugFunc = false;
   debugFunc = debugFunc || debugWidget;
   TRACEMF(debugFunc, "Starting. feature 0x%02x, newval=x%04x", _featureCode, newval);

    ValueBaseWidget::setCurrentShSl(newval);

    newval = newval & 0xff;

    if (newval == 0) {
       TRACECF(debugFunc, "newval = 0, oldCbIndex != 0");
       _cb->setCurrentIndex(0);
       _spinSlider->setEnabled(false);
    }
    else if (newval == 255) {
       TRACECF(debugFunc,"newval = 255, oldCbIndex != 2");
       _cb->setCurrentIndex(2);
       _spinSlider->setEnabled(false);
    }
    else {   // newval = 1..254
       TRACECF(debugFunc, "newval = %d, oldCbIndex != 1", newval);
       _cb->setCurrentIndex(1);
       _spinSlider->setShSl(newval);
       _spinSlider->setEnabled(true);
   }
}


uint16_t ValueSpecialWidgetX62::getCurrentShSl() {
    // get sl from combobox
    if (_cb->currentIndex() == 0)
       _sl = 0;
    else if (_cb->currentIndex() == 2)
       _sl = 255;
    else
       _sl = _spinSlider->getShSl();

    _sh = 0;

    uint16_t result = (_sh << 8) | _sl;
    return result;
}


void ValueSpecialWidgetX62::combobox_activated(int index) {
   bool debug = false;
   debug = debug || debugWidget;
   TRACEMCF(debug, "feature 0x%02x, index=%d", _featureCode, index);
   assert(_cb->currentIndex() == index);
   assert(_sh == 0);

   uint8_t new_sh = 0;
   uint8_t new_sl = _spinSlider->getShSl();

   if (index == 0)
      new_sl = 0;
   else if (index == 2)
      new_sl = 255;

   if (new_sh != _sh || new_sl != _sl) {
      TRACEMCF(debug, "Value changed.  New sl: %u", new_sl);
      TRACEMCF(debug, "Emitting featureValueChanged, featureCode = 0x%02x, sh=0, new_sl=0x%02x",
                         _featureCode, new_sl);
         emit featureValueChanged(_featureCode, 0, new_sl);
   // _sh = 0;  // unnecessary because of assert()
      _sl = new_sl;
   }
   else {
      TRACEMCF(debug, "Value not changed.");
   }
}
//ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddccccc
