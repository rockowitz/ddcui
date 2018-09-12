// other_options_dialog.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef OTHER_OPTIONS_DIALOG_H
#define OTHER_OPTIONS_DIALOG_H

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDialog>

#include "base/other_options_state.h"

namespace Ui {
class OtherOptionsDialog;
}

class OtherOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OtherOptionsDialog(OtherOptionsState * state, QWidget *parent = 0);
    ~OtherOptionsDialog();

signals:
     void ncValuesSourceChanged(NcValuesSource valuesSource);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_helpRequested();
    void on_buttonBox_clicked(QAbstractButton* button);

private:
    void setUiSource(NcValuesSource source);

    Ui::OtherOptionsDialog *ui;
    const char * _cls;
    OtherOptionsState * _state;
};

#endif // OTHER_OPTIONS_DIALOG_H
