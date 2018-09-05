// help_dialog.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HELP_DIALOG_H_
#define HELP_DIALOG_H_

#include <QtWidgets/QDialog>

class QTextBrowser;


class HelpDialog2 : public QDialog {

    Q_OBJECT

public:
    explicit HelpDialog2(QWidget *parent = 0);
    HelpDialog2( QString title, QString& htmlText, QWidget *parent = 0);
    ~HelpDialog2();

    void setText(QString& htmlText);

private:
    void commonInit();

    QTextBrowser* _textBrowser;
 // QTextEdit *   _textEdit;
};

#endif /* HELP_DIALOG_H_ */
