// help_dialog.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HELP_DIALOG_H_
#define HELP_DIALOG_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

class QTextBrowser;

void viewHelp(QString simpleFn, QString title,  QWidget * parent = nullptr);
void viewHelpByText(QString text, QString title,  QWidget * parent = nullptr);
void viewHelpByTextX(QString text, QString title, QFont font, QWidget *parent=nullptr);

class HelpDialog2 : public QDialog {

    Q_OBJECT

public:
    explicit HelpDialog2(QWidget *parent = 0);
    HelpDialog2( QString title, QString& htmlText, QWidget *parent = 0);

#ifdef UNFINISHED
    HelpDialog2(char * title, char * simpleFn, QWidget* parent) {
#endif

    ~HelpDialog2();

    void setText(QString& htmlText);

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
