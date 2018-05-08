/* feature_selection_dialog.h */

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
