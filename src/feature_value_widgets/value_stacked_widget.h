#ifndef VALUE_STACKED_WIDGET_H
#define VALUE_STACKED_WIDGET_H

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

#include <QtWidgets/QStackedWidget>

// #include "nongui/simple_feature_value.h"
// #include "nongui/simple_feature_value_subject.h"

// #include "value_abstract_widget.h"
#include "value_base_widget.h"
#include "value_std_widget.h"
#include "value_cont_widget.h"
#include "value_nc_widget.h"
#include "value_reset_widget.h"
#include "value_2button_widget.h"

class QWidget;
class QPaintEvent;

class ValueStackedWidget :  public QStackedWidget
{
   Q_OBJECT

public:
    explicit ValueStackedWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv);
    void     setCurrentValue(uint16_t newval);
    uint16_t getCurrentValue();
    bool     isSimpleNc();
    void     setNcValuesSource(NcValuesSource newsrc);

    // QSize sizeHint() const override;
    // void paintEvent(QPaintEvent *event) override;

signals:
    void stackedFeatureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private:
    const char *        _cls;
    uint8_t             _featureCode;
    QStackedWidget*     _stacked;
    int                 _pageno_selected;
    ValueBaseWidget*    _cur_stacked_widget;

    int                 _pageno_std;
    int                 _pageno_cont;
    int                 _pageno_nc;
    int                 _pageno_reset;
    int                 _pageno_2button;

    ValueStdWidget*     _stdWidget;
    ValueContWidget*    _contWidget;
    ValueNcWidget*      _ncWidget;
    ValueResetWidget*   _resetWidget;
    Value2ButtonWidget* _2ButtonWidget;

private slots:
    void onContainedWidgetChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

};

#endif // VALUE_STACKED_WIDGET_H


