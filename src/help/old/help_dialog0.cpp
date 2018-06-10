#include "help_dialog0.h"
#include "ui_help_dialog0.h"

HelpDialog0::HelpDialog0(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog0)
{
    ui->setupUi(this);
}

HelpDialog0::~HelpDialog0()
{
    delete ui;
}
