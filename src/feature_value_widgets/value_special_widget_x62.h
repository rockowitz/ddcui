// value_special_widget_x62.h

// Copyright (C) 2020-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_SPECIAL_WIDGET_X62_H_
#define VALUE_SPECIAL_WIDGET_X62_H_

#include "config.h"
#include "base/ddcui_parms.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_simple_cont_widget.h"

class QLabel;
class QPushButton;
class QSlider;
class QSpinBox;
class EnhancedSlider;

class ValueSpecialWidgetX62 : public ValueSimpleContWidget
{
    Q_OBJECT

public:            // methods
    ValueSpecialWidgetX62(QWidget *parent = nullptr);
    ~ValueSpecialWidgetX62();

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    uint16_t getCurrentShSl()                        override;

private:           // methods
    void createInitialWidgets();
    void layoutWidget(QHBoxLayout * layout);
    void loadComboBox();

private slots:     // methods
    void combobox_activated(int index);

private:           // member variables
    const char *     _cls;
    QComboBox *      _cb;
};

#endif  // VALUE_SPECIAL_WIDGET_X62_H_

