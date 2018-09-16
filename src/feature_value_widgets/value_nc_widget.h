/* value_nc_widget.h - Widget for displaying/editing a simple Non-Continuous VCP feature value */

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

#ifndef VALUE_NC_WIDGET_H
#define VALUE_NC_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>

#include "config.h"
#include "base/other_options_state.h"
#include "feature_value_widgets/value_base_widget.h"


class ValueNcWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueNcWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    int findItem(uint8_t sl_value);
    DDCA_Feature_Value_Entry * getComboBoxEntries(NcValuesSource mode);
    void loadComboBox(NcValuesSource mode);
    void reloadComboBox(NcValuesSource newSource);

private:
    QComboBox *    _cb;
#ifdef APPLY_CANCEL
    QPushButton *  _applyButton;
    QPushButton *  _cancelButton;
#endif
    NcValuesSource _curNcValuesSource = OtherOptionsState::DefaultNcValuesSource;
    bool           _guiChange = false;

private slots:
    void combobox_activated(int index);
};

#endif // VALUE_NC_WIDGET_H
