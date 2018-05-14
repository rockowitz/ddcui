/* other_options_dialog.cpp */

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

#include "option_dialogs/other_options_dialog.h"

#include <assert.h>

#include "ui_other_options_dialog.h"

OtherOptionsDialog::OtherOptionsDialog(OtherOptionsState * state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherOptionsDialog)
{
    _state = state;
    ui->setupUi(this);

    switch(state->ncValuesSource) {
    case NcValuesFromCapabilities:
        ui->capabilitiesNcValuesButton->setChecked(true);
        break;
    case NcValuesFromMccs:
        ui->mccsNcValuesButton->setChecked(true);
        break;
    case NcValuesFromBoth:
       ui->bothNcValuesButton->setChecked(true);
       break;
    case NcValuesSourceUnset:
       assert(false);
    }
}

OtherOptionsDialog::~OtherOptionsDialog()
{
    delete ui;
}

void OtherOptionsDialog::on_buttonBox_accepted()
{
    NcValuesSource oldsrc = _state->ncValuesSource;

    if (ui->capabilitiesNcValuesButton->isChecked() )
        _state->ncValuesSource = NcValuesFromCapabilities;
    else if (ui->bothNcValuesButton->isChecked() )
        _state->ncValuesSource = NcValuesFromBoth;
    else {
       assert(  ui->mccsNcValuesButton->isChecked() );
       _state->ncValuesSource = NcValuesFromMccs;
    }

    if (_state->ncValuesSource != oldsrc) {
        emit ncValuesSourceChanged(_state->ncValuesSource);

    }
}
