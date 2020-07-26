// slider_spinner.cpp

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

// static bool debugLayout;
 
#include "base/core.h"
#include "core_widgets/spin_slider.h"


void SpinSlider::createWidgets() {
   _slider = new EnhancedSlider(Qt::Horizontal);
    _slider->setFocusPolicy(Qt::StrongFocus);
    _slider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _slider->setSingleStep(1);
   // _slider->setFixedSize(200,18);  // was 18 , 14 stuffs slider to bottom
    _slider->setMinimumSize(200,10);

    _slider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    // Trying to force a bit of space above and below slider
    // _slider->setContentsMargins(3,0,1,0);  // no effect
    // increases height of rows in at least style adwaita
    //  _slider->setStyleSheet("padding-top:2px;padding-bottom:2px");
    if (debugLayout)
       _slider->setStyleSheet("background-color:pink;");

    _spinBox = new QSpinBox();
    _spinBox->setSingleStep(1);
   // _spinBox->setFixedSize(100,18);   // extra large for 2 byte values, possible horizontal up/down buttons
    _spinBox->setMinimumSize(100,18);   // becomes 52,32  - how?
    _spinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _spinBox->setFont(FeatureValueNumberEntryFont);

    _spinBox->setAlignment(Qt::AlignRight);
    if (debugLayout)
       _spinBox->setStyleSheet("background-color:green;");
}


QLayout* SpinSlider::layoutWidget() {
   QLayout* layout = new QHBoxLayout();

   layout->setSpacing(5);
   // layout->addWidget(sliderContainer);
   layout->addWidget(_slider);
   // crops off tick marks at bottom of slider:
   // how to shrink the slider size?
   // layout->setAlignment(_curSlider, Qt::AlignVCenter);

   layout->addWidget(_spinBox);

   layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
   setLayout(layout);

   return layout;
}


SpinSlider::SpinSlider(QWidget * parent)
        : QWidget(parent)
        , _cls(metaObject()->className())
{
   createWidgets();
   layoutWidget();

   connect(_slider,  SIGNAL(sliderReleased()),
           this,     SLOT(  onSliderReleased()));
   connect(_spinBox, SIGNAL(valueChanged(int)),
           this,     SLOT(  onSpinBoxValueChanged(int)));

   // Tie the slider and spinbox together
   connect(_slider,  SIGNAL(valueChanged(int)),
           _spinBox, SLOT(  setValue(int)));
   connect(_spinBox, SIGNAL(valueChanged(int)),
           _slider,  SLOT(  setValue(int)));

    _spinBoxTimer = new QTimer();
    _spinBoxTimer->setSingleShot(true);
    _spinBoxTimer->setInterval(1000);

    connect(_spinBoxTimer,   SIGNAL(timeout()),
            this,            SLOT(onSpinBoxTimedOut()));
}


void SpinSlider::setFeatureCode(uint8_t featureCode) {
   _featureCode = featureCode;
   _isFeatureCodeSet = true;
}


void SpinSlider::setRange(int minval, int maxval) {
    _slider->setTickInterval((maxval-minval)/10);
    _slider->setRange(minval, maxval);
    _spinBox->setRange(minval, maxval);
}


void  SpinSlider::setEnabled(bool enabled) {
   _slider->setEnabled(enabled);
   _spinBox->setEnabled(enabled);
}


// Called by the containing class to update the widget
void SpinSlider::setShSl(uint16_t newval) {
    bool debug = false;
    // TRACEMCF(debug, "newval = 0x%04x", newval);
    // ValueBaseWidget::setCurrentShSl(newval);
    // _guiChange = false;

    uint8_t sh = newval >> 8;
    uint8_t sl = newval & 0xff;
    int curval = sh << 8 | sl;
    TRACEMCF(debug, "Starting. feature=0x%02x, newval=%d, curval=%d", _featureCode , curval);

    TRACEMF(debug, "Calling _spinBoxTime->stop(), _spinBox->setValue()");
    // in case the timer is running, don't trigger
    _spinBoxTimer->stop();
    _spinBox->setValue(curval);
    // _slider->setValue(curval);

    // _guiChange = true;
    TRACECF(debug, "Done");
}


// Used by the containing class to query the widget's value after having been
// notified that that widget value has changed.
uint16_t SpinSlider::getShSl() {
    int curval = _spinBox->value();
    uint8_t sh = (curval >> 8) & 0xff;
    uint8_t sl = curval & 0xff;
    uint16_t result = (sh << 8) | sl;
    return result;
}


void SpinSlider::onSliderReleased() {
   bool debug = false;
   debug = debug || debugValueWidgetSignals;
   // TRACECF(debug, "feature=0x%02x",  _featureCode);

   int newval = _spinBox->value();

   uint8_t new_sh = (newval >> 8) & 0xff;
   uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //     printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);

   // if (_guiChange) {
      TRACECF(debug, "emitting featureValueChanged(), _feature_code=0x%02x, new_sh=0x%02x, new_sl=0x%02x",
                     _featureCode, new_sh, new_sl);
      emit featureValueChanged(_featureCode, new_sh, new_sl);
   // }
}


void SpinSlider::onSpinBoxValueChanged(int value) {
   bool debug = false;
   debug = debug || debugValueWidgetSignals;
   // TRACECF(debug, "feature=0x%02x, value=%d, _guiChange=%d=%s",
   //                _featureCode, value, _guiChange, SBOOL(_guiChange));
   TRACECF(debug, "feature=0x%02x, value=%d", _featureCode, value);

   int newval = _spinBox->value();

   // Do not immediately signal valueChanged() - this generates too many API calls.
   // instead only emit valueChanged() after multiple changes occur or the final change is made
   // uint8_t new_sh = (newval >> 8) & 0xff;
   // uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //    printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);
   _latestSpinBoxValue = newval & 0xffff;

   // if (_guiChange || true) {   // *** TEMP ***
      TRACECF(debug, "Starting spinbox timer");
      _spinBoxTimer->start();
   // }
   // else {
   //    TRACECF(debug,"Not starting spinbox timer");
   // }
}


void SpinSlider::onSpinBoxTimedOut() {
   bool debug = false;
   debug = debug || debugValueWidgetSignals;

   uint8_t new_sh = (_latestSpinBoxValue >> 8) & 0xff;
   uint8_t new_sl = _latestSpinBoxValue & 0xff;

   TRACECF(debug, "feature 0x%02x, _latestSpinBoxValue=%d, new_sh=0x%02x, new_sl=0x%02x, "
                  "emitting featureValueChanged()",
                  _featureCode, _latestSpinBoxValue, new_sh, new_sl);
   emit featureValueChanged(_featureCode, new_sh, new_sl);
}

#ifdef OLD
// TODO: Set at class level, not for individual instances
void SpinSlider::setControlKeyRequired(bool onoff) {
   // TRACE("onoff=%s", sbool(onoff));
   _slider->oldSetControlKeyRequired(onoff);
}
#endif

