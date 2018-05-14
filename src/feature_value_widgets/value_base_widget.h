/* value_base_widget.h - Common features of all Value_..._Widgets */

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

#ifndef VALUE_BASE_WIDGET_H
#define VALUE_BASE_WIDGET_H

// #include <QtCore/QSize>
#include <QtWidgets/QFrame>

#include <ddcutil_c_api.h>

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_abstract_widget.h"

class FeatureBaseModel;

class ValueBaseWidget : public ValueAbstractWidget
{
    Q_OBJECT

public:
    explicit ValueBaseWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    // QSize sizeHint() const override;   //   needed?

    uint8_t                _featureCode;
    DDCA_Display_Ref       _dref;
    DDCA_Feature_Metadata  _finfo;
    DDCA_Cap_Vcp *         _capVcp;
    uint8_t                _mh;
    uint8_t                _ml;
    uint8_t                _sh;
    uint8_t                _sl;

protected:
    char * _cls;

signals:
// compiler warning: signals may not be declared virtual
void featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);
// virtual void featureValueChanged(uint16_t val);

public slots:
};

#endif // VALUE_BASE_WIDGET_H
