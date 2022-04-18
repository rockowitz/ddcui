/** value_simple_cont_widget.h
  *
  * Display/edit a Continuous VCP feature value with specified minimum and maximum values
  */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_SIMPLE_CONT_WIDGET_H
#define VALUE_SIMPLE_CONT_WIDGET_H

#include "config.h"
#include "base/ddcui_parms.h"

#include "nongui/feature_value.h"
#include "core_widgets/spin_slider.h"
#include "feature_value_widgets/value_base_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;
class EnhancedSlider;
class SpinSlider;

class ValueSimpleContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueSimpleContWidget(QWidget *parent = nullptr);
    ~ValueSimpleContWidget();

    void     setRange(int minval, int maxval);
    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    uint16_t getCurrentShSl()                        override;

protected:   // methods
    void resizeEvent(QResizeEvent * event)           override;
    virtual void layoutWidget(QHBoxLayout * layout);
    void createWidgets();
    void additionalWidgets();

private slots:
    void onFeatureValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl);
    void setInstanceControlKeyRequired(bool onoff) override;

private:     // methods
    void debugSimpleContLayout();

protected:   // member variables
    int           _minval = -1;
    int           _maxval = -1;
    SpinSlider *  _spinSlider;
    uint16_t      _newval;
    // bool       _guiChange = false;
    QLabel*       _maxTitle;
    QLabel*       _maxValue;
    // int           _id;

private:     // member variables
    // static int    idGenerator;
    const char *  _cls;
};

#endif // VALUE_SIMPLE_CONT_WIDGET_H

