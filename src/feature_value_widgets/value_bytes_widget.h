// value_bytes_widget.h

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_BYTES_WIDGET_H_
#define VALUE_BYTES_WIDGET_H_


#include "config.h"

#include <QtWidgets/QLineEdit>

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_base_widget.h"
#include "base/number_entry_widget.h"

class QLabel;
class QPushButton;
// class QLineEdit;


class ValueBytesWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueBytesWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval)        override;
    uint16_t getCurrentValue()                       override;


private:
    QLabel*     _mhTitle;
    NumberEntryWidget *     _mhValue;
    QLabel*     _mlTitle;
    NumberEntryWidget *    _mlValue;
    QLabel*     _shTitle;
    NumberEntryWidget *   _shValue;
    QLabel*     _slTitle;
    NumberEntryWidget *   _slValue;
    uint16_t    _newval;
    bool        _guiChange = false;
    QFont nonMonoFont9;

// #ifdef APPLY_CANCEL
//    QPushButton *  _applyButton;
//    QPushButton *  _cancelButton;
// #endif

// #ifdef APPLY_CANCEL
private:
    void onApplyButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);

    QPushButton * _applyButton;
    QPushButton * _cancelButton;
// #endif



};

#endif /* VALUE_BYTES_WIDGET_H_ */
