#ifndef OTHER_OPTIONS_DIALOG_H
#define OTHER_OPTIONS_DIALOG_H

#include <QtWidgets/QDialog>

#include "base/other_options_state.h"

namespace Ui {
class OtherOptionsDialog;
}

class OtherOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OtherOptionsDialog(OtherOptionsState * state, QWidget *parent = 0);
    ~OtherOptionsDialog();

signals:
     void ncValuesSourceChanged(NcValuesSource valuesSource);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::OtherOptionsDialog *ui;

    OtherOptionsState * _state;
};

#endif // OTHER_OPTIONS_DIALOG_H
