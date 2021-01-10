// help_dialog.cpp

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "base/core.h"
#include "help_dialog.h"


void HelpDialog::commonInit() {
   bool navigable = true;
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);

    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    _textBrowser = new QTextBrowser;
    // _textBrowser->setFontPointSize(20.0);
    _textBrowser->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_textBrowser);
    _buttons = new QDialogButtonBox(QDialogButtonBox::Close);
    mainLayout->addWidget(_buttons);
    setLayout(mainLayout);
    setMinimumSize(600,400);

    // QMetaObject::connect_slots_by_name(this);
    QObject::connect(
           _buttons, &QDialogButtonBox::rejected,
           this,          &HelpDialog::on_buttonBox_rejected);
    // There's only a Cancel button
    // QObject::connect(
    //        _buttons, &QDialogButtonBox::accepted,
    //        this,          &HelpDialog2::on_buttonBox_accepted);
}


HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent)
    ,_cls(strdup(metaObject()->className() ))
{
    commonInit();
}


HelpDialog::HelpDialog(QString title, QString& htmlText, QWidget* parent)
    : QDialog(parent)
    ,_cls(strdup(metaObject()->className()) )
{
    TRACEC("title=%s, htmlText=%s", QS2S(title), QS2S(htmlText));
    commonInit();
    setWindowTitle(title);
    _textBrowser->setText(htmlText);
    show();   // not showing - why?
}

#ifdef UNFINISHED
HelpDialog::HelpDialog(char * simpleFn, char * title, QWidget* parent)
   : QDialog(parent)
   ,_cls(strdup(metaObject()->className()) )
{
   commonInit();
   setWindowtitle(title);
}
#endif



HelpDialog::~HelpDialog() {
   // TODO Auto-generated destructor stub
}


void HelpDialog::setText(QString& htmlText) {
   _textBrowser->setText(htmlText);
}

void HelpDialog::setSource(QString& source) {
   _textBrowser->setSource(source);
}


// doesn't occur - there's only a Cancel button
// but referenced in generated UI code
void HelpDialog::on_buttonBox_accepted() {
   TRACEC("Executing");
}

void HelpDialog::on_buttonBox_rejected() {
   // TRACEC("Executing");
   this->close();
}


void viewHelpByText(QString text, QString title, QWidget * parent) {

   HelpDialog* hd = new HelpDialog(parent);
   hd->setText(text);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( title );
   hd->exec();
}


void viewHelpByTextX(QString text, QString title, QFont font,QWidget * parent) {

   HelpDialog* hd = new HelpDialog(parent);
   hd->setFont(font);
   hd->setText(text);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( title );
   hd->exec();     // always modal
}





void viewHelp(QString simpleFn, QString title, QWidget * parent) {
   // QString fn = QString(simpleFn).prepend(":/docs/");
   QString fn = QString(":/docs/").append(simpleFn);
   QFile f(fn);
   f.open(QFile::ReadOnly | QFile::Text);
   QTextStream in(&f);
   QString htmlText = in.readAll();

   // qDebug() << htmlText;

   HelpDialog* hd = new HelpDialog(parent);
   hd->setText(htmlText);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( title );
   hd->exec();
   // delete hd;
}



