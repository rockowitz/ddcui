/** @file features_scrollarea_ui.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
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

   QObject::connect(baseModel,     &FeatureBaseModel::signalEndInitialLoad,
                    featuresView,  &FeaturesScrollAreaView::onEndInitialLoad);

   // QObject::connect(featuresView, &FeaturesScrollAreaView::signalVcpRequest,
   //                  curMonitor,   &Monitor::putVcpRequest);
}


