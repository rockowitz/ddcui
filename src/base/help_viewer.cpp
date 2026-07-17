/** @file help_viewer.cpp */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// NOTE: Retained for reference only.  This file is NOT built (not listed in
// CMakeLists.txt) and would not compile as-is:
//   - help_viewer.h declares HelpViewer as a namespace, but show() below uses
//     'this', which is valid only in a member function.
//   - HelpDialog2 is not defined anywhere (cf. HelpDialog in help/help_dialog.h).
// See help/help_dialog.h and help/help_browser.h for the classes actually used.

#include "help_viewer.h"

void HelpViewer::show(QString simpleFn, QString title) {
   QString fn = simpleFn.prepend(":/docs/");
   QFile f(fn);
   f.open(QFile::ReadOnly | QFile::Text);
   QTextStream in(&f);
   QString htmlText = in.readAll();
   // qDebug() << htmlText;

   HelpDialog2* hd = new HelpDialog2(this);
   hd->setText(htmlText);
      // hd->_textBrowser->setSource(fn);
   hd->setWindowTitle( title );
   hd->show();
}
