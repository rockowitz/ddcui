// other_options_dialog.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "option_dialogs/other_options_dialog.h"

#include <assert.h>
#include <QtCore/QDebug>

#include "base/ddcui_globals.h"
#include "help/help_dialog.h"

#include "ui_other_options_dialog.h"


void OtherOptionsDialog::setUiSource(NcValuesSource source)
{
   switch(source) {
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


OtherOptionsDialog::OtherOptionsDialog(OtherOptionsState * state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherOptionsDialog),
    _cls( metaObject()->className() ),
    _state(state)
{
    // _state = state;
    ui->setupUi(this);

    setWindowTitle("ddcui - Other Options");
    setUiSource(state->ncValuesSource);
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


void OtherOptionsDialog::on_buttonBox_helpRequested()
{
    // PRINTFCM();
    QString fn(":/docs/nc_values.html");
    QFile f(fn);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&f);

    QString htmlText = in.readAll();

    // qDebug() << htmlText;

    // doesn't show dialog box
    // HelpDialog2("ddcui Help - Other Options", htmlText, this);

    HelpDialog2* hd = new HelpDialog2(this);
    hd->setText(htmlText);
    hd->setWindowTitle("ddcui Help - Other Options");
    hd->show();
}


// Reset
void OtherOptionsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
   if(button== (QAbstractButton*) ui->buttonBox->button(QDialogButtonBox::Reset) ){
      // PRINTFCM("Reset");
      setUiSource(OtherOptionsState::DefaultNcValuesSource);
      // on_buttonBox_accepted();  // do not actually change until OK

      // NcValuesSource oldsrc = _state->ncValuesSource;
      //  _state->ncValuesSource = OtherOptionsState::DefaultNcValuesSource;
      //   setUiSource(_state->ncValuesSource);
      //   if (_state->ncValuesSource != oldsrc) {
      //       emit ncValuesSourceChanged(_state->ncValuesSource);
      //   }
   }
}

