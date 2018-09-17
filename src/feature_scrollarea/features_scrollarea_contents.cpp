/* features_scroll_area_contents.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_scrollarea/features_scrollarea_contents.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "feature_value_widgets/value_stacked_widget.h"
#include "feature_scrollarea/feature_widget.h"


static bool debugWidget = false;


FeaturesScrollAreaContents::FeaturesScrollAreaContents(QWidget * parent) :
    QWidget(parent)
{
   _cls = metaObject()->className();

  //  _pageChangeObservers = new QVector<PageChangeObserver*>;

   // setLayout(new QVBoxLayout());

   // check that signal being fired
   // connect(this, SIGNAL(showCentralWidgetByWidget(QWidget *)),
   //         qApp, SLOT(aboutQt()));

   // layout()->setContentsMargins(0,0,0,0); // no layout defined
   if (debugLayout)
     setStyleSheet("background-color:aqua;");
}


FeaturesScrollAreaContents::~FeaturesScrollAreaContents() {
   // TODO IMPLEMENT!!!
}


void FeaturesScrollAreaContents::setModel(FeatureBaseModel * baseModel) {
   _baseModel = baseModel;
}


void FeaturesScrollAreaContents::setContainingScrollArea(QScrollArea * scrollArea) {
   _containingScrollArea = scrollArea;
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
   PRINTFTCMF(debugFunc, "Executing");
   assert(_containingScrollArea != nullptr);
   // _containingScrollArea->hide();

}

void FeaturesScrollAreaContents::endInitialLoad()
{
   bool debugFunc = debugWidget;
   debugFunc = false;
   PRINTFTCMF(debugFunc, "Executing");
   assert(_containingScrollArea);

   emit showCentralWidgetByWidget(_containingScrollArea);

   // notifyPageChangeObservers(123);   // dummy value for now
}

#ifdef PAGE_CHANGE_OBSERVER
void FeaturesScrollAreaContents::addPageChangeObserver(PageChangeObserver * observer) {
   _pageChangeObservers->append(observer);
}


void FeaturesScrollAreaContents::notifyPageChangeObservers(int pageno) {
   int ct = _pageChangeObservers->count();
   PRINTFCMF(debugSignals, "Starting ct=%d", ct);
   for (int ndx = 0; ndx < ct; ndx++) {
       PRINTFCMF(debugSignals, "Notifying observer");
       PageChangeObserver*  observer = _pageChangeObservers->at(ndx);
       observer->pageChangedByWidget(_containingScrollArea);
   }
}
#endif

