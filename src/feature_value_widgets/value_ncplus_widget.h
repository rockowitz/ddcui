/** @file value_ncplus_widget.cpp
 * For NC features with additional information in a byte other thatn SL
 */

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_NCPLUS_WIDGET_H_
#define VALUE_NCPLUS_WIDGET_H_

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include <ddcutil_types.h>

#include "config.h"
#include "base/ddca_utils.h"
#include "base/other_options_state.h"
#include "feature_value_widgets/value_base_widget.h"

#include "feature_value_widgets/value_nc_widget.h"


class ValueNcplusWidget : public ValueNcWidget
{
    Q_OBJECT

public:
    ValueNcplusWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    // uint16_t getCurrentValue() override;

protected:
    void resizeEvent(QResizeEvent * evt) override;

private:
    // QLabel * _auxTitle;
    // QLabel * _auxDetail;

    // void layoutWidget();
    void setAuxFieldsX14();
    void setAuxFieldsXca();
    void setAuxFields();

private:
    const char *    _cls;
};

#endif /* VALUE_NCPLUS_WIDGET_H_ */

