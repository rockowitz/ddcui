/* featureselectiondialog.cpp */

#include <assert.h>
#include <stdio.h>
#include <iostream>

#include "ui_featureselectiondialog.h"
#include "featureselectiondialog.h"


using namespace std;

FeatureSelectionDialog::FeatureSelectionDialog(QWidget *parent, FeatureSelector * featureSelector) :   // was QWidget
    QDialog(parent),
    ui(new Ui::FeatureSelectionDialog)
{
    cout << "(FeatureSelectionDialog)\n";
    printf("(FeatureSelectonDialog) featureSelector = %p\n", featureSelector);
    // this->_mainWindow = mainWindow;
    // this->_local_fsid = mainWindow->feature_list_id();
    ui->setupUi(this);
    QRadioButton * curButton;
    this->_local_fsid = featureSelector->feature_list_id;
    switch(_local_fsid) {
    case DDCA_FEATURE_LIST_KNOWN:
        curButton = ui->known_radioButton;
        break;
    case DDCA_FEATURE_LIST_COLOR:
                curButton = ui->color_radioButton;
        break;
    case DDCA_FEATURE_LIST_PROFILE:
                curButton = ui->profile_RadioButton;
        break;
    case DDCA_FEATURE_LIST_MFG:
                curButton = ui->mfg_RadioButton;
        break;
    }
    curButton->setChecked(true);

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
  setFeatureSet(DDCA_FEATURE_LIST_KNOWN);
}

void FeatureSelectionDialog::on_scan_radioButton_clicked()
{
  // setFeatureSet(DDCA_FEATURE_LIST_SCAN);
}

void FeatureSelectionDialog::on_mfg_RadioButton_clicked()
{
  setFeatureSet(DDCA_FEATURE_LIST_MFG);
}

void FeatureSelectionDialog::on_profile_RadioButton_clicked()
{
  setFeatureSet(DDCA_FEATURE_LIST_PROFILE);
}

void FeatureSelectionDialog::on_color_radioButton_clicked()
{
  setFeatureSet(DDCA_FEATURE_LIST_COLOR);
}

void FeatureSelectionDialog::on_show_unsupported_checkBox_stateChanged(int arg1)
{
   cout << "(on_show_unpported_checkBox_StateChanged) arg1 = " << arg1 << endl;
}

void FeatureSelectionDialog::on_include_table_checkBox_stateChanged(int arg1)
{
   cout << "(on_include_table_checkBox_StateChanged) arg1 = " << arg1 << endl;
}

void FeatureSelectionDialog::on_buttonBox_accepted()
{
    // which button is currently clicked?
    DDCA_Feature_List_Id feature_list;
    if (ui->color_radioButton->isChecked())
        feature_list = DDCA_FEATURE_LIST_COLOR;
    else if (ui->known_radioButton->isChecked())
        feature_list = DDCA_FEATURE_LIST_KNOWN;
    else if (ui->mfg_RadioButton->isChecked())
        feature_list = DDCA_FEATURE_LIST_MFG;
    else if (ui->profile_RadioButton->isChecked())
        feature_list = DDCA_FEATURE_LIST_PROFILE;
    else if (ui->scan_radioButton->isChecked())
        feature_list = DDCA_FEATURE_LIST_KNOWN;    // hack for now
    else
        feature_list = DDCA_FEATURE_LIST_KNOWN;    // should never occur

    // this->_mainWindow->set_feature_list_id(feature_list);
    this->_feature_selector->feature_list_id = feature_list;

    // todo:
    // show_unsupported and show-table check boces
}
