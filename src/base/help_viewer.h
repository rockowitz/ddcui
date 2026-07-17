/** @file help_viewer.h */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// NOTE: Retained for reference only; not built.  HelpViewer is declared here as
// a namespace, but help_viewer.cpp defines show() as though it were a member
// (uses 'this') and references an undefined HelpDialog2, so it will not compile.
// The classes actually used are HelpDialog (help/help_dialog.h) and HelpBrowser
// (help/help_browser.h).

#ifndef HELP_VIEWER_H_
#define HELP_VIEWER_H_

#include <QtCore>
#include <QDialog>

class QTextBrowser;

namespace HelpViewer {
void show(QString helpfile, QString title);
};

#endif /* HELP_VIEWER_H_ */
