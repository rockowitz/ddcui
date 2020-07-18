// value_special_widget_x62.h

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_SPECIAL_WIDGET_X62_H_
#define VALUE_SPECIAL_WIDGET_X62_H_

#include "config.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_simple_cont_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;
class EnhancedSlider;

class ValueSpecialWidgetX62 : public ValueSimpleContWidget
{
    Q_OBJECT

public:
    ValueSpecialWidgetX62(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    uint16_t getCurrentShSl()                        override;
    // void     setControlKeyRequired(bool onoff)       override;

    int findItem(uint8_t sl_value);
    // Local_Feature_Value_Table * getComboBoxEntries(NcValuesSource mode);
    // void loadComboBox(NcValuesSource mode);
    // void reloadComboBox(NcValuesSource newSource);

protected:

private:
    void layoutWidget(QHBoxLayout * layout);
    void loadComboBox();

private slots:
    void combobox_activated(int index);
    // void forSpinSliderChanged(uint16_t newval);


protected:   // member variables
    QComboBox *   _cb;


private:    // member variables
    void createInitialWidgets();

    const char *     _cls;

};

#endif  // VALUE_SPECIAL_WIDGET_X62_H_

