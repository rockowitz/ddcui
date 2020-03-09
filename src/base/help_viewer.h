// help_viewer.c

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later



#ifndef HELP_VIEWER_C_
#define HELP_VIEWER_C_

#include <QtCore/QtCore>
#include <QtWidgets/QDialog>

class QTextBrowser;


namespace HelpViewer {
void show(QString helpfile, QString title);
};


#endif /* HELP_VIEWER_C_ */
