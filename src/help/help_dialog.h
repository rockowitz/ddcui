/* help_dialog.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </copyright>
 */

#ifndef HELP_DIALOG_H_
#define HELP_DIALOG_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

// class QTextBrowser;


class HelpDialog2 : public QDialog {

    Q_OBJECT

public:
    explicit HelpDialog2(QWidget *parent = 0);
    HelpDialog2( QString title, QString& htmlText, QWidget *parent = 0);
    ~HelpDialog2();

    void setText(QString& htmlText);

private:
    void commonInit();

public:   // temp
    QTextBrowser* _textBrowser;
    QTextEdit *   _textEdit;



};

#endif /* HELP_DIALOG_H_ */
