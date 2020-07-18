/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_SIMPLE_CONT_WIDGET_H
#define VALUE_SIMPLE_CONT_WIDGET_H

#include "config.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_base_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;
class EnhancedSlider;

class ValueSimpleContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueSimpleContWidget(int minval = 1, int maxval = 0x0E, QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    uint16_t getCurrentShSl()                        override;
    void     setControlKeyRequired(bool onoff)       override;

protected:
    void resizeEvent(QResizeEvent * event)           override;

    virtual void layoutWidget(QHBoxLayout * layout);
    void createWidgets();

#ifdef UNUSED
public slots:
    void when_ckrChanged(bool onoff);
#endif

private slots:
    void onSliderReleased();
    // void onSliderValueChanged(int value);  // unused
     // void onSpinBoxEditingFinished();     // unused
    void onSpinBoxTimedOut();
    void onSpinBoxValueChanged(int value);

protected:   // member variables
    int _minval;
    int _maxval;
    EnhancedSlider * _curSlider;
    QSpinBox*        _curSpinBox;
    uint16_t         _newval;
    QTimer*          _spinBoxTimer;
    bool             _guiChange = false;


private:    // member variables
    static int idGenerator;
    int _id;
    const char *     _cls;

#ifdef FOR_SUBCLASS
    QLabel*          _maxTitle;
    QLabel*          _maxValue;
#endif

};

#endif // VALUE_SIMPLE_CONT_WIDGET_H

