// help_viewer.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "help_viewer.h"

void HelpViewer::show(QString &simpleFn, QString& title) {
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
