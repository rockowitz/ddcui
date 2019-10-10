// number_entry_widget.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QLineEdit>
#include <QtGui/QValidator>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "number_entry_widget.h"
 

NumberEntryWidget::NumberEntryWidget(
      uint8_t bytect,
      QFont   font,
      int     style,
      QSize   size,
      QWidget *parent)
    : QLineEdit(nullptr)
    , _bytect(bytect)
{
    // how ensure bytect between 1 and 4?

    QString rxs;

    switch(bytect) {
    case 1:  rxs = QString("^[xX][A-Fa-f0-9](1)$");    break;

    // case 2:
    // case 3:
    // case 4:
    default:
       // how to fail?
    throw std::runtime_error("Invalid bytect");
    }

    QRegExp rx = QRegExp(rxs);

    setValidator( new QRegExpValidator(rx));

    setMaxLength( bytect+1);

    setFont(font);
    // setFrameStyle(style); ???
    setFixedSize(size);
    setAlignment(Qt::AlignRight);

    if (debugLayout)
       setStyleSheet("background-color:orange;");

}



