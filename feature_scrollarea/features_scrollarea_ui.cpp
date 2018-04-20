/* features_scrollarea_ui.cpp
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

#include <QtWidgets/QApplication>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QLabel>

#include "features_scrollarea_view.h"

#include "features_scrollarea_ui.h"
#include "../base/monitor.h"



// Sets curMonitor->featuresScrollAreaView
// on endInitialLoad, creates new scrollarea, scrollareacontents, does not set them in Monitor

void initFeaturesScrollAreaView(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   printf("(%s) --------------------------> _views_StackedWidget=%p\n", __func__, stackedWidget); fflush(stdout);

   FeaturesScrollAreaView * featuresView =
            new FeaturesScrollAreaView(
                    curMonitor,
                    baseModel,
                    stackedWidget);
   curMonitor->_featuresScrollAreaView = featuresView;

   QObject::connect(baseModel,          SIGNAL(signalEndInitialLoad()),
                    featuresView,       SLOT(  onEndInitialLoad()));

   // requires no-arg constructor, copy constructor
   qRegisterMetaType<DdcError>("DdcError");
   // qRegisterMetaType<DdcError&>("DdcError&");
   QObject::connect(baseModel,     &FeatureBaseModel::signalDdcError,
                    featuresView,  &FeaturesScrollAreaView::onModelDdcError);
   // QObject::connect(baseModel,     SIGNAL(signalDdcError( DdcError&)),
   //                  featuresView,  SLOT(  onModelDdcError(DdcError&)));


      // QObject::connect(baseModel,          &FeatureBaseModel::signalEndInitialLoad,
      //                  featuresView,       &FeaturesScrollAreaView::onEndInitialLoad);

   QObject::connect(featuresView, &FeaturesScrollAreaView::signalVcpRequest,
                    curMonitor,   &Monitor::putVcpRequest);


}


#ifdef UNUSED
// Ugh: class method to locate the showCentralWidgetByWidget slot

// Sets curMonitor->_page_features_scrollarea
//      curMonitor->_featuresScrollAreaContents
//                  _pageno features scroll area

QWidget * MainWindow::initFeaturesScrollArea(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
        //
        // Per-monitor permanent scroll area
        //

        printf("(MainWindow::%s) Allocating per-monitor features scrollarea page\n", __func__); fflush(stdout);


        QScrollArea *   featuresScrollArea = new QScrollArea();

        FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
        QVBoxLayout* vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName("created in initDisplaySelector");
        scrollAreaContents->setLayout(vboxLayout);

        curMonitor->_page_features_scrollarea   = featuresScrollArea;
        curMonitor->_featuresScrollAreaContents = scrollAreaContents;  // n. constructor sets the layout

        featuresScrollArea->setWidget(scrollAreaContents);
        scrollAreaContents->setContainingScrollArea(featuresScrollArea);

        int pageno = stackedWidget->count();
        curMonitor->_pageno_features_scrollarea = pageno;
        stackedWidget->addWidget(featuresScrollArea);

        featuresScrollArea->setObjectName(
            QString::asprintf("features_scrollarea-%d-pageno-%d", curMonitor->_monitorNumber,pageno));
        scrollAreaContents->setObjectName(QString::asprintf("featuresScrollAreaContents-%d", pageno));

        // probably premature
        // printf("(%s) Setting current index %d\n",  __func__, pageno); fflush(stdout);
        stackedWidget->setCurrentIndex(pageno);



        // *** Connect base model to scroll area ***

        QObject::connect(baseModel,          SIGNAL(signalStartInitialLoad()),
                         scrollAreaContents, SLOT(  startInitialLoad()));
        QObject::connect(baseModel,          SIGNAL(signalEndInitialLoad()),
                         scrollAreaContents, SLOT(  endInitialLoad()));
        QObject::connect(baseModel,          SIGNAL(signalFeatureAdded(FeatureValue)),    // char is a built-in QMetaType, uint8_t is not
                         scrollAreaContents, SLOT(  featureAdded(FeatureValue)));
        QObject::connect(baseModel,          SIGNAL(signalFeatureUpdated(char)),
                         scrollAreaContents, SLOT(  featureUpdated(char)));

        // Use Qt5 function pointers
        QObject::connect(baseModel,          &FeatureBaseModel::signalFeatureAdded,
                         scrollAreaContents, &FeaturesScrollAreaContents::featureAdded);

        QObject::connect(baseModel,          &FeatureBaseModel::signalFeatureUpdated,
                         scrollAreaContents, &FeaturesScrollAreaContents::featureUpdated);

        // not working, try old mechanism
        // now it's working, sigh
        QObject::connect(scrollAreaContents, &FeaturesScrollAreaContents::showCentralWidgetByWidget,
                         this,               &MainWindow::showCentralWidgetByWidget);

        QObject::connect(scrollAreaContents, SIGNAL(showCentralWidgetByWidget(QWidget*)),
                         this,               SLOT(  showCentralWidgetByWidget(QWidget*)) );

        // slots not working, but observer is triggered
        // scrollAreaContents->addPageChangeObserver(this);

        // *** End scroll area connections

   return featuresScrollArea;
}
#endif


