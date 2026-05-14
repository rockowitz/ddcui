/** @file nc_values_dialog.h */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NC_VALUES_DIALOG_H
#define NC_VALUES_DIALOG_H

#include <QAbstractButton>
#include <QDialog>

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

#endif // NC_VALUES_DIALOG_H
