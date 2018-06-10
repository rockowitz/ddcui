/* value_nc_widget.cpp */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </copyright>
 */

#include "feature_value_widgets/value_reset_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

#include <ddcutil_c_api.h>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"


static bool dimensionReportShown = false;

ValueResetWidget::ValueResetWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
   _cls = strdup(metaObject()->className());
   // PRINTFCM("Starting." );

   QFont nonMonoFont;
   nonMonoFont.setPointSize(8);

   _resetButton = new QPushButton("Restore");
   _resetButton->setMaximumSize(60,20);

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _resetButton->setSizePolicy(*sizePolicy);
    _resetButton->setFont(nonMonoFont);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    _resetButton->setStyleSheet("background-color:white;");

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_resetButton);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (debugLayout) {
       this->setStyleSheet("background-color:cyan;");
       if (!dimensionReportShown) {
           // printf("-------------------------------------------->\n"); fflush(stdout);
           reportWidgetDimensions(_resetButton, _cls, __func__, "ValueResetWidget::_resetButton");
           reportWidgetDimensions(this, _cls, __func__, "ValueResetWidget");
           dimensionReportShown = true;
       }
    }

    // QObject::connect(_resetButton,  &QAbstractButton::released),
    //                  this,          &ValueResetWidget::on_resetButton_pressed );

    QObject::connect(_resetButton,  SIGNAL(released()),
                     this,          SLOT(  on_resetButton_pressed()) );

    delete(sizePolicy);
}


void ValueResetWidget::on_resetButton_pressed() {
   // PRINTFCMF(debugValueWidgetSignals, "Button pressed. Emitting featureValueChanged()");
   emit featureValueChanged(_featureCode, 0, 1);
}

