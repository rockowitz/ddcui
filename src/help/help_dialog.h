/** \file help_dialog.h */

// Copyright (C) 2018-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HELP_DIALOG_H_
#define HELP_DIALOG_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

class QTextBrowser;

void viewHelp(QString simpleFn, QString title,  QWidget * parent = nullptr);
void viewHelpByText(QString text, QString title,  QWidget * parent = nullptr);
void viewHelpByTextX(QString text, QString title, QFont font, QWidget *parent=nullptr);

class HelpDialog : public QDialog {

    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    HelpDialog( QString title, QString& htmlText, QWidget *parent = 0);

#ifdef UNFINISHED
    HelpDialog(char * title, char * simpleFn, QWidget* parent) {
#endif

    ~HelpDialog();

    void setText(QString& htmlText);
    void setSource(QString& source);

private slots:
        void on_buttonBox_accepted();
        void on_buttonBox_rejected();

private:
    void commonInit();

    char * _cls;

    QTextBrowser* _textBrowser;
 // QTextEdit *   _textEdit;
    QDialogButtonBox*   _buttons;



};

#endif /* HELP_DIALOG_H_ */
