/** @file value_base_widget.h
 *  Superclass for all Value_..._Widgets
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_BASE_WIDGET_H
#define VALUE_BASE_WIDGET_H

#include <QtWidgets/QFrame>
#include <QtWidgets/QComboBox>
#include <QtGui/QPalette>

#include <ddcutil_types.h>

#include "base/global_state.h"
#include "nongui/feature_value.h"

extern bool showValueWidgetResizeEvents;

class ValueStackedWidget;

class ValueBaseWidget : public QFrame    // was ValueAbstractWidget
{
    Q_OBJECT

// *** Constructors and Methods

public:
    explicit            ValueBaseWidget(QWidget *parent = nullptr);
    virtual             ~ValueBaseWidget();

    virtual void        setFeatureValue(const FeatureValue &fv);
    virtual void        setCurrentShSl(uint16_t newval);
    virtual uint16_t    getCurrentShSl();

    // QSize            sizeHint() const override;   //   needed?

public slots:
    virtual void setEnabled(bool onoff);

signals:
    void                featureValueChanged(uint8_t feature_code,
                                            uint8_t sh, uint8_t sl);

protected:
    QComboBox *      createFormattedComboBox();
    bool             isChangable();

    // void resizeEvent(  QResizeEvent * ev) override;   // for debugging layout

// *** Variables ***

public:
    int                     _id;

protected:
    static int              nextId;

    // initialized since Coverity complains that uninitialized in constructor,
    // will be set by setFeatureValue() before first use
    uint8_t                 _featureCode = 0;
    DDCA_Display_Ref        _dref = NULL;
    DDCA_Feature_Metadata * _finfo = NULL;
    DDCA_Cap_Vcp *          _capVcp = NULL;
    DDCA_MCCS_Version_Spec  _vspec = DDCA_VSPEC_UNQUERIED;   // yes, it's in DDCA_Feature_Metadata, but this makes use clearer
    uint8_t                 _mh = 0;
    uint8_t                 _ml = 0;
    uint8_t                 _sh = 0;
    uint8_t                 _sl = 0;
    DDCA_Status             _ddcrc = 0;
    GlobalState&            _globalState = GlobalState::instance();
    int                     _featureValueWidgetHeight = 20;
    QPalette::ColorRole     _savedBackgroundColor = QPalette::Base;

private:
    const char *            _cls;

};

#endif // VALUE_BASE_WIDGET_H
