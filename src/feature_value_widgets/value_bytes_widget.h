// value_bytes_widget.h

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
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


class ValueBytesWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueBytesWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentShSl(uint16_t newval)         override;
    uint16_t getCurrentShSl()                        override;

private:
    QLabel * newTitle(QString title, int titleHeight);
    void     layoutWidget();
    void     checkAcceptCancelEnabled();

private slots:
    // void whenSlChanged(uint8_t val);
    // void whenShChanged(uint8_t val);
    // void whenTextValueChanged(bool ok);
    // void whenEventFieldChanged(int fieldNumber);

    void whenStateChanged(NumberEntryWidget * whichWidget, NumberEntryWidget::States newState);

    void onApplyButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);

private:   // member variables
    static int idGenerator;
    int _id;
    const char *           _cls;
    QLabel*                _mhTitle;
    NumberEntryWidget *    _mhWidget;
    QLabel*                _mlTitle;
    NumberEntryWidget *    _mlWidget;
    QLabel*                _shTitle;
    NumberEntryWidget *    _shWidget;
    QLabel*                _slTitle;
    NumberEntryWidget *    _slWidget;
    // uint8_t               _sl;
    // uint16_t    _newval;
    bool                   _guiChange = false;
    int                    widgetHeight = 20;

    QFont nonMonoFont9;

    // bool _shGood = true;
    // bool _slGood = true;
    // uint8_t _shNew;
    // uint8_t _slNew;

    int _currentField = 0;

    NumberEntryWidget::States _slState =  NumberEntryWidget::StateOldValid;
    NumberEntryWidget::States _shState =  NumberEntryWidget::StateOldValid;

   QPushButton * _applyButton;
   QPushButton * _cancelButton;
};

#endif /* VALUE_BYTES_WIDGET_H_ */
