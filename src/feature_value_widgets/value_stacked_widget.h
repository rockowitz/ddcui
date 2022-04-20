/** @file value_stacked_widget.h */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_STACKED_WIDGET_H
#define VALUE_STACKED_WIDGET_H

#include <QtWidgets/QStackedWidget>

#include "value_2button_widget.h"
#include "value_base_widget.h"
#include "value_bytes_widget.h"
#include "value_cnc_widget_x14.h"
#include "value_nc_widget.h"
#include "value_ncplus_widget.h"
#include "value_new_cont_widget.h"
#include "value_reset_widget.h"
#include "value_special_widget_x62.h"
#include "value_std_widget.h"


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
    void enableSubwidgets();

protected:
    static int nextId;

private:
    const char *        _cls;
    int                 _id;
    uint8_t             _featureCode;
    int                 _pageno_selected;
    ValueBaseWidget*    _cur_stacked_widget;

    int                 _pageno_std;
    int                 _pageno_cont;
    int                 _pageno_simple_cont;
    int                 _pageno_nc;
    int                 _pageno_reset;
    int                 _pageno_2button;
    int                 _pageno_x14;
    int                 _pageno_bytes;
    int                 _pageno_ncplus;
    int                 _pageno_x62;

    ValueStdWidget*         _stdWidget;
    ValueNewContWidget*     _newContWidget;
    ValueSimpleContWidget*  _simpleContWidget;
    ValueNcWidget*          _ncWidget;
    ValueResetWidget*       _resetWidget;
    Value2ButtonWidget*     _2ButtonWidget;
    ValueCncWidgetX14*      _cncWidgetX14;
    ValueBytesWidget*       _bytesWidget;
    ValueNcplusWidget*      _ncplusWidget;
    ValueSpecialWidgetX62*  _specialWidgetX62;

    ValueBaseWidget *       _subwidget[10];
    int                     _subwidgetCt = 0;

    bool                    _instanceControlKeyRequired = false;
    bool                    _instanceControlKeyPressed = false;
};

#endif // VALUE_STACKED_WIDGET_H


