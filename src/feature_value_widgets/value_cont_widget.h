/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

#include "config.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_base_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;
class EnhancedSlider;


class ValueContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueContWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

private:
    // QSlider*    _curSlider;
    EnhancedSlider * _curSlider;
    QSpinBox*   _curSpinBox;
    QLabel*     _maxTitle;
    QLabel*     _maxValue;
    uint16_t    _newval;
    QTimer*     _spinBoxTimer;
    bool        _guiChange = false;

private slots:
    void onSliderReleased();
    // void onSliderValueChanged(int value);  // unused
    // void onSpinBoxEditingFinished();     // unused
    void onSpinBoxTimedOut();
    void onSpinBoxValueChanged(int value);

#ifdef APPLY_CANCEL
private:
    void onApplyButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);

    QPushButton * _applyButton;
    QPushButton * _cancelButton;
#endif

};

#endif // VALUE_CONT_WIDGET_H

