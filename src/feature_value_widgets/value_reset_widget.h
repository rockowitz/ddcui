/** @file value_reset_widget.h
 *  Widget containing a Reset button
 */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef VALUE_RESET_WIDGET_H
#define VALUE_RESET_WIDGET_H

#include <QtWidgets/QWidget>

#include "value_base_widget.h"

class QPushButton;
class QPalette;

class ValueResetWidget : public ValueBaseWidget
{
    Q_OBJECT

// Constructors and Methods

public:
    ValueResetWidget(QWidget *parent = nullptr);
    ~ValueResetWidget();

public slots:
    void setEnabled(bool onoff) override;

protected:
    void resizeEvent(QResizeEvent * evt) override;  // for debugging

private slots:
    void on_resetButton_pressed();

private:
    void layoutWidget();

// Variables

private:
    const char *        _cls;
    QPushButton *       _resetButton;
};

#endif // VALUE_RESET_WIDGET_H
