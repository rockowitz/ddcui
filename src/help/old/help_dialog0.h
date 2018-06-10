#ifndef HELP_DIALOG0_H
#define HELP_DIALOG0_H

#include <QtWidgets/QDialog>

namespace Ui {
class HelpDialog0;
}

class HelpDialog0 : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog0(QWidget *parent = 0);
    ~HelpDialog0();

private:
    Ui::HelpDialog0 *ui;
};

#endif // HELP_DIALOG0_H
