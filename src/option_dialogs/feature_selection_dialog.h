/** \file feature_selection_dialog.h  */

// Copyright (C) 2018-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURESELECTIONDIALOG_H
#define FEATURESELECTIONDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QAbstractButton>

#include <ddcutil_c_api.h>

#include "base/feature_selector.h"


namespace Ui {
class FeatureSelectionDialog;
}

class FeatureSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureSelectionDialog(QWidget *parent, FeatureSelector* featureSelector);
    ~FeatureSelectionDialog();

    void useSelectorData(FeatureSelector * fsel);
    void useSelectorData();

signals:
    // void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);
    void featureSelectionChanged();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_helpRequested();
    void on_buttonBox_clicked(QAbstractButton* button);

    void on_known_radioButton_clicked(bool checked);
    void on_capabilities_radioButton_clicked(bool checked);
    void on_mfg_radioButton_clicked(bool checked);
    void on_color_radioButton_clicked(bool checked);
    void on_scan_radioButton_clicked(bool checked);
    void on_custom_radioButton_clicked(bool checked);

    void on_allCapabilities_checkbox_stateChanged(int arg1);
    void on_onlyCapabilities_checkbox_stateChanged(int arg1);
 // void on_showUnsupported_checkbox_stateChanged(int arg1);   // not needed

private:     // methods
    DDCA_Feature_List validateCustomFeatureList(char *);

private:     // member variables
    // keep in order for constructor initializers
    char *                      _cls;
    Ui::FeatureSelectionDialog* _ui;
    FeatureSelector*            _featureSelector;
};

#endif // FEATURESELECTIONDIALOG_H

