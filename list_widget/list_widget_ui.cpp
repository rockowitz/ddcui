/* list_widget_ui.cpp
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


#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>

#include "feature_list_widget.h"
#include "list_widget_ui.h"
#include "../base/monitor.h"


//ListWidgetUI::ListWidgetUI() {
//   // TODO Auto-generated constructor stub
//
//}
//
//ListWidgetUI::~ListWidgetUI() {
//   // TODO Auto-generated destructor stub
//}


QSizePolicy tableWidgetSizePolicyLocal() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}



// stackedwidget page contains:
//     a PageListWidget, extends QList Widget
//        contains FeatureWidget, extends QListWidgetItem, Widget
//
// sets curMonitor-> page_listWidget
//                ->_featureListWidget
//                ->_pageno_listWidget

QWidget * initListWidget(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
  // FeatureItemModel * listModel = new FeatureItemModel(baseModel);

   // FeatureListWidget * listWidget = ui->feature_listWidget;   // WRONG  -need separate instance for each monitor


   // page_list_widget/vcp_feature_listwidget
#ifdef REF
   QWidget *page_list_widget;
   QListWidget *feature_listWidget;
   int _pageno_list_widget;
#endif

   QWidget * page_listWidget =  new QWidget();

   // which?
   QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
   sizePolicy3.setHorizontalStretch(1);
   sizePolicy3.setVerticalStretch(1);
   // sizePolicy3.setHeightForWidth(page_vcp->sizePolicy().hasHeightForWidth());
   sizePolicy3.setHeightForWidth(false);
   // WHICH?
   page_listWidget->setSizePolicy(sizePolicy3);
   // page_listWidget->setSizePolicy(pageSizePolicy());
   page_listWidget->setMinimumSize(QSize(700,0));

   curMonitor->_page_listWidget = page_listWidget;


#ifdef PER_MONITOR_FEATURE_SCROLLAREA
   QWidget * page_scrollArea = new QWidget();
   page_scrollArea->setObjectName(QString::asprintf("page_scrollArea-%d", monitorNumber));
   curMonitor->_page_scrollArea = page_scrollArea;
   page_scrollArea->setSizePolicy(pageSizePolicy());
   page_scrollArea->setMinimumSize(QSize(700,0));
#endif

   // TODO: size, font, etc

   // feature_listWidget = new QListWidget(page_list_widget);
  //  FeatureListWidget * listWidget = ui->feature_listWidget;
   FeatureListWidget * featureListWidget= new FeatureListWidget(curMonitor->_page_listWidget);
   featureListWidget->setObjectName(QString::asprintf("featureListWidget-%d",monitorNumber));
   featureListWidget->setModel(baseModel);

   std::string on2 =  featureListWidget->objectName().toStdString();
   // fails: const char * on3 = featureListWidget->objectName().toStdString().c_str();
   // must be separate step.  why?
   const char * on3 = on2.c_str();
   printf("(MainWindow::%s) Allocated FeatureListWidget. objectName = %s\n", __func__, on3); fflush(stdout);

   featureListWidget->setSizePolicy(tableWidgetSizePolicyLocal());
   curMonitor->_featureListWidget = featureListWidget;

   QHBoxLayout *hLayout = new QHBoxLayout(page_listWidget);
   hLayout->setSpacing(6);
   hLayout->setContentsMargins(11,11,11,11);
   hLayout->addWidget(featureListWidget);

   curMonitor->_pageno_listWidget = stackedWidget->count();
   stackedWidget->addWidget(page_listWidget);
   page_listWidget->setObjectName(
         QString::asprintf("page_listWidget-%d-pageno-%d",monitorNumber, curMonitor->_pageno_listWidget));

   // These  connects belong are for listModel, not listWidget
//   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
//                    listModel,  SLOT(  startInitialLoad()));
//   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
//                    listModel,  SLOT(  endInitialLoad()));

   // *** Connect baseModel to ListWidget ***

    qRegisterMetaType<FeatureValue>("FeatureValue");
    QObject::connect(baseModel,         SIGNAL(signalStartInitialLoad()),
                     featureListWidget, SLOT(  startInitialLoad()));
    QObject::connect(baseModel,         SIGNAL(signalEndInitialLoad()),
                     featureListWidget, SLOT(  endInitialLoad()));
    QObject::connect(baseModel,         SIGNAL(signalFeatureAdded(FeatureValue)),    // char is a built-in QMetaType, uint8_t is not
                     featureListWidget, SLOT(  featureAdded(FeatureValue)));
    QObject::connect(baseModel,         SIGNAL(signalFeatureUpdated(char)),
                     featureListWidget, SLOT(  featureUpdated(char)));

    // Use Qt5 function pointers
    QObject::connect(baseModel,         &FeatureBaseModel::signalFeatureAdded,
                     featureListWidget, &FeatureListWidget::featureAdded);

    // use directly coded observers  - DISABLED slots now working
    // baseModel->addFeatureChangeObserver(featureListWidget);

    // listModel, not listWidget
    // curMonitor->setFeatureItemModel(listModel);

   return page_listWidget;
}



#ifdef NO
// really listWidget

// initialization for model/view variant listview

QWidget * initListView(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   // Layout stacked widget page page_list_widget, contains vcp_feature_listwidget
   #ifdef REF
   QWidget *page_list_widget;
   QListWidget *feature_listWidget;
   int _pageno_list_widget;
   #endif

   QWidget * page_list_widget = new QWidget();
   page_list_widget->setObjectName(QString::fromUtf8("page_list_widget"));

   // TODO: size, font, etc

   // feature_listWidget = new QListWidget(page_list_widget);
   FeatureListWidget * feature_listWidget = new FeatureListWidget(page_list_widget);
   feature_listWidget->setObjectName(QString::fromUtf8("feature_listWidget"));

   int pageno  = stackedWidget->count();
   stackedWidget->addWidget(page_list_widget);

   curMonitor->page_list_widget   = page_list_widget;
   curMonitor->feature_listWidget = feature_listWidget;
   curMonitor->_pageno_listWidget = pageno;
}
#endif


