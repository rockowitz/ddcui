// slider_spinner.h

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SLIDER_SPINNER_H_
#define SLIDER_SPINNER_H_

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLayout>

#include "nongui/feature_value.h"

#include "enhanced_slider.h"

class SpinSlider : public QWidget {
   Q_OBJECT

public:
   // allocation and initialization
   SpinSlider(QWidget * parent = nullptr);
   void  setRange(int minval, int maxval);
   void  setFeatureCode(uint8_t featureCode);

   // For containing class
   void     setShSl(uint16_t newval) ;
   uint16_t getShSl();
   void     setControlKeyRequired(bool onoff)   ;
   void     setEnabled(bool enabled);

   // For slider <-> spinbox communication
   void     setValue(int val);
   int      value();

signals:
    // compiler warning: signals may not be declared virtual
    void featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private slots:
    // for spinbox<->slider synchronization
    void onSliderReleased();
    // void onSliderValueChanged(int value);  // unused
     // void onSpinBoxEditingFinished();     // unused
    void onSpinBoxTimedOut();
    void onSpinBoxValueChanged(int value);

private:      // methods
   void       createWidgets();
   QLayout *  layoutWidget();

private:      // member variables
   const char *     _cls;
   EnhancedSlider * _slider;
   // bool          _ctrl_key_required;
   QSpinBox *       _spinBox;
   QTimer *         _spinBoxTimer;
   uint8_t          _featureCode;
   bool             _featureCodeSet = true;  // for assert()
   bool             _guiChange = true;
   uint16_t         _latestSpinBoxValue;
};

#endif /* SLIDER_SPINNER_H_ */
