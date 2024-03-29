/** \file value_new_cont_widget.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "base/ddcui_parms.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>

#include "base/ddcui_parms.h"

#include "base/widget_debug.h"

#include "c_util/string_util.h"

#include "feature_value_widgets/value_new_cont_widget.h"
#include "../base/ddcui_core.h"


void ValueNewContWidget::layoutWidget(QHBoxLayout * layout) {
    bool debug = false;
    TRACECF(debug, " Starting." );

    // createWidgets();   // done in ValueSimpleContWidget constructor
    // additionalWidgets();  // done in our constructor

    layout->addSpacing(5);
    layout->addWidget(_spinSlider);

    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);

    layout->addSpacing(5+10);
    layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
    layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
    setLayout(layout);

    TRACECF(debug, "Done." );
}


ValueNewContWidget::ValueNewContWidget(QWidget *parent)
    : ValueSimpleContWidget(parent)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACEMCF(debug, "TRACECMF. Executing. After ValueSimpleContWidget constructor, _id=%d, _featureCode=0x%02x", _id, _featureCode);
}


ValueNewContWidget::~ValueNewContWidget() {
   bool debug = false;
   TRACEMCF(debug, "Executing. this._id = %d", _id);
   free((void*) _cls);
}


void ValueNewContWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    debug = debug ||debugValueWidgetSignals;

    int maxval = fv.val().mh << 8 | fv.val().ml;
    int curval = fv.val().sh << 8 | fv.val().sl;
    // maxval = 99999;   // for testing big numbers
    // curval = 99999;   // for testing big numbers
    TRACEMCF(debug, "Starting. _id=%d, feature=0x%02x, curval=%d, maxval=%d",
                    _id, fv.featureCode(), curval, maxval);

    _spinSlider->setRange(0,maxval);
    ValueSimpleContWidget::setFeatureValue(fv);
    _maxValue->setText(QString::number(maxval));

    TRACEMCF(debug, "Done");
}

