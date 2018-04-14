/* list_model_ui.cpp
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


#include "list_model_view_ui.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QLabel>

#include "monitor.h"



QWidget * initListView(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
    FeatureItemModel * listModel = new FeatureItemModel(baseModel);

    // Layout stacked widget page page_list_view, contains vcp_listview
    QWidget * page_list_view = new QWidget();
    page_list_view->setObjectName(QString::fromUtf8("page_list_view"));

   QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   sizePolicy5.setHorizontalStretch(0);
   sizePolicy5.setVerticalStretch(0);
   sizePolicy5.setHeightForWidth(page_list_view->sizePolicy().hasHeightForWidth());
   page_list_view->setSizePolicy(sizePolicy5);

   QFont font;
   font.setFamily(QString::fromUtf8("LM Mono 9"));
   font.setPointSize(9);
   page_list_view->setFont(font);

   QListView * vcp_listView = new QListView(page_list_view);
   vcp_listView->setObjectName(QString::fromUtf8("vcp_listView"));

   vcp_listView->setGeometry(QRect(6, 6, 256, 192));
   sizePolicy5.setHeightForWidth(vcp_listView->sizePolicy().hasHeightForWidth());
   vcp_listView->setSizePolicy(sizePolicy5);
   QFont font1;
   font1.setFamily(QString::fromUtf8("Monospace"));
   font1.setPointSize(10);
   vcp_listView->setFont(font1);

   QLabel* label = new QLabel(page_list_view);
   label->setObjectName(QString::fromUtf8("label"));
   label->setGeometry(QRect(268, 6, 342, 17));
   label->setText(QApplication::translate("MainWindow", "Header line for page_list_view containing vcp_listView", 0));

   int pageno = stackedWidget->count();
   stackedWidget->addWidget(page_list_view);

   // These  connects belong are for listModel, not listWidget
   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                    listModel,  SLOT(  startInitialLoad()));
   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                    listModel,  SLOT(  endInitialLoad()));

   curMonitor->setFeatureItemModel(listModel);

   curMonitor->_pageno_list_view = pageno;
   curMonitor->page_list_view    = page_list_view;
   curMonitor->vcp_listView      = vcp_listView;

   return page_list_view;
}

