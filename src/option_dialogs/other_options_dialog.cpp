/* other_options_dialog.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <QtCore/QDebug>

#include "base/core.h"
#include "help/help_dialog.h"
#include "help/help_browser.h"

#include "option_dialogs/other_options_dialog.h"
#include "ui_other_options_dialog.h"    // generated by UIC


void OtherOptionsDialog::setUiSource(NcValuesSource source)
{
   bool debugFunc = false;
   TRACEMF(debugFunc, "Executing. source=%d=%s", source, ncValuesSourceName(source) );
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
   }
}

void OtherOptionsDialog::setUseLatestNcValueNames(bool newval) {
   ui->latestNcValueNamesCheckbox->setChecked(newval);
}


OtherOptionsDialog::OtherOptionsDialog(OtherOptionsState * state, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtherOptionsDialog),
    _cls( strdup(metaObject()->className()) ),
    _state(state)
{
    // _state = state;
    ui->setupUi(this);
    // this->setContextMenuPolicy(Qt::PreventContextMenu);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    setWindowTitle("ddcui - NC Feature Values");
    setUiSource(state->_ncValuesSource);
    setUseLatestNcValueNames(state->_useLatestNcValues);
}


OtherOptionsDialog::~OtherOptionsDialog()
{
    delete ui;
    free((void*) _cls);
}


void OtherOptionsDialog::on_buttonBox_accepted()
{
    // NcValuesSource oldsrc = _state->_ncValuesSource;
    // bool           oldUseLatestNames = _state->_useLatestNcValueNames;

    NcValuesSource newsrc;

    if (ui->capabilitiesNcValuesButton->isChecked() )
       newsrc = NcValuesFromCapabilities;
    else if (ui->bothNcValuesButton->isChecked() )
        newsrc = NcValuesFromBoth;
    else {
       assert(  ui->mccsNcValuesButton->isChecked() );
       newsrc = NcValuesFromMccs;
    }

    _state->changeNcValuesSource(newsrc, ui->latestNcValueNamesCheckbox->isChecked());
}


void OtherOptionsDialog::on_buttonBox_helpRequested()
{
   // TRACEC("Starting");
   // viewHelp(QString("nc_values.html"), QString("ddcui Help -X Other Options"), this);
   // HelpBrowser::showPage("qrc:docs/help_nc_values.html", false);

   HelpDialog* hd = new HelpDialog(this);
   hd->setSource("qrc:docs/help_nc_values.html");
   hd->exec();
}


// Reset
void OtherOptionsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
   if(button== (QAbstractButton*) ui->buttonBox->button(QDialogButtonBox::Reset) ){
      // TRACE("Reset");
      setUiSource(OtherOptionsState::DefaultNcValuesSource);
      setUseLatestNcValueNames(OtherOptionsState::DefaultUseMaximalMccsNcValues);
   }
}
