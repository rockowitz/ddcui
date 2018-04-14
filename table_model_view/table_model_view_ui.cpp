/* table_model_view_ui.cpp
 *
 * Created on: Apr 13, 2018
 *     Author: rock
 *
 * <copyright>
 * Copyright (C) 2014-2015 Sanford Rockowitz <rockowitz@minsoft.com>
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
#include <QtWidgets/QTableView>

#include "monitor.h"

#include "table_model_view_ui.h"


// Initialization for Model/View Table variant

void initTableView(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   int monitorNumber = curMonitor->_monitorNumber;

   FeatureTableModel * tableModel = new FeatureTableModel(baseModel);

   QObject::connect(baseModel,  SIGNAL(signalStartInitialLoad()),
                    tableModel, SLOT(  startInitialLoad()));
   QObject::connect(baseModel,  SIGNAL(signalEndInitialLoad()),
                    tableModel, SLOT(  endInitialLoad()));

   curMonitor->setFeatureTableModel(tableModel);

   QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   sizePolicy1.setHorizontalStretch(1);
   sizePolicy1.setVerticalStretch(1);
   // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
   sizePolicy1.setHeightForWidth(false);

   // Layout stacked widget page page_table_view, contains vcp_tableView
   QWidget * page_table_view = new QWidget();

   // sizePolicy1.setHeightForWidth(page_table_view->sizePolicy().hasHeightForWidth());
   page_table_view->setSizePolicy(sizePolicy1);

   QHBoxLayout *
   horizontalLayout = new QHBoxLayout(page_table_view);
   horizontalLayout->setSpacing(6);
   horizontalLayout->setContentsMargins(11, 11, 11, 11);
   horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

   QTableView *
   vcp_tableView = new QTableView(page_table_view);

   QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
   sizePolicy4.setHorizontalStretch(1);
   sizePolicy4.setVerticalStretch(1);
   // sizePolicy4.setHeightForWidth(vcp_tableView->sizePolicy().hasHeightForWidth());
   sizePolicy4.setHeightForWidth(false);
   vcp_tableView->setSizePolicy(sizePolicy4);
   vcp_tableView->setMinimumSize(QSize(700, 0));     // was 561,0

   vcp_tableView->setSelectionMode(QAbstractItemView::NoSelection);
   vcp_tableView->setSortingEnabled(false);

   horizontalLayout->addWidget(vcp_tableView);

   int pageno = stackedWidget->count();

   page_table_view->setObjectName(QString::asprintf("page_table_view-%d-pageno-%d",
         monitorNumber, pageno));
   vcp_tableView->setObjectName(QString::asprintf("vcp_tableView-%d-pageno-%d",
         monitorNumber, pageno));

   stackedWidget->addWidget(page_table_view);
   curMonitor->_pageno_table_view = pageno;
   curMonitor->_page_table_view = page_table_view;
   curMonitor->_vcp_tableView = vcp_tableView;

}
