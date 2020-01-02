// feature_widget_header.h

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURE_WIDGET_HEADER_H_
#define FEATURE_WIDGET_HEADER_H_

#include <QtWidgets/QWidget>
#include <QtGui/QResizeEvent>

#include "../base/core.h"

class FeatureWidgetHeader : public QFrame {
   Q_OBJECT

public:
   explicit FeatureWidgetHeader(QWidget * parent = nullptr);
   virtual ~FeatureWidgetHeader();

protected:
    void resizeEvent(QResizeEvent * event) override;

private:
    void layoutWidget();

private:     // member variables
    const char * _cls;

};

#endif /* FEATURE_WIDGET_HEADER_H_ */
