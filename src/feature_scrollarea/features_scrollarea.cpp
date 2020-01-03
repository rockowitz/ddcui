// features_scrollarea.cpp

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


#include <QtWidgets/QScrollArea>
#include <QtCore/QSize>


#include "feature_scrollarea/features_scrollarea.h"

#include "../base/core.h"
#include "../base/core.h"
#include "../base/widget_debug.h"
#include "feature_scrollarea/features_scrollarea_contents.h"


static bool showWidgetDimensions = false;
static bool traceResizeEvents = false;

void FeaturesScrollArea::layoutWidget() {

   if (debugLayout) {
      setStyleSheet("background-color:maroon;");

      static bool widgetDimensionsReported = false;
      if (showWidgetDimensions && !widgetDimensionsReported) {
         reportWidgetDimensions(this, _cls, __func__, "FeatureScrollAreaContents dimensions");
      }
   }
}


FeaturesScrollArea::FeaturesScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
   _cls = metaObject()->className();
   // TRACE("Executing");
   layoutWidget();
}

FeaturesScrollArea::~FeaturesScrollArea()
{
}


void FeaturesScrollArea::resizeEvent(QResizeEvent * evt)
{
   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();
   QWidget * contents = widget();
   QSize contentsSize = contents->size();
   // TODO: get calculate the actual scrollbar width
   QSize newSize = QSize(contentsSize.width()-24, contentsSize.height());
   contents->resize(newSize);
   evt->ignore();

   if (traceResizeEvents) {
      TRACEC("old size = %d, %d", oldSz.width(), oldSz.height());
      TRACEC("new size = %d, %d", newSz.width(), newSz.height());
      TRACEC("Current contents size %d,%d", contentsSize.width(), contentsSize.height());
      TRACEC("Resizing contents to  %d,%d", newSize.width(), newSize.height());
   }
}

