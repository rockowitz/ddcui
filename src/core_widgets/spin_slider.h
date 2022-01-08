// slider_spinner.h

// Copyright (C) 2020-2022 Sanford Rockowitz <rockowitz@minsoft.com>
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
   ~SpinSlider();
   void     setRange(int minval, int maxval);
   void     setFeatureCode(uint8_t featureCode);

   // For containing class
   void     setShSl(uint16_t newval) ;
   uint16_t getShSl();
   void     setEnabled(bool enabled);

signals:
    // compiler warning: signals may not be declared virtual
    void    featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private slots:
    // for signaling valueChanged():
    void    onSliderReleased();
    void    onSpinBoxValueChanged(int value);
    void    onSpinBoxTimedOut();

private:        // methods
   void     createWidgets();
   QLayout* layoutWidget();

private:        // member variables
   const char *     _cls;
   EnhancedSlider * _slider;
   QSpinBox *       _spinBox;
   QTimer *         _spinBoxTimer;
   uint8_t          _featureCode;
   bool             _isFeatureCodeSet = true;  // for assert()
   uint16_t         _latestSpinBoxValue;
   // enum          _valueChangeInitiator { ChangedByModel, ChangedByGUI };
};

#endif /* SLIDER_SPINNER_H_ */
