/** @file nc_values_dialog.h */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef OTHER_OPTIONS_DIALOG_H
#define OTHER_OPTIONS_DIALOG_H

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDialog>

#include "base/nc_values_state.h"

namespace Ui {
class NcValuesDialog;
}

class NcValuesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NcValuesDialog(NcValuesState * state, QWidget *parent = 0);
    ~NcValuesDialog();

signals:
     void ncValuesSourceChanged(NcValuesSource valuesSource, bool useLatestNcValueNames);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_helpRequested();
    void on_buttonBox_clicked(QAbstractButton* button);

private:
    void setUiSource(NcValuesSource source);
    // void setUseLatestNcValueNames(bool newval);

    const char * _cls;
    Ui::NcValuesDialog *ui;
    NcValuesState * _state;
};

#endif // OTHER_OPTIONS_DIALOG_H
