/* feature_selection_dialog.cpp */

#include <assert.h>
#include <stdio.h>
#include <iostream>

#include "ui_featureselectiondialog.h"

#include "base/ddcui_globals.h"
#include "feature_selection_dialog.h"


using namespace std;


void FeatureSelectionDialog::useSelectorData()
{
   if (debugFeatureSelection) {
       PRINTFCM("_feature_selector:");
       _featureSelector->dbgrpt();
   }

   QRadioButton * curButton;
   DDCA_Feature_Subset_Id local_fsid = _featureSelector->_featureListId;
      switch(local_fsid) {
      case DDCA_SUBSET_KNOWN:
          curButton = _ui->known_radioButton;
          break;
      case DDCA_SUBSET_COLOR:
          curButton = _ui->color_radioButton;
          break;
      case DDCA_SUBSET_PROFILE:
          curButton = _ui->profile_RadioButton;
          break;
      case DDCA_SUBSET_MFG:
          curButton = _ui->mfg_RadioButton;
          break;
      case DDCA_SUBSET_UNSET:
         assert(false);
         break;
      }
      curButton->setChecked(true);

      _ui->include_table_checkBox->setChecked(   _featureSelector->_includeTableFeatures);
      _ui->show_unsupported_checkBox->setChecked(_featureSelector->_showUnsupportedFeatures);
      _ui->capabilities_checkbox->setChecked(    _featureSelector->_respectCapabilities);
}


FeatureSelectionDialog::FeatureSelectionDialog(
      QWidget *         parent,
      FeatureSelector * featureSelector
   ) :  QDialog(parent),
        _ui(new Ui::FeatureSelectionDialog),
        _featureSelector(featureSelector)

{
   _cls                    = strdup(metaObject()->className());
    // printf("(FeatureSelectonDialog) featureSelector = %p\n", featureSelector); fflush(stdout);
    _ui->setupUi(this);
    useSelectorData();
}


FeatureSelectionDialog::~FeatureSelectionDialog()
{
    delete _ui;
}


#ifdef UNUSED
void FeatureSelectionDialog::setFeatureSet(int fsid) {

    cout << "(setFeatureSet) fsid=" << fsid << endl;
}

void FeatureSelectionDialog::on_known_radioButton_clicked(bool checked)
{
   cout << "(FeatureSelectionDialog::on_known_radioButton_clicked) checked=" << checked << endl;
}

void FeatureSelectionDialog::on_known_radioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_KNOWN);
}

void FeatureSelectionDialog::on_scan_radioButton_clicked()
{
  // setFeatureSet(DDCA_FEATURE_LIST_SCAN);
}

void FeatureSelectionDialog::on_mfg_RadioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_MFG);
}

void FeatureSelectionDialog::on_profile_RadioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_PROFILE);
}

void FeatureSelectionDialog::on_color_radioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_COLOR);
}

void FeatureSelectionDialog::on_capabilities_checkbox_stateChanged(int arg1)
{
   cout << "(on_capabilities_checkBox_stateChanged) arg1 = " << arg1 << endl;
}

void FeatureSelectionDialog::on_show_unsupported_checkBox_stateChanged(int arg1)
{
   cout << "(on_show_unpported_checkBox_stateChanged) arg1 = " << arg1 << endl;
}

void FeatureSelectionDialog::on_include_table_checkBox_stateChanged(int arg1)
{
   cout << "(on_include_table_checkBox_StateChanged) arg1 = " << arg1 << endl;
}
#endif


void FeatureSelectionDialog::on_buttonBox_accepted()
{
    // printf("(FeatureSelectionDialog::%s)\n",  __func__); fflush(stdout);
    // which button is currently clicked?

    DDCA_Feature_Subset_Id feature_list;
    if (_ui->color_radioButton->isChecked())
        feature_list = DDCA_SUBSET_COLOR;
    else if (_ui->known_radioButton->isChecked())
        feature_list = DDCA_SUBSET_KNOWN;
    else if (_ui->mfg_RadioButton->isChecked())
        feature_list = DDCA_SUBSET_MFG;
    else if (_ui->profile_RadioButton->isChecked())
        feature_list = DDCA_SUBSET_PROFILE;
    else if (_ui->scan_radioButton->isChecked())
        feature_list = DDCA_SUBSET_KNOWN;    // hack for now
    else
        feature_list = DDCA_SUBSET_KNOWN;    // should never occur

    bool changed = false;
    if (feature_list != _featureSelector->_featureListId) {
       _featureSelector->_featureListId = feature_list;
       changed = true;
    }
    if (_ui->include_table_checkBox->isChecked() !=  _featureSelector->_includeTableFeatures) {
       _featureSelector->_includeTableFeatures    = _ui->include_table_checkBox->isChecked();
       changed = true;
    }
    if (_featureSelector->_showUnsupportedFeatures !=_ui->show_unsupported_checkBox->isChecked()) {
       _featureSelector->_showUnsupportedFeatures = _ui->show_unsupported_checkBox->isChecked();
       changed = true;
    }
    if (_featureSelector->_respectCapabilities  != _ui->capabilities_checkbox->isChecked() ) {
       _featureSelector->_respectCapabilities     = _ui->capabilities_checkbox->isChecked();
       changed = true;
    }
    // TODO: show_unsupported and show-table check boxes

    if (debugFeatureSelection) {
        PRINTFCM("_feature_selector:");
        _featureSelector->dbgrpt();
        if (changed)
           PRINTFCM("Signaling featureSelectionChanged()");
        else
           PRINTFCM("NOT Signaling featureSelectionChanged()");
    }

    if (changed)
       emit featureSelectionChanged();
    // emit featureSelectionAccepted(feature_list);
}

