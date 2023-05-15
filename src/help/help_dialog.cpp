// help_dialog.cpp

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "base/ddcui_core.h"

#include "help_dialog.h"


void HelpDialog::createWidgets() {
   _textBrowser = new QTextBrowser;
   _textBrowser->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   _textBrowser->setContextMenuPolicy(Qt::PreventContextMenu);
   _buttons = new QDialogButtonBox(QDialogButtonBox::Ok);
}

void HelpDialog::layoutWidgets() {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);

    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(_textBrowser);
    mainLayout->addWidget(_buttons);
    setLayout(mainLayout);
    setMinimumSize(600,400);
}


void HelpDialog::connectWidgets() {
    // QMetaObject::connect_slots_by_name(this);
    QObject::connect(_buttons, &QDialogButtonBox::rejected, this, &HelpDialog::on_buttonBox_rejected);
    QObject::connect(_buttons, &QDialogButtonBox::accepted, this, &HelpDialog::on_buttonBox_accepted);
    QObject::connect(_textBrowser, &QTextBrowser::sourceChanged, this, &HelpDialog::updateWindowTitle);
}


void HelpDialog::commonInit() {
   createWidgets();
   layoutWidgets();
   connectWidgets();
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
   free(_cls);
   delete _buttons;
   delete _textBrowser;
}


void HelpDialog::setText(QString& htmlText) {
   _textBrowser->setText(htmlText);
}

void HelpDialog::setSource(const char * source) {
   // TRACEC("Source: %s", source);
   _textBrowser->setSource(QUrl(source));
}


// doesn't occur - there's only a Cancel button
// but referenced in generated UI code
void HelpDialog::on_buttonBox_accepted() {
   // TRACEC("Executing");
   this->close();
}


void HelpDialog::on_buttonBox_rejected() {
   // TRACEC("Executing");
   this->close();
}


//
// Static methods
//

void HelpDialog::viewHelpByText(QString text, QString title, QWidget * parent) {

   HelpDialog* hd = new HelpDialog(parent);
   hd->setText(text);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( title );
   hd->exec();
}


void HelpDialog::viewHelpByTextX(QString text, QString title, QFont font,QWidget * parent) {

   HelpDialog* hd = new HelpDialog(parent);
   hd->setFont(font);
   hd->setText(text);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( title );
   hd->exec();     // always modal
}


void HelpDialog::viewResourceHelp(QString simpleFn, QString title, QWidget * parent) {
   // QString fn = QString(simpleFn).prepend(":/docs/");
   QUrl fn = QString("qrc:/docs/").append(simpleFn);
#ifdef OLD
   QFile f(fn);
   f.open(QFile::ReadOnly | QFile::Text);
   QTextStream in(&f);
   QString htmlText = in.readAll();

   // qDebug() << htmlText;
   HelpDialog* hd = new HelpDialog(parent);
   hd->setText(htmlText);
#endif

   HelpDialog* hd = new HelpDialog(parent);
   hd->_textBrowser->setSource(fn);
   // hd->setWindowTitle( title );
   hd->exec();
   // delete hd;
}



void HelpDialog::updateWindowTitle()
{
setWindowTitle(tr("Help: %1").arg(_textBrowser->documentTitle()));
}




