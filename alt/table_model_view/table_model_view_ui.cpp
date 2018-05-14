/* table_model_view_ui.cpp
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

#include "alt/table_model_view/table_model_view_ui.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>

#include "base/monitor.h"

#include "alt/table_model_view/feature_table_model.h"
#include "alt/table_model_view/feature_value_tableitem_delegate.h"


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

   QTableView * tview = vcp_tableView;
   tview->setModel(tableModel);
   tview->setColumnWidth(0,40);   // feature code
   tview->setColumnWidth(1, 200);  // feature name
   tview->setColumnWidth(2, 40);   // C/NC/T
   tview->setColumnWidth(3, 30);   //  RW/WO/RO
   tview->setColumnWidth(4, 400);  // feature value

   printf("(%s) Before editing config\n", __func__); fflush(stdout);

   tview->setItemDelegateForColumn(4, new FeatureValueTableItemDelegate);
   tview->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
   tview->setSelectionBehavior(QAbstractItemView::SelectItems);
    tview->setSelectionMode(QAbstractItemView::SingleSelection);

   stackedWidget->addWidget(page_table_view);
   curMonitor->_pageno_table_view = pageno;
   curMonitor->_page_table_view = page_table_view;
   curMonitor->_vcp_tableView = vcp_tableView;

}
