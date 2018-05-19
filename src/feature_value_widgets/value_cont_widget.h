/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

#include "config.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_base_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;


class ValueContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueContWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;


protected:
#ifdef UNUSED
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void leaveEvent(QEvent * event);
#endif


private:
    QSlider*    _curSlider;
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

