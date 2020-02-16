// features_scroll_area_contents.cpp - Contains rows of FeatureWidgets

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_scrollarea/features_scrollarea_contents.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtCore/QSize>

#include "base/core.h"
#include "base/widget_debug.h"

#include "feature_value_widgets/value_stacked_widget.h"
#include "feature_scrollarea/feature_widget.h"


static bool debugWidget = false;
static bool showWidgetDimensions = false;
static bool traceResizeEvents = false;


FeaturesScrollAreaContents::FeaturesScrollAreaContents(QWidget * parent) :
    QWidget(parent)
{
   bool debug = false;
   _cls = metaObject()->className();

  //  _pageChangeObservers = new QVector<PageChangeObserver*>;

   // setLayout(new QVBoxLayout());

   // check that signal being fired
   // connect(this, SIGNAL(showCentralWidgetByWidget(QWidget *)),
   //         qApp, SLOT(aboutQt()));

   // layout()->setContentsMargins(0,0,0,0); // no layout defined

   // doesn't expand the contents area
   //setSizePolicy( QSizePolicy::MinimumExpanding,  QSizePolicy::MinimumExpanding);
   TRACECF(debug, "Executing");

   if (debugLayout) {
      setStyleSheet("background-color:aqua;");

      static bool widgetDimensionsReported = false;
      if (showWidgetDimensions && !widgetDimensionsReported) {
         reportWidgetDimensions(this, _cls, __func__, "FeatureScrollAreaContents dimensions");
      }
   }
}


FeaturesScrollAreaContents::~FeaturesScrollAreaContents() {
   bool debug = false;
   TRACECF(debug, "Executing");
   // TODO IMPLEMENT!!!
}


void FeaturesScrollAreaContents::setModel(FeatureBaseModel * baseModel) {
   _baseModel = baseModel;
}


void FeaturesScrollAreaContents::setContainingScrollArea(QScrollArea * scrollArea) {
   _containingScrollArea = scrollArea;
   // no, just adds horizontal scrollbar on bottom
   // scrollArea->setWidgetResizable(true);
}

#ifdef UNUSED
void FeaturesScrollAreaContents::featureAdded(FeatureValue fv)
{
   if (debugMain) {
       printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
       // fv.report();
   }

   FeatureWidget * newTableEntry = new FeatureWidget();
   newTableEntry->setFeatureValue(fv);

   if (debugMain)
       newTableEntry->dbgrpt();

   layout()->addWidget(newTableEntry);
   _widgets[fv._feature_code] = newTableEntry;
}
#endif


#ifdef OLD
// Unused!
void FeaturesScrollAreaContents::featureUpdated(char feature_code)
{
   if (debugMain)
       printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
   // get the current value for the feature

   FeatureValue * fv  = _baseModel->modelVcpValueFind(feature_code);

   // find the entry in _widgets
   _widgets[feature_code];


   // set value in the widget
}
#endif


void FeaturesScrollAreaContents::startInitialLoad(void)
{
   bool debugFunc = debugWidget;
   debugFunc = false;
   TRACECF(debugFunc, "Executing");
   assert(_containingScrollArea != nullptr);
   // _containingScrollArea->hide();

}

void FeaturesScrollAreaContents::endInitialLoad()
{
   bool debugFunc = debugWidget;
   debugFunc = false;
   TRACECF(debugFunc, "Executing");
   assert(_containingScrollArea);

   emit showCentralWidgetByWidget(_containingScrollArea);
}


#ifdef PAGE_CHANGE_OBSERVER
void FeaturesScrollAreaContents::addPageChangeObserver(PageChangeObserver * observer) {
   _pageChangeObservers->append(observer);
}


void FeaturesScrollAreaContents::notifyPageChangeObservers(int pageno) {
   int ct = _pageChangeObservers->count();
   TRACECF(debugSignals, "Starting ct=%d", ct);
   for (int ndx = 0; ndx < ct; ndx++) {
       TRACECF(debugSignals, "Notifying observer");
       PageChangeObserver*  observer = _pageChangeObservers->at(ndx);
       observer->pageChangedByWidget(_containingScrollArea);
   }
}
#endif


void FeaturesScrollAreaContents::resize(int w, int h)
{
   TRACEC("width = %d, height = %s", w, h);
   QWidget::resize(w, h);
}
void FeaturesScrollAreaContents::resize(QSize sz)
{
   TRACEC("width = %d, height = %s", sz.width(), sz.height());
   QWidget::resize(sz);
}


void FeaturesScrollAreaContents::resizeEvent(QResizeEvent * evt)
{
   if (traceResizeEvents) {
      QSize oldSz = evt->oldSize();
      QSize newSz = evt->size();

      TRACEC("old size = %d, %d   new size = %d, %d",
              oldSz.width(), oldSz.height(), newSz.width(), newSz.height());
   }
   evt->ignore();
}

int FeaturesScrollAreaContents::maxRowHeight() {
   return 0;
}


