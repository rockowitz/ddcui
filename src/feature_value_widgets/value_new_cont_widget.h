/** \file value_new_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_NEW_CONT_WIDGET_H
#define VALUE_NEW_CONT_WIDGET_H

#include "config.h"

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_simple_cont_widget.h"


class ValueNewContWidget : public ValueSimpleContWidget
{
    Q_OBJECT

public:
    ValueNewContWidget(QWidget *parent = nullptr);
    ~ValueNewContWidget();
    void setFeatureValue(const FeatureValue &fv) override;

private:
    void layoutWidget(QHBoxLayout * layout) override;

    const char * _cls;
};

#endif // VALUE_NEW_CONT_WIDGET_H

