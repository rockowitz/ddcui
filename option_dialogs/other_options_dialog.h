/* other_options_dialog.h */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef OTHER_OPTIONS_DIALOG_H
#define OTHER_OPTIONS_DIALOG_H

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

private:
    Ui::OtherOptionsDialog *ui;

    OtherOptionsState * _state;
};

#endif // OTHER_OPTIONS_DIALOG_H
