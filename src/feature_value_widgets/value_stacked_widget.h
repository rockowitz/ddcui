/** @file value_stacked_widget.h */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_STACKED_WIDGET_H
#define VALUE_STACKED_WIDGET_H

#include <QStackedWidget>

#include "base/nc_values_state.h"

#include "value_base_widget.h"


class QWidget;
class QPaintEvent;

class ValueStackedWidget :  public QStackedWidget
{
   Q_OBJECT

public:
    explicit ValueStackedWidget(QWidget *parent = nullptr);
    ~ValueStackedWidget();

    void     setFeatureValue(const FeatureValue &fv);
    void     setCurrentValue(uint16_t newval);
    // uint16_t getCurrentValue();
#ifdef NC_FEATURE_VALUES2
    bool     hasSlTable();
#endif


public slots:
    void     setNcValuesSource(NcValuesSource newsrc, bool newUseLatestNames);
    void     setInstanceControlKeyRequired(bool onoff);
    void     setInstanceControlKeyPressed(bool onoff);

    // QSize sizeHint() const override;
    // void paintEvent(QPaintEvent *event) override;

signals:
    void stackedFeatureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private slots:
    void forContainedWidgetChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private:
    void enableCurrentWidget();

protected:
    static int nextId;

private:
    const char *        _cls;
    int                 _id;
    uint8_t             _featureCode;

    // The single value widget appropriate to the feature, created by
    // setFeatureValue().  NULL until setFeatureValue() has been called.
    ValueBaseWidget*    _cur_stacked_widget = nullptr;

    bool                _instanceControlKeyRequired = false;
    bool                _instanceControlKeyPressed = false;
};

void init_value_stacked_widget();

#endif // VALUE_STACKED_WIDGET_H
