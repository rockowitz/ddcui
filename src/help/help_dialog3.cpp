// help_dialog3.cpp
// Variant of HelpDialog that uses QTCreator to create dialog box
// Incomplete - Do not use

#include "help_dialog3.h"
#include "ui_help_dialog3.h"

HelpDialog3::HelpDialog3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog3)
{
    ui->setupUi(this);
}

HelpDialog3::~HelpDialog3()
{
    delete ui;
}
