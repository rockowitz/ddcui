/* value_cnc_widget_x14.h
 *
 *  Special handling for complex non-continuous feature x14 - Color Preset
 */

// Copyright (C) 2019-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_CNC_WIDGET_X14_H_
#define VALUE_CNC_WIDGET_X14_H_

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "../base/nc_values_state.h"
#include "config.h"
#include "base/ddca_utils.h"
#include "feature_value_widgets/value_base_widget.h"

#include "feature_value_widgets/value_nc_widget.h"


class ValueCncWidgetX14 : public ValueNcWidget
{
    Q_OBJECT

public:
    ValueCncWidgetX14(QWidget *parent = nullptr);
    ~ValueCncWidgetX14();

    void     setFeatureValue(const FeatureValue &fv) override;
    // void     setCurrentValue(uint16_t newval) override;
    // uint16_t getCurrentValue() override;

    // int findItem(uint8_t sl_value);
    // Local_Feature_Value_Table * getComboBoxEntries(NcValuesSource mode);
    // void loadComboBox(NcValuesSource mode);
    // void reloadComboBox(NcValuesSource newSource);

private:
    QLabel * _mhdesc;
    QLabel * _mhlabel;
    const char * _cls;

#ifdef NO
    QComboBox *    _cb;
#ifdef APPLY_CANCEL
    QPushButton *  _applyButton;
    QPushButton *  _cancelButton;
#endif
    NcValuesSource _curNcValuesSource = OtherOptionsState::DefaultNcValuesSource;
    bool           _guiChange = false;

private slots:
    void combobox_activated(int index);
#endif
};

#endif /* VALUE_CNC_WIDGET_X14_H_ */
