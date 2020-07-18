/* value_new_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_NEW_CONT_WIDGET_H
#define VALUE_NEW_CONT_WIDGET_H

#include "config.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_simple_cont_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;
class EnhancedSlider;

class ValueNewContWidget : public ValueSimpleContWidget
{
    Q_OBJECT

public:
    ValueNewContWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    // void     setCurrentShSl(uint16_t newval)         override;
    // uint16_t getCurrentShSl()                        override;
    // void     setControlKeyRequired(bool onoff)       override;

protected:
    // void resizeEvent(QResizeEvent * event)           override;
    // void subLayout();

// private:
    void layoutWidget(QHBoxLayout * layout) ;
    void additionalWidgets();


private slots:
//    void onSliderReleased();
//    void onSpinBoxTimedOut();
//    void onSpinBoxValueChanged(int value);



private:    // member variables
    const char *     _cls;
     QLabel*          _maxTitle;
     QLabel*          _maxValue;
};

#endif // VALUE_NEW_CONT_WIDGET_H

