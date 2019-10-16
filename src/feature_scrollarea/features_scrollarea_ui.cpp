/* features_scrollarea_ui.cpp */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QApplication>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QLabel>

#include "base/monitor.h"

#include "features_scrollarea_view.h"
#include "features_scrollarea_ui.h"

#include "nongui/msgbox_queue.h"


// Sets curMonitor->featuresScrollAreaView
// on endInitialLoad, creates new scrollarea, scrollareacontents, does not set them in Monitor

void initFeaturesScrollAreaView(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget,
      MsgBoxQueue *     msgboxQueue
     )
{
   // printf("(%s) stackedWidget=%p\n", __func__, stackedWidget); fflush(stdout);
   // printf("(%s) Starting. msgboxQueue=%p\n", __func__, msgboxQueue); fflush(stdout);

   FeaturesScrollAreaView * featuresView =
            new FeaturesScrollAreaView(
                    curMonitor,
                    baseModel,
                    stackedWidget,
                    msgboxQueue);
   curMonitor->_featuresScrollAreaView = featuresView;

   QObject::connect(baseModel,          SIGNAL(signalEndInitialLoad()),
                    featuresView,       SLOT(  onEndInitialLoad()));

   // requires no-arg constructor, copy constructor
   qRegisterMetaType<DdcError>("DdcError");
   qRegisterMetaType<DdcError*>("DdcError*");
   qRegisterMetaType<DdcDetailedError>("DdcDetailedError");
   qRegisterMetaType<DdcDetailedError*>("DdcDetailedError*");
   qRegisterMetaType<DdcFeatureError>("DdcFeatureError");
   qRegisterMetaType<DdcFeatureError*>("DdcFeatureError*");
   // qRegisterMetaType<DdcVerifyError>("DdcVerifyError");
   // qRegisterMetaType<DdcFeatureError&>();
   QObject::connect(baseModel,     &FeatureBaseModel::signalDdcFeatureError,
                    featuresView,  &FeaturesScrollAreaView::onModelDdcFeatureError);

   QObject::connect(baseModel,     &FeatureBaseModel::signalDdcDetailedError,
                    featuresView,  &FeaturesScrollAreaView::onModelDdcDetailedError);

   // QObject::connect(baseModel,     SIGNAL(signalDdcFeatureError( DdcFeatureError&)),
   //                  featuresView,  SLOT(  onModelDdcFeatureError(DdcFeatureError&)));

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


