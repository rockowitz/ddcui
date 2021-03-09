// help_browser.h

// Copyright (C) 2020-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextBrowser>

#ifndef HELP_BROWSER_H_
#define HELP_BROWSER_H_


class HelpBrowser : public QWidget
{
   Q_OBJECT

public:
   HelpBrowser(const QString& page, bool navigable, QWidget* parent = 0);

   static void showPage(const QString& page, bool navigable);

private slots:
   void updateWindowTitle();

protected:
   void keyPressEvent(QKeyEvent * ev) override;

private:   //methods
   void createWidgets(bool navigable);
   void layoutWidgets(bool navigable);


private:
   QTextBrowser * _textBrowser;
   QPushButton *  _homeButton;
   QPushButton *  _backButton;
   QPushButton *  _closeButton;
   QDialogButtonBox * _buttonBox;

};

#endif /* HELP_BROWSER_H_ */
