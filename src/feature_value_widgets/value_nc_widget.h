/* value_nc_widget.h - Widget for displaying/editing a simple Non-Continuous VCP feature value */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_NC_WIDGET_H
#define VALUE_NC_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>

#include "config.h"
#include "base/ddca_utils.h"
#include "base/other_options_state.h"
#include "feature_value_widgets/value_base_widget.h"


class ValueNcWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueNcWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval) override;
    uint16_t getCurrentShSl() override;

    int findItem(uint8_t sl_value);
    Local_Feature_Value_Table * getComboBoxEntries(NcValuesSource mode);
    void loadComboBox(NcValuesSource mode);
    void reloadComboBox(NcValuesSource newSource);

protected:
    // void layoutWidgetAux();
    void resizeEvent(QResizeEvent * evt) override;

private:
    void layoutWidget();

#ifdef APPLY_CANCEL
    QPushButton *  _applyButton;
    QPushButton *  _cancelButton;
#endif
    NcValuesSource _curNcValuesSource = OtherOptionsState::DefaultNcValuesSource;
    bool           _guiChange = false;

protected slots:
    void combobox_activated(int index);

protected:   // member variables
    QComboBox *   _cb;
    QHBoxLayout * _layout;
    QLabel * _extraInfo;

private:     // member variables
    const char *                  _cls;
};

#endif // VALUE_NC_WIDGET_H
