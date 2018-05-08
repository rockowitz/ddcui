/* features_scroll_area_contents.cpp
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

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
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
   assert(_containingScrollArea != nullptr);
   // _containingScrollArea->hide();

}

void FeaturesScrollAreaContents::endInitialLoad()
{
   PRINTFCMF(debugWidget, "Executing");
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

