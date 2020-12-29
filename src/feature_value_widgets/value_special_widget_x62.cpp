// value_special_widget_x62.cpp

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "base/ddcui_parms.h"
#include "core_widgets/enhanced_slider.h"
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
    // layout->addWidget(spacer);
    layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
    // layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    TRACECF(debug, "Done. ValueSpecialWidgetX62::layoutWidget" );
}


ValueSpecialWidgetX62::ValueSpecialWidgetX62(QWidget *parent)
    : ValueSimpleContWidget(parent)
//    , _newval(0)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACECF(debug, "Starting.  ValueSpecialWidgetX62, _cls=%s", _cls );
    setRange(0x01, 0xfe);
    // _spinSlider->setFeatureCode(0x62);   // needed, set by setFeatureValue()

    createInitialWidgets();
    delete layout();   // delete layout created by superclass constructor
    QHBoxLayout * layout = new QHBoxLayout();
    layoutWidget(layout);

    loadComboBox();

    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );

    TRACECF(debug, "Done. ValueSpecialWidgetX62");
}


void ValueSpecialWidgetX62::setFeatureValue(const FeatureValue &fv) {
   bool debug = false;

   TRACEMCF(debug, "ValueSpecialWidgetX62. featureCode=0x%02x, capVcp=%p, sh=0x%02x, sl=0x%02x, ddcrc=%d",
                 fv.featureCode(), fv.capVcp(), fv.ddcrc());

   // _guiChange = false;   // changes not initiated by GUI, do not signal model change

    // TRACEMCF(debugWidget, "Starting. feature 0x%02x, new sl=x%02x, Before ValueBaseWidget::setFeatureValue()",
    //           fv.featureCode(), fv.val().sl);
    ValueSimpleContWidget::setFeatureValue(fv);

    // setCurrentShSl(fv.val().sl);

    // uint16_t initialValue = _sh << 8 | _sl;
    // // _guiChange=true;

    TRACEMCF(debug, "Done");
}


void ValueSpecialWidgetX62::loadComboBox() {

   _cb->addItem(QString("0: Mute"),             QVariant(  0) );
   _cb->addItem(QString("1-254: Level"),        QVariant(  1) );
   _cb->addItem(QString("255 - Default Level"), QVariant(255) );

   _cb->setCurrentIndex(2);
}


void ValueSpecialWidgetX62::setCurrentShSl(uint16_t newval) {
   bool debugFunc = true;
   debugFunc = debugFunc || debugWidget;
   TRACEMF(debugFunc, "Starting. feature 0x%02x, newval=x%04x", _featureCode, newval);

   // _guiChange = false;

    ValueBaseWidget::setCurrentShSl(newval);

    newval = newval & 0xff;
    // int oldCbIndex = _cb->currentIndex();
    // int oldSlider  = _spinSlider->getShSl();

    if (newval == 0) {
     //  if (oldCbIndex != 0) {
          TRACECF(debugFunc, "newval = 0, oldCbIndex != 0");
          // // _guiChange = true;
          _cb->setCurrentIndex(0);
          _spinSlider->setEnabled(false);
      // }
    }
    else if (newval == 255) {
   //    if (oldCbIndex != 2) {
          TRACECF(debugFunc,"newval = 255, oldCbIndex != 2");
          // // _guiChange = true;
          _cb->setCurrentIndex(2);
          _spinSlider->setEnabled(false);
   //    }
    }
    else {   // newval = 1.254
    //   if (oldCbIndex != 1) {
          TRACECF(debugFunc, "newval = %d, oldCbIndex != 1", newval);
          // // _guiChange = true;
          _cb->setCurrentIndex(1);
          _spinSlider->setShSl(newval);
          _spinSlider->setEnabled(true);

    }

#ifdef OUT

       else {
          TRACEC("newval = %d, oldCbIndex == 1", newval);
          if (_curSlider->value() != newval) {
             // // _guiChange = true;
             _curSlider->setValue(newval);
          }
       }
}
#endif

    // _guiChange = true;
}


uint16_t ValueSpecialWidgetX62::getCurrentShSl() {
    // get sl from combobox

    // int ndx = _cb->currentIndex();
    // QVariant qv = _cb->itemData(ndx);
    // uint i = qv.toUInt();

    if (_cb->currentIndex() == 0)
       _sl = 0;
    else if (_cb->currentIndex() == 2)
       _sl = 255;
    else
       _sl = _spinSlider->getShSl();

    _sh = 0;
    // _sl = i & 0xff;

    uint16_t result = (_sh << 8) | _sl;
    return result;
}


void ValueSpecialWidgetX62::combobox_activated(int index) {
   bool debug = false;
   debug = debug || debugWidget;
   TRACEMCF(debug, "feature 0x%02x, index=%d", _featureCode, index);
   int ndx = _cb->currentIndex();
   assert(ndx == index);

   // QVariant qv = _cb->itemData(ndx);
   // uint i = qv.toUInt();
   // uint8_t new_sh = 0;
   // uint8_t new_sl = i & 0xff;

   uint8_t new_sh = 0;

   assert(_sh == 0);

   uint8_t new_sl = _spinSlider->getShSl();

   if (index == 0)
      new_sl = 0;
   else if (index == 2)
      new_sl = 255;

   if (new_sh != _sh || new_sl != _sl) {
      TRACEMCF(debug, "Value changed.  New sl: %u", new_sl);
      // TRACEMCF(debug, "Value changed.  New sl: %u, _guiChange=%d", new_sl, _guiChange);
      // if (_guiChange) {
         TRACEMCF(debug, "Emitting featureValueChanged, featureCode = 0x%02x, sh=0, new_sl=0x%02x",
                         _featureCode, new_sl);
         emit featureValueChanged(_featureCode, 0, new_sl);
      // }
      _sh = 0;
      _sl = new_sl;
   }
   else {
      TRACEMCF(debug, "Value not changed.");
   }
}

// void ValueSpecialWidgetX62::forSpinSliderChanged(uint16_t newval) {
// }

