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

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <QtWidgets/QVBoxLayout>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "feature_value_widgets/value_stacked_widget.h"
#include "feature_widget_basic.h"

#include "features_scroll_area_contents.h"


// static bool dimensionReportShown = false;
static bool debugMain = false;
static bool debugSignals = false;

FeaturesScrollAreaContents::FeaturesScrollAreaContents(QWidget * parent) :
    QWidget(parent)
{
   _cls = metaObject()->className();

   _pageChangeObservers = new QVector<PageChangeObserver*>;

   // setLayout(new QVBoxLayout());

   // check that signal being fired
   // connect(this, SIGNAL(showCentralWidgetByWidget(QWidget *)),
   //         qApp, SLOT(aboutQt()));

   // layout()->setContentsMargins(0,0,0,0); // no layout defined
   if (debugLayout)
     setStyleSheet("background-color:aqua;");
}

FeaturesScrollAreaContents::~FeaturesScrollAreaContents() {
   // TODO Auto-generated destructor stub
}

void FeaturesScrollAreaContents::setModel(FeatureBaseModel * baseModel) {
   _baseModel = baseModel;
}

void FeaturesScrollAreaContents::setContainingScrollArea(QScrollArea * scrollArea) {
   _containingScrollArea = scrollArea;
}



void FeaturesScrollAreaContents::featureAdded(FeatureValue fv)
{
   if (debugMain) {
       printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
       // fv.report();
   }

   FeatureWidgetBasic * newTableEntry = new FeatureWidgetBasic();
   newTableEntry->setFeatureValue(fv);

   if (debugMain)
       newTableEntry->dbgrpt();

   layout()->addWidget(newTableEntry);
   _widgets[fv._feature_code] = newTableEntry;
}

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

void FeaturesScrollAreaContents::startInitialLoad(void)
{
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
   assert(_containingScrollArea != nullptr);
   // _containingScrollArea->hide();

}

void FeaturesScrollAreaContents::endInitialLoad()
{
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
   assert(_containingScrollArea);

   // _containingScrollArea->show();

   emit showCentralWidgetByWidget(_containingScrollArea);
   // emit showCentralWidgetByPageno(int pageno);

   notifyPageChangeObservers(123);   // dummy value for now
}


void FeaturesScrollAreaContents::addPageChangeObserver(PageChangeObserver * observer) {
   _pageChangeObservers->append(observer);
}

void FeaturesScrollAreaContents::notifyPageChangeObservers(int pageno) {
   int ct = _pageChangeObservers->count();
   if (debugSignals)
       printf("(%s) Starting ct=%d\n", __func__, ct);  fflush(stdout);
   for (int ndx = 0; ndx < ct; ndx++) {
          if (debugSignals)
              printf("(%s) Notifying observer\n", __func__);  fflush(stdout);
           PageChangeObserver*  observer = _pageChangeObservers->at(ndx);
           observer->pageChangedByWidget(_containingScrollArea);
   }
}



