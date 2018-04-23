/* feature_selection_dialog.cpp */

#include "../option_dialogs/feature_selection_dialog.h"

#include <assert.h>
#include <stdio.h>
#include <iostream>

#include "ui_featureselectiondialog.h"


using namespace std;

FeatureSelectionDialog::FeatureSelectionDialog(
      QWidget *         parent,
      FeatureSelector * featureSelector
   ) :  QDialog(parent),
        ui(new Ui::FeatureSelectionDialog)
{
    cout << "(FeatureSelectionDialog)\n";
    printf("(FeatureSelectonDialog) featureSelector = %p\n", featureSelector);
    // this->_mainWindow = mainWindow;
    // this->_local_fsid = mainWindow->feature_list_id();
    ui->setupUi(this);
    QRadioButton * curButton;
    this->_local_fsid = featureSelector->_featureListId;
    switch(_local_fsid) {
    case DDCA_SUBSET_KNOWN:
        curButton = ui->known_radioButton;
        break;
    case DDCA_SUBSET_COLOR:
        curButton = ui->color_radioButton;
        break;
    case DDCA_SUBSET_PROFILE:
        curButton = ui->profile_RadioButton;
        break;
    case DDCA_SUBSET_MFG:
        curButton = ui->mfg_RadioButton;
        break;
    case DDCA_SUBSET_UNSET:
       assert(false);
       break;
    }
    curButton->setChecked(true);

    ui->include_table_checkBox->setChecked(   featureSelector->_includeTableFeatures);
    ui->show_unsupported_checkBox->setChecked(featureSelector->_showUnsupportedFeatures);
    ui->capabilities_checkbox->setChecked(    featureSelector->_respectCapabilities);

    this->_feature_selector = featureSelector;

}

FeatureSelectionDialog::~FeatureSelectionDialog()
{
    delete ui;
}


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

void FeatureSelectionDialog::on_buttonBox_accepted()
{
    printf("(FeatureSelectionDialog::%s)\n",  __func__); fflush(stdout);
    // which button is currently clicked?
    DDCA_Feature_Subset_Id feature_list;
    if (ui->color_radioButton->isChecked())
        feature_list = DDCA_SUBSET_COLOR;
    else if (ui->known_radioButton->isChecked())
        feature_list = DDCA_SUBSET_KNOWN;
    else if (ui->mfg_RadioButton->isChecked())
        feature_list = DDCA_SUBSET_MFG;
    else if (ui->profile_RadioButton->isChecked())
        feature_list = DDCA_SUBSET_PROFILE;
    else if (ui->scan_radioButton->isChecked())
        feature_list = DDCA_SUBSET_KNOWN;    // hack for now
    else
        feature_list = DDCA_SUBSET_KNOWN;    // should never occur

    // this->_mainWindow->set_feature_list_id(feature_list);
    this->_feature_selector->_featureListId = feature_list;

    _feature_selector->_includeTableFeatures    = ui->include_table_checkBox->isChecked();
    _feature_selector->_showUnsupportedFeatures = ui->show_unsupported_checkBox->isChecked();
    _feature_selector->_respectCapabilities     = ui->capabilities_checkbox->isChecked();

    emit featureSelectionAccepted(feature_list);

    // todo:
    // show_unsupported and show-table check boces
}
