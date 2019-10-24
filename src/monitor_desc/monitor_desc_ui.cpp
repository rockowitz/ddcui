// monitor_desc_ui.cpp

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "monitor_desc/monitor_desc_ui.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
// #include <QtWidgets/QSizePolicy>

#include "base/monitor.h"


// Common initialization for monitor info and capabilities

static
void initPlaintextWidget(
      const char *          name,
      int                   monitorNumber,
      QStackedWidget *      stackedWidget,  // central widget of GUI
      QWidget **            page_loc,
      int *                 pageno_loc,
      QPlainTextEdit **     pagePlainText_loc)
{
   // TODO: CLEAN UP AND SIMPLIFY!

     QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

     // not needed
   //   sizePolicy1.setHorizontalStretch(1);
   //   sizePolicy1.setVerticalStretch(1);
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

