// user_interface_options_dialog.h

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef USER_INTERFACE_OPTIONS_DIALOG_H
#define USER_INTERFACE_OPTIONS_DIALOG_H

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDialog>

#include "base/user_interface_options_state.h"

namespace Ui {
class UserInterfaceOptionsDialog;
}

class UserInterfaceOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInterfaceOptionsDialog(UserInterfaceOptionsState * state, QWidget *parent = 0);
    ~UserInterfaceOptionsDialog();

    void setCkrChanged(bool onoff);
    void setDialogBoxControlKeyRequired(bool onoff);

signals:
     void ckrChanged(bool onoff);
     void  userInterfaceDialog_accepted(UserInterfaceOptionsState * state);

public:
    Ui::UserInterfaceOptionsDialog *  _ui;   // why is this pointing to another instance of the same class?

private slots:
    void on_actionButtonBox_accepted();
    void on_actionButtonBox_helpRequested();
    void on_actionButtonBox_clicked(QAbstractButton* button);

private:
    const char * _cls;
    // void setUiSource(NcValuesSource source);

    UserInterfaceOptionsState *       _state;
};

#endif // USER_INTERFACE_OPTIONS_DIALOG_H
