/* value_base_widget.h - Common features of all Value_..._Widgets */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_BASE_WIDGET_H
#define VALUE_BASE_WIDGET_H

#include <QtWidgets/QFrame>

#include <ddcutil_c_api.h>

#include "base/global_state.h"
#include "nongui/feature_value.h"

extern bool showValueWidgetResizeEvents;

class ValueBaseWidget : public QFrame    // was ValueAbstractWidget
{
    Q_OBJECT

public:          //methods
    explicit ValueBaseWidget(QWidget *parent = nullptr);

    virtual void     setFeatureValue(const FeatureValue &fv); // override;
    virtual void     setCurrentShSl(uint16_t newval);         // override;
    virtual uint16_t getCurrentShSl();                        // override;

    // QSize sizeHint() const override;   //   needed?
    virtual void     setControlKeyRequired(bool onoff);
    bool             getControlKeyRequired();

protected:       // methods
    // void resizeEvent(QResizeEvent * evt) override;

signals:
    // compiler warning: signals may not be declared virtual
    void featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

public slots:
    void when_controlKeyRequired_changed(bool onoff);

protected:      // member variables
    // initialized since coverity complains that uninitialized in constructor
    // will be set by setFeatureValue() before first use
    uint8_t                 _featureCode = 0;
    DDCA_Display_Ref        _dref = NULL;
    DDCA_Feature_Metadata * _finfo = NULL;
    DDCA_Cap_Vcp *          _capVcp = NULL;
    DDCA_MCCS_Version_Spec  _vspec = DDCA_VSPEC_UNQUERIED;   // yes, it's in DDCA_Feature_Metadata, but this makes use claearer
    uint8_t                 _mh = 0;
    uint8_t                 _ml = 0;
    uint8_t                 _sh = 0;
    uint8_t                 _sl = 0;
    bool                    _controlKeyRequired = false;
    DDCA_Status             _ddcrc = 0;
    GlobalState&            _globalState = GlobalState::instance();
    int                     _featureValueWidgetHeight = 20;

private:        // member variables
    const char *            _cls;
};

#endif // VALUE_BASE_WIDGET_H
