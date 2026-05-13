/** @file help_viewer.h */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HELP_VIEWER_H_
#define HELP_VIEWER_H_

#include <QtCore/QtCore>
#include <QtWidgets/QDialog>

class QTextBrowser;

namespace HelpViewer {
void show(QString helpfile, QString title);
};

#endif /* HELP_VIEWER_H_ */
