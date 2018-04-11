/* feature_welection_dialog.h */

#ifndef FEATURESELECTIONDIALOG_H
#define FEATURESELECTIONDIALOG_H

// #include <QtWidgets/QDialog>

#include <ddcutil_c_api.h>

#include "feature_selector.h"
#include "mainwindow.h"

namespace Ui {
class FeatureSelectionDialog;
}

class FeatureSelectionDialog : public QDialog  // was QDialog
{
    Q_OBJECT

public:
    explicit FeatureSelectionDialog(QWidget *parent, FeatureSelector* featureSelector);   // was QWidget
    ~FeatureSelectionDialog();

private slots:
    void on_known_radioButton_clicked(bool checked);

    void on_known_radioButton_clicked();

    void on_scan_radioButton_clicked();

    void on_mfg_RadioButton_clicked();

    void on_profile_RadioButton_clicked();

    void on_color_radioButton_clicked();

    void on_show_unsupported_checkBox_stateChanged(int arg1);

    void on_include_table_checkBox_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::FeatureSelectionDialog *ui;

    void setFeatureSet(int fsid);

    DDCA_Feature_Subset_Id _local_fsid;

    MainWindow * _mainWindow;

    FeatureSelector * _feature_selector = nullptr;
};

#endif // FEATURESELECTIONDIALOG_H
