/* value_std_widget.h - Widget for displaying a formatted VCP feature value */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_STD_WIDGET_H
#define VALUE_STD_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

#include "feature_value_widgets/value_base_widget.h"


class ValueStdWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueStdWidget(QWidget *parent = nullptr);

    void setFeatureValue(const FeatureValue  &fv) override;
    void setCurrentShSl(uint16_t newval) override;

protected:
    void resizeEvent(QResizeEvent * evt) override;

private:
    void layoutWidget();
    void setValueField();

private:
    const char * _cls;
    QLabel *     _valueField;
};

#endif // VALUE_STD_WIDGET_H
