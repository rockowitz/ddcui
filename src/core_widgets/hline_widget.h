// hline_widget.h - creates a horizonal line

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HLINE_WIDGET_H_
#define HLINE_WIDGET_H_

#include <QtWidgets/QWidget>

#include "../base/core.h"

class HLineWidget  : public QFrame {
   Q_OBJECT

public:
   explicit HLineWidget(QWidget * parent = nullptr);
   virtual ~HLineWidget();

private:
   const char * _cls;
};

#endif /* HLINE_WIDGET_H_ */
