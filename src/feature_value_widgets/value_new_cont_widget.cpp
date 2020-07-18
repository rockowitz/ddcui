/* value_new_cont_widget.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>

#include <QtWidgets/QWidget>
// #include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QtGui/QKeyEvent>
#include <QtGui/QFocusEvent>
#include <QtCore/QEvent>

#include <QtWidgets/QHBoxLayout>

#include "base/ddcui_parms.h"
#include "base/core.h"
#include "base/widget_debug.h"

#include "c_util/string_util.h"

#include "core_widgets/enhanced_slider.h"

#include "feature_value_widgets/value_new_cont_widget.h"


void ValueNewContWidget::additionalWidgets() {
   bool debug = false;
   TRACECF(debug, " ValueNewContWidget. Starting." );
   // max value fields

   _maxTitle = new QLabel("Max:");
   _maxTitle->setFixedSize(30,18);
   _maxTitle->setFont(FeatureValueTextFont);
   _maxTitle->setContentsMargins(5,0,0,0);
   if (debugLayout)
      _maxTitle->setStyleSheet("background-color:cyan;");

   _maxValue = new QLabel();
   _maxValue->setFont(FeatureValueTextFont);
   // _maxValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);
   _maxValue->setFrameStyle(QFrame::Plain | QFrame::NoFrame);
   _maxValue->setFixedSize(35,20);
   _maxValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
   if (debugLayout)
      _maxValue->setStyleSheet("background-color:orange;");
   TRACECF(debug, " ValueNewContWidget. Done." );
}


void ValueNewContWidget::layoutWidget(QHBoxLayout * layout) {
   bool debug = false;
   TRACECF(debug, " ValueNewContWidget. Starting." );

   // createWidgets();   // done in ValueSimpleContWidget constructor
   // additionalWidgets();  // done in our constructor

   // ValueSimpleContWidget::layoutWidget(layout);

    layout->addSpacing(5);
    layout->addWidget(_curSlider);

    layout->addWidget(_curSpinBox);
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);

    layout->addSpacing(5+10);
    // layout->addWidget(spacer);
    layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
    layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
    setLayout(layout);

    TRACECF(debug, " ValueNewContWidget. Done." );
}


ValueNewContWidget::ValueNewContWidget(QWidget *parent)
    : ValueSimpleContWidget(0,255,parent)
//    , _newval(0)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACEMCF(debug, "Starting." );

    delete layout();   // delete layout created by superclass constructor
    additionalWidgets();
    QHBoxLayout * layout = new QHBoxLayout();
    layoutWidget(layout);

    TRACEMCF(debug, "TRACECMF.  After ValueSimpleContWidget constructor");
}


void ValueNewContWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = debugValueWidgetSignals;
    // debug = true;
    TRACEMCF(debug, "Starting. feature code: 0x%02x, before ValueBaseWidget::setFeatureValue()", fv.featureCode());
    ValueBaseWidget::setFeatureValue(fv);

    int maxval = _mh << 8 | _ml;
    int curval = _sh << 8 | _sl;

    TRACEMCF(debug,
              "feature=0x%02x, curval=%d, maxval=%d", _featureCode, curval, maxval);

    // maxval = 99999;   // for testing big numbers
    // curval = 99999;
    _guiChange = false;

    _curSlider->setTickInterval(maxval/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(maxval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(maxval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

    QString s = QString::number(maxval);
    _maxValue->setText(s);

    _guiChange = true;
    TRACEMCF(debug, "Done");
}


