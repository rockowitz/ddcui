/* monitor_desc_ui.cpp
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


#include <QtWidgets/QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>

#include "monitor_desc_ui.h"
#include "../base/monitor.h"


// Initialization for monitor info, capabilities are identical

// static
void initPlaintextWidget(
      const char *          name,
      int                   monitorNumber,
      QStackedWidget *      stackedWidget,
      QWidget **            page_loc,
      int *                 pageno_loc,
      QPlainTextEdit **     pagePlainText_loc

)
{
   // TODO: CLEAN UP AND SIMPLIFY!

     QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
     sizePolicy1.setHorizontalStretch(1);
     sizePolicy1.setVerticalStretch(1);
     // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
     sizePolicy1.setHeightForWidth(false);

      // Layout stacked widget page: page_widget, contains moninfoPlainText
      QWidget * page_widget = new QWidget();


      QPlainTextEdit *plainTextWidget;

      // sizePolicy1.setHeightForWidth(page_widget->sizePolicy().hasHeightForWidth());
      page_widget->setSizePolicy(sizePolicy1);

      plainTextWidget = new QPlainTextEdit(page_widget);
      plainTextWidget->setObjectName(QString::asprintf("plainTextWidget-%s-%d", name, monitorNumber));
      plainTextWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

      plainTextWidget->setGeometry(QRect(6, 6, 700, 191));   // was 574,191
      // sizePolicy1.setHeightForWidth(plainTextWidget->sizePolicy().hasHeightForWidth());
      plainTextWidget->setSizePolicy(sizePolicy1);
      plainTextWidget->setMaximumSize(QSize(2000, 16777215));   // 574->2000
      // plainTextWidget->setLineWrapMode(QPlainTextEdit::NoWrap);
      plainTextWidget->setLineWrapMode(QPlainTextEdit::WidgetWidth);
      plainTextWidget->setReadOnly(true);

      QFont fixedFont("Courier");
      fixedFont.setPointSize(10);
      plainTextWidget->setFont(fixedFont);

      // AMEN!
      QHBoxLayout *
      pageMoninfoLayout = new QHBoxLayout(page_widget);
      pageMoninfoLayout->setSpacing(6);
      // pageMoninfoLayout->setContentsMargins(11, 11, 11, 11);
      pageMoninfoLayout->setObjectName(QString::asprintf("pageLayout-%s-%d", name, monitorNumber));
      pageMoninfoLayout->addWidget(plainTextWidget);

      int pageno_widget = stackedWidget->count();
      stackedWidget->addWidget(page_widget);

      page_widget->setObjectName(
            QString::asprintf("page_widget-%s-%d-pageno-%d", name, monitorNumber, pageno_widget));

      *page_loc          = page_widget;
      *pageno_loc        = pageno_widget;
      *pagePlainText_loc = plainTextWidget;
}

QWidget * initMonitorInfoWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget)

{
   initPlaintextWidget(
         "MonitorInfo",
         curMonitor->_monitorNumber,              // 1 based
         stackedWidget,
         &curMonitor->_page_moninfo,
         &curMonitor->_pageno_moninfo,
         &curMonitor->_moninfoPlainText);

    return curMonitor->_page_moninfo;
}


QWidget * initCapabilitiesWidget(
      Monitor *         curMonitor,
      QStackedWidget *  stackedWidget)
{
   initPlaintextWidget(
         "Capabilities",
         curMonitor->_monitorNumber,              // 1 based
         stackedWidget,
         &curMonitor->_page_capabilities,
         &curMonitor->_pageno_capabilities,
         &curMonitor->_capabilitiesPlainText);

   return curMonitor->_page_capabilities;

}


