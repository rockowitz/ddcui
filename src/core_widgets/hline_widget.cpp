// hline_widget.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "hline_widget.h"

HLineWidget::HLineWidget(QWidget * parent )
: QFrame(parent)
{
   _cls = metaObject()->className();

   setFrameStyle(QFrame::HLine);

   QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

   setSizePolicy(sizePolicy);
   setMaximumHeight(1);
   setLineWidth(1);
   // setFrameWidth(1);
}

HLineWidget::~HLineWidget() {
   // TODO Auto-generated destructor stub
}

