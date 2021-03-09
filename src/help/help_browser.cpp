// help_browser.cpp

// Copyright (C) 2020-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

 // Adapted from C++ GUI Programming With Qt 4

#include <QtCore/QObject>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "base/core.h"
#include "help/help_browser.h"

//
// Event debugging
//

static
void dbgrptQKeyEvent(QKeyEvent * event) {
   printf("  key:  %d\n", event->key());
   printf("  modifiers: 0x%x\n", (unsigned int) event->modifiers());
   printf("  nativeModifiers: 0x%08x\n", event->nativeModifiers());
   printf("  nativeScanCode:  0x%08x\n", event->nativeScanCode());
}



void HelpBrowser::showPage(const QString& page, bool navigable) {
   HelpBrowser * browser = new HelpBrowser(page, navigable);
   browser->resize(600,400);
   // browser->setMinimumSize(600,400);
   browser->show();
}


void HelpBrowser::createWidgets(bool navigable) {
   _textBrowser = new QTextBrowser;
   if (navigable) {
      _homeButton  = new QPushButton("&Home");
      _homeButton->setShortcut(tr("H"));
      _backButton  = new QPushButton("&Back");
      _backButton->setShortcut(tr("B"));
   }
   // _closeButton = new QPushButton(QDialogButtonBox::Close);  // doesn't compile
   _closeButton = new QPushButton("&Close");
   _closeButton->setShortcut(tr("Esc"));
   _closeButton->setShortcut(tr("C"));
   _closeButton->setDefault(true);
}


#ifdef FAILURE
void HelpBrowser::layoutWidgets(bool navigable) {
   _buttonBox = new QDialogButtonBox();
   // _buttonBox->addButton(_backButton,QDialogButtonBox::InvalidRole);
   // _buttonBox->addButton(_homeButton);
   _buttonBox->addButton("Close2", QDialogButtonBox::RejectRole);
   _closeButton = _buttonBox->addButton(QDialogButtonBox::Close);

#ifdef OLD
   QHBoxLayout* buttonLayout = new QHBoxLayout();
   if (navigable) {
      buttonLayout->addWidget(_backButton);
      buttonLayout->addWidget(_homeButton);
   }
   buttonLayout->addStretch();
   buttonLayout->addWidget(_closeButton);
#endif

   QVBoxLayout* mainLayout = new QVBoxLayout();

   mainLayout->addWidget(_textBrowser);
#ifdef OLD
   mainLayout->addLayout(buttonLayout);
#endif
   mainLayout->addWidget(_buttonBox);
   setLayout(mainLayout);
}
#endif

void HelpBrowser::layoutWidgets(bool navigable) {
   QHBoxLayout* buttonLayout = new QHBoxLayout();
   if (navigable) {
      buttonLayout->addWidget(_backButton);
      buttonLayout->addWidget(_homeButton);
   }
   buttonLayout->addStretch();
   buttonLayout->addWidget(_closeButton);

   QVBoxLayout* mainLayout = new QVBoxLayout();

   mainLayout->addWidget(_textBrowser);
   mainLayout->addLayout(buttonLayout);
   setLayout(mainLayout);
}


HelpBrowser::HelpBrowser(
      const QString& page,
      bool           navigable,
      QWidget*       parent)
    : QWidget(parent)
{
   // TRACEM("Starting. page=%s, navigable=%s", QS2S(page), SBOOL(navigable));
   createWidgets(navigable);
   layoutWidgets(navigable);

   setAttribute(Qt::WA_DeleteOnClose);
   setAttribute(Qt::WA_GroupLeader);
   // setWindowModality(Qt::ApplicationModal);   // choices: Qt::NonModal, Qt::WindowModal, Qt::ApplicationModal

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


void HelpBrowser::keyPressEvent(QKeyEvent *   ev)
{
   // dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Escape  ||
       ev->key() == Qt::Key_Return   ||
       ev->key() == Qt::Key_Enter)
   {
     close();
   }
   ev->ignore();
}


