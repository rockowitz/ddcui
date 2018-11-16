/* value_base_widget.h - Common features of all Value_..._Widgets */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_BASE_WIDGET_H
#define VALUE_BASE_WIDGET_H

#include <QtWidgets/QFrame>

#include <ddcutil_c_api.h>

#include "nongui/feature_value.h"


class ValueBaseWidget : public QFrame    // was ValueAbstractWidget
{
    Q_OBJECT

public:
    explicit ValueBaseWidget(QWidget *parent = nullptr);

    virtual void     setFeatureValue(const FeatureValue &fv); // override;
    virtual void     setCurrentValue(uint16_t newval);        // override;
    virtual uint16_t getCurrentValue();                       // override;

    // QSize sizeHint() const override;   //   needed?

protected:
    char *                  _cls;
    uint8_t                 _featureCode;
    DDCA_Display_Ref        _dref;
    DDCA_Feature_Metadata * _finfo;
    DDCA_Cap_Vcp *          _capVcp;
    uint8_t                 _mh;
    uint8_t                 _ml;
    uint8_t                 _sh;
    uint8_t                 _sl;

signals:
// compiler warning: signals may not be declared virtual
void featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

};

#endif // VALUE_BASE_WIDGET_H
