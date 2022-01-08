/* value_bytes_widget.h
 *
 * For undefined features.
 */

// Copyright (C) 2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_BYTES_WIDGET_H_
#define VALUE_BYTES_WIDGET_H_

#include "config.h"

#include <QtWidgets/QLineEdit>

#include "core_widgets/number_entry_widget.h"
#include "nongui/feature_value.h"

#include "feature_value_widgets/value_base_widget.h"

class QLabel;
class QPushButton;
class QComboBox;


class ValueBytesWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueBytesWidget(QWidget *parent = nullptr);
    ~ValueBytesWidget();

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    uint16_t getCurrentShSl()                        override;

private:
    QLabel * newTitle(QString title, int titleHeight);
    void     createWidgets();
    void     layoutWidget();
    void     checkAcceptCancelEnabled();

private slots:
    void when_combobox_activated(int index);
    void onApplyButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);

private:   // member variables
    static int idGenerator;
    int _id;
    const char *           _cls;
    QLabel*                _mhTitle;
    QLabel*                _mhValue;
    QLabel*                _mlTitle;
    QLabel*                _mlValue;
    QLabel*                _shTitle;
    QComboBox *            _shValue;
    QLabel*                _slTitle;
    QComboBox *            _slValue;
    QPushButton *          _applyButton;
    QPushButton *          _cancelButton;
    bool                   _guiChange = false;
    const int              widgetHeight = 20;
    // QFont nonMonoFont9;
    uint8_t                _shNew;
    uint8_t                _slNew;
};

#endif /* VALUE_BYTES_WIDGET_H_ */
