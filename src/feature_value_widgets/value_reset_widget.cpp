/* value_nc_widget.cpp */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

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

#include "base/core.h"
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
   // _cb->setHorizontalStretch(0);
   _resetButton->setSizePolicy(*sizePolicy);
   _resetButton->setFont(FeatureValueButtonFont);
   // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
   // _resetButton->setStyleSheet("background-color:white;");
   _resetButton->setStyleSheet("background-color:white;color:black;");

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
   _cls = strdup(metaObject()->className());
   // TRACE("Starting." );
   layoutWidget();

   // QObject::connect(_resetButton,  &QAbstractButton::released),
    //                  this,          &ValueResetWidget::on_resetButton_pressed );

    QObject::connect(_resetButton,  SIGNAL(released()),
                     this,          SLOT(  on_resetButton_pressed()) );
}


void ValueResetWidget::on_resetButton_pressed() {
   // TRACEF(debugValueWidgetSignals, "Button pressed. Emitting featureValueChanged()");
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

