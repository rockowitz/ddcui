/* value_std_widget.h - Widget for displaying a formatted VCP feature value */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_STD_WIDGET_H
#define VALUE_STD_WIDGET_H

#include <QWidget>
#include <QLabel>

#include "feature_value_widgets/value_base_widget.h"


class ValueStdWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueStdWidget(QWidget *parent = nullptr);
    ~ValueStdWidget();

    void setFeatureValue(const FeatureValue  &fv) override;
    void setCurrentShSl(uint16_t newval) override;

public slots:
    void setEnabled(bool onoff) override;

protected:
    void resizeEvent(QResizeEvent * evt) override;

private:
    void layoutWidget();
    void setValueField();

private:
    const char * _cls;
    QLabel *     _valueField;
};

void init_value_std_widget();

#endif // VALUE_STD_WIDGET_H
