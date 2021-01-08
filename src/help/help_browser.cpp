// help_browser.cpp

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

 // Adapted from C++ GUI Programming With Qt 4

#include <QtCore/QObject>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "base/core.h"
#include "help/help_browser.h"


void HelpBrowser::showPage(const QString& page, bool navigable) {
   HelpBrowser * browser = new HelpBrowser(page, navigable);
   browser->resize(500,400);
   browser->show();
}


void HelpBrowser::createWidgets(bool navigable) {
   _textBrowser = new QTextBrowser;
   if (navigable) {
      _homeButton  = new QPushButton("&Home");
      _backButton  = new QPushButton("&Back");
   }
   _closeButton = new QPushButton("Close");
   _closeButton->setShortcut(tr("Esc"));
}


void HelpBrowser::layoutWidgets(bool navigable) {
   QHBoxLayout* buttonLayout = new QHBoxLayout();
   if (navigable) {
      buttonLayout->addWidget(_homeButton);
      buttonLayout->addWidget(_backButton);
   }
   buttonLayout->addStretch();
   buttonLayout->addWidget(_closeButton);

   QVBoxLayout* mainLayout = new QVBoxLayout();
   mainLayout->addLayout(buttonLayout);
   mainLayout->addWidget(_textBrowser);
   setLayout(mainLayout);
}


HelpBrowser::HelpBrowser(
      const QString& page,
      bool           navigable,
      QWidget*       parent)
    : QWidget(parent)
{
   TRACEM("Starting. page=%s, navigable=%s", QS2S(page), SBOOL(navigable));
   createWidgets(navigable);
   layoutWidgets(navigable);

   setAttribute(Qt::WA_DeleteOnClose);
   setAttribute(Qt::WA_GroupLeader);

   if (navigable) {
      QObject::connect(_homeButton, &QPushButton::clicked, _textBrowser, &QTextBrowser::home);
      QObject::connect(_backButton, &QPushButton::clicked, _textBrowser, &QTextBrowser::backward);
   }
   QObject::connect(   _closeButton, &QPushButton::clicked,        this, &QTextBrowser::close);
   QObject::connect(   _textBrowser, &QTextBrowser::sourceChanged, this, &HelpBrowser::updateWindowTitle);

   _textBrowser->setSource(page);
}


void HelpBrowser::updateWindowTitle()
{
   setWindowTitle(tr("Help: %1").arg(_textBrowser->documentTitle()));
}

