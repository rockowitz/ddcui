#ifndef HELP_DIALOG3_H
#define HELP_DIALOG3_H

#include <QDialog>

namespace Ui {
class HelpDialog3;
}

class HelpDialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog3(QWidget *parent = nullptr);
    ~HelpDialog3();

private:
    Ui::HelpDialog3 *ui;
};

#endif // HELP_DIALOG3_H
