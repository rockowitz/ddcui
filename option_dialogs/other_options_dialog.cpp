/* other_options_dialog.cpp */

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
