/* feature_selection_dialog.h */

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

#ifndef FEATURESELECTIONDIALOG_H
#define FEATURESELECTIONDIALOG_H

#include <QtWidgets/QDialog>

#include <ddcutil_c_api.h>

#include "base/feature_selector.h"

// #include "main/mainwindow.h"


namespace Ui {
class FeatureSelectionDialog;
}

class FeatureSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureSelectionDialog(QWidget *parent, FeatureSelector* featureSelector);
    ~FeatureSelectionDialog();

    void useSelectorData();

signals:
    // void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);
    void featureSelectionChanged();

private slots:
    void on_buttonBox_accepted();

#ifdef UNUSED
    void on_known_radioButton_clicked(bool checked);
    void on_known_radioButton_clicked();
    void on_scan_radioButton_clicked();
    void on_mfg_RadioButton_clicked();
    void on_profile_RadioButton_clicked();
    void on_color_radioButton_clicked();
    void on_capabilities_checkbox_stateChanged(int arg1);
    void on_show_unsupported_checkBox_stateChanged(int arg1);
    void on_include_table_checkBox_stateChanged(int arg1);
#endif

private:
    // keep in order for constructor initializers
    char *                      _cls;
    Ui::FeatureSelectionDialog* _ui;
    FeatureSelector*            _featureSelector;
};

#endif // FEATURESELECTIONDIALOG_H
