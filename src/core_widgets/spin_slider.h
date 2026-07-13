/** @file spin_slider.h
 *  Combines a slider with a spinbox
 */

// Copyright (C) 2020-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SPIN_SLIDER_H_
#define SPIN_SLIDER_H_

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLayout>

class SpinSlider : public QWidget {
   Q_OBJECT

// *** Constructors and Methods

public:
   // allocation and initialization
   SpinSlider(QWidget * parent = nullptr);
   ~SpinSlider();
   void     setRange(int minval, int maxval);
   void     setFeatureCode(uint8_t featureCode);

   // For containing class
   void     setShSl(uint16_t newval) ;
   uint16_t getShSl();

signals:
    void    featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private slots:
    // for signaling valueChanged():
    void    onSliderReleased();
    void    onSpinBoxValueChanged(int value);
    void    onSpinBoxTimedOut();

private:
   void     createWidgets();
   QLayout* layoutWidget();

// *** Variables ***

private:
   const char *     _cls;
   QSlider *        _slider;
   QSpinBox *       _spinBox;
   QTimer *         _spinBoxTimer;
   uint8_t          _featureCode = 0;
   bool             _isFeatureCodeSet = true;  // for assert()
   uint16_t         _latestSpinBoxValue = 0;
   // enum          _valueChangeInitiator { ChangedByModel, ChangedByGUI };
};

void init_spin_slider();

#endif /* SPIN_SLIDER_H_ */
