/** @file value_reset_widget.cpp
 *  Widget containing a Reset button
 */


// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_reset_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QPalette>
#include <QtGui/QRegion>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

#include <ddcutil_c_api.h>

#include "../base/ddcui_core.h"
#include "base/widget_debug.h"


static bool showDimensionReport = false;
static bool showBasicDims = false || debugFeatureDimensions;
static bool showResizeEvents = false;


void ValueResetWidget::layoutWidget() {
   QFont nonMonoFont;
   nonMonoFont.setPointSize(8);

   _resetButton = new QPushButton("Restore");
   _resetButton->setMaximumSize(60,20);

   QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   _resetButton->setSizePolicy(*sizePolicy);
   _resetButton->setFont(FeatureValueButtonFont);
   //   _resetButton->setStyleSheet("background-color:white;color:black;");
   _savedBackgroundColor = _resetButton->backgroundRole();

   QHBoxLayout * layout = new QHBoxLayout();
   layout->addSpacing(5);
   layout->addWidget(_resetButton);
   layout->addStretch(1);
   layout->setContentsMargins(0,0,0,0);
   setLayout(layout);

   delete(sizePolicy);

   if (debugLayout) {
      this->setStyleSheet("background-color:cyan;");

      static bool dimensionReportShown = false;
      if (showDimensionReport && !dimensionReportShown) {
         // printf("-------------------------------------------->\n"); fflush(stdout);
         reportWidgetDimensions(_resetButton, _cls, __func__, "ValueResetWidget::_resetButton");
         reportWidgetDimensions(this, _cls, __func__, "ValueResetWidget");
         dimensionReportShown = true;
      }
   }

   static bool basicDimsShown = false;
   if (showBasicDims && !basicDimsShown) {
       REPORT_BASIC_WIDGET_DIMENSIONS(this);
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_resetButton);

       QObjectList  childs = this->children();
       for (int ndx = 0; ndx < childs.size(); ndx++) {
           QObject* curobj = childs.at(ndx);
           QString name   = curobj->objectName();
           const char *  clsName = curobj->metaObject()->className();
           printf("   Child: %s, type:%s\n", name.toLatin1().data(), clsName);
       }
       fflush(stdout);
       basicDimsShown = true;
    }
}


ValueResetWidget::ValueResetWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
   bool debug = false;
   _cls = strdup(metaObject()->className());
   TRACEMCF(debug, "Starting. _id=%d, _featureCode=0x%02x", _id, _featureCode );
   layoutWidget();

   // QObject::connect(_resetButton,  &QAbstractButton::released),
   //                  this,          &ValueResetWidget::on_resetButton_pressed );

   QObject::connect(_resetButton,  SIGNAL(released()),
                    this,          SLOT(  on_resetButton_pressed()) );
   TRACEMCF(debug, "Done.");
}


ValueResetWidget::~ValueResetWidget() {
   free((void*) _cls);
}


void ValueResetWidget::setEnabled(bool onoff) {
   bool debug = false;
   TRACEMCF(debug, "Starting. onoff=%s", SBOOL(onoff));
   ValueBaseWidget::setEnabled(onoff);
#ifdef OUT
   if (onoff) {
      _resetButton->setBackgroundRole(_savedBackgroundColor);
   }
   else {
      _resetButton->setBackgroundRole(QPalette::Dark);
   }
#endif
   TRACEMCF(debug, "Done.");
}


void ValueResetWidget::on_resetButton_pressed() {
   bool debug = false;
   TRACEMCF(debug, "Starting. _id=%d, _featureCode=0x%02x, isEnabled=%s",
                   _id, _featureCode, SBOOL(ValueBaseWidget::isEnabled()));


   TRACEMCF(debug || debugValueWidgetSignals, "Button pressed. Emitting featureValueChanged()");
   emit featureValueChanged(_featureCode, 0, 1);
}


void ValueResetWidget::resizeEvent(QResizeEvent * evt)
{
   bool show = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool resizeShown = false;
   if (showResizeEvents && !resizeShown) {
      show = true;
      resizeShown = true;
   }

#ifdef ALT
   int oldWidth = oldSz.width();
   int oldHeight = oldSz.height();
   int newWidth = newSz.width();
   int newHeight = newSz.width();
   if (oldHeight != newHeight || oldWidth != newWidth) {
      show = true;
   }
#endif

   if (show) {
      TRACEC("old size = %d, %d", oldSz.width(), oldSz.height());
      TRACEC("new size = %d, %d", newSz.width(), newSz.height());
   }

   evt->ignore();
}
