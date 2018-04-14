/* table_widget_ui.cpp
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
#include <QtWidgets/QTableWidget>

#include "monitor.h"


#include "table_widget_ui.h"


QSizePolicy tableWidgetSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}




// initialization for QTableWidget variant

void initTableWidget(
      Monitor *         curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget)
{
   // Layout stacked widget page page_table_item, contains tableWidget

   QSizePolicy  sizePolicy1 = QSizePolicy();    // probably should take from elsewhere
   QSizePolicy  sizePolicy3 = QSizePolicy();    // probably should take from elsewhere

   QWidget *
   page_table_item = new QWidget();
   page_table_item->setObjectName(QString::fromUtf8("page_table_item"));

   sizePolicy1.setHeightForWidth(page_table_item->sizePolicy().hasHeightForWidth());
   page_table_item->setSizePolicy(sizePolicy1);

   QTableWidget *
   tableWidget = new QTableWidget(page_table_item);
   if (tableWidget->columnCount() < 5)
       tableWidget->setColumnCount(5);
   QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
   QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
   QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
   QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
   QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
   tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
   tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
   tableWidget->setGeometry(QRect(0, -9, 751, 251));

   // sizePolicy3.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
   tableWidget->setSizePolicy(sizePolicy3);
   tableWidget->setMinimumSize(QSize(581, 0));
   tableWidget->setColumnCount(5);


   // Set header titles
   QTableWidgetItem * tableWidgetItem = NULL;
   tableWidgetItem = tableWidget->horizontalHeaderItem(0);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Code", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(1);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Name", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(2);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Type", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(3);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "RW", 0));
   tableWidgetItem = tableWidget->horizontalHeaderItem(4);
   tableWidgetItem->setText(QApplication::translate("MainWindow", "Value", 0));


   int pageno = stackedWidget->count();
   stackedWidget->addWidget(page_table_item);

   curMonitor->_pageno_table_item = pageno;
   curMonitor->page_table_item    = page_table_item;
   curMonitor->tableWidget        = tableWidget;
}

