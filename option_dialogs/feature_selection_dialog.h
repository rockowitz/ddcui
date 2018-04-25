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
    void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);


private slots:
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
    void on_buttonBox_accepted();

private:
#ifdef UNUSED
    void setFeatureSet(int fsid);
#endif

    Ui::FeatureSelectionDialog* ui;
    FeatureSelector*            _featureSelector;
    // MainWindow *                _mainWindow;
    // DDCA_Feature_Subset_Id      _local_fsid;

    // FeatureSelector * _feature_selector = nullptr;
};

#endif // FEATURESELECTIONDIALOG_H
