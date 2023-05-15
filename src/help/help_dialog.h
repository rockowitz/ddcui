/** \file help_dialog.h */

// Copyright (C) 2018-2023 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HELP_DIALOG_H_
#define HELP_DIALOG_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtCore/QUrl>

class QTextBrowser;


class HelpDialog : public QDialog {

    Q_OBJECT

public:

   static void viewResourceHelp(QString simpleFn, QString title,  QWidget * parent = nullptr);
   static void viewHelpByText(QString text, QString title,  QWidget * parent = nullptr);
   static void viewHelpByTextX(QString text, QString title, QFont font, QWidget *parent=nullptr);

    explicit HelpDialog(QWidget *parent = 0);
    HelpDialog( QString title, QString& htmlText, QWidget *parent = 0);

#ifdef UNFINISHED
    HelpDialog(char * title, char * simpleFn, QWidget* parent) {
#endif

    ~HelpDialog();

    void setText(QString& htmlText);
    void setSource(const char * source);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void updateWindowTitle();

private:  // methods
    void createWidgets();
    void layoutWidgets();
    void connectWidgets();
    void commonInit();

private:  // member variables
    char *             _cls;
    QTextBrowser*      _textBrowser;
    QDialogButtonBox*  _buttons;
};

#endif /* HELP_DIALOG_H_ */
