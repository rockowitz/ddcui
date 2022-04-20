/** @file value_nc_widget.h
 *  Widget to display/edit a simple Non-Continuous VCP feature value
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_NC_WIDGET_H
#define VALUE_NC_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>

#include "config.h"
#include "c_util/data_structures.h"
#include "feature_value_widgets/value_base_widget.h"


class ValueNcWidget : public ValueBaseWidget
{
    Q_OBJECT

// *** Methods ***

public:
    ValueNcWidget(QWidget *parent = nullptr);
    ~ValueNcWidget();

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval) override;
    uint16_t getCurrentShSl() override;

public slots:
    void     reloadComboBox(NcValuesSource newSource, bool useLatestNames);
    void     setEnabled(bool onoff) override;


protected:
    // void  layoutWidgetAux();
    void     resizeEvent(QResizeEvent * evt) override;

protected slots:
    void     combobox_activated(int index);

private:
    void     createWidgets();
    void     layoutWidget();
    int      findItem(uint8_t sl_value);
    char *   sl_value_table_lookup(DDCA_Feature_Value_Entry * value_entries, uint8_t value_id);
    void     loadComboBox2();


// *** Member Variables ***

protected:
    QComboBox *    _cb;
    QHBoxLayout *  _layout;
    QLabel *       _extraInfo;

private:
    NcValuesSource _ncValuesSource = OtherOptionsState::DefaultNcValuesSource;
    bool           _useLatestNcValues = OtherOptionsState::DefaultUseMaximalMccsNcValues;
    Bit_Set_256    _observedValues = EMPTY_BIT_SET_256;
    Bit_Set_256    _validValues = EMPTY_BIT_SET_256;
    bool           _guiChange = false;
    const char *   _cls;
};

#endif // VALUE_NC_WIDGET_H
