/* features_scrollarea_view.cpp */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </copyright>
 */

#include "feature_scrollarea/features_scrollarea_view.h"

#include <QtCore/QString>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <typeinfo>

#include "base/global_state.h"
#include "base/other_options_state.h"
// #include "base/vertical_scroll_area.h"
#include "base/monitor.h"
#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "nongui/ddc_error.h"
#include "nongui/msgbox_queue.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/feature_widget_header.h"
#include "feature_scrollarea/features_scrollarea_contents.h"


static bool dimensionReportShown = false;

// static QSemaphore errorMsgSemaphore(1);

FeaturesScrollAreaView::FeaturesScrollAreaView(
        Monitor *          monitor,
        FeatureBaseModel * model,
        QStackedWidget *   centralStackedWidget,
        MsgBoxQueue *      msgboxQueue,
        QObject *          parent)
    : QObject(parent)
    , _cls(metaObject()->className())
    , _monitor(monitor)
    , _baseModel(model)
    , _centralStackedWidget(centralStackedWidget)
    , _msgboxQueue(msgboxQueue)
    , _curNcValuesSource(NcValuesSourceUnset)
{
   // _msgboxQueue = msgboxQueue;
   PRINTFTCM("Executing. _msgboxQueue=%p", _msgboxQueue);
}


void FeaturesScrollAreaView::freeContents(void) {
   // TODO
}


void FeaturesScrollAreaView::onEndInitialLoad(void) {
    PRINTFTCM("Starting, Monitor=%s", _monitor->_displayInfo->model_name);
#ifdef ALT_FEATURES
    if (_monitor->_curFeaturesView != Monitor::FEATURES_VIEW_SCROLLAREA_VIEW) {
        PRINTFTCM("Not FEATURES_VIEW_SCROLLAREA, skipping");
        return;
    }
#endif

    // TODO:
    // free existing QScrollArea, QScrollAreaContents

    // scrollWrap contains a FeatureWidgetHeader and a scrollable
    // collection of FeatureWidget rows

    QFrame * scrollWrap = new QFrame();
    QVBoxLayout * wrapLayout = new QVBoxLayout;
    wrapLayout->setSpacing(0);
    wrapLayout->addWidget(new FeatureWidgetHeader());

    QScrollArea * scrollArea = new QScrollArea();
    // causes extra space between C/NC/T column and value column
    // VerticalScrollArea * scrollArea = new VerticalScrollArea();

    FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
    scrollAreaContents->setObjectName("scrollAreaContents local to onEndInitialLoad");

    // no effect, only applies if no layout is set
    // scrollAreaContents->setSizePolicy(pageSizePolicy());

    // screws things up, value block forced right and truncated
    // scrollAreaContents->setMinimumSize(QSize(900,0));

    QVBoxLayout * vLayout  = new QVBoxLayout();
    vLayout->setObjectName("vLayout in onEndInitLoad");
    vLayout->setMargin(0);
    vLayout->setSpacing(0);    // <=== CONTROLS SPACING BETWEEN ROWS
    if (debugLayout) {
       scrollAreaContents->setStyleSheet("background-color:chartreuse;");
       _centralStackedWidget->setStyleSheet("background-color:chocolate:");
    }

    // vLayout->addWidget(new FeatureWidgetHeader());

    int ct = 0;
    for (int feature_code = 0; feature_code < 256; feature_code++) {
         FeatureValue * fv =  _baseModel->modelVcpValueFilteredFind(feature_code);
         if (fv) {
             // FeatureWidget * w = new FeatureWidget();
             FeatureWidget * w = new FeatureWidget(*fv);
             if (debugLayout) {
                // gets applied to the widgets inside w, not to w itself
                w->setStyleSheet("background-color:brown;");
             }
             // w->setFeatureValue(*fv);

             QObject::connect(w ,   &FeatureWidget::valueChanged,
                              this, &FeaturesScrollAreaView::onUIValueChanged);
             vLayout->addWidget(w);
             _widgets[feature_code] = w;
             ct++;
         }
    }

    scrollAreaContents->setLayout(vLayout);
    scrollArea->setWidget(scrollAreaContents);

    wrapLayout->addWidget(scrollArea);
    scrollWrap->setLayout(wrapLayout);

    QObject::connect(_baseModel, &FeatureBaseModel::signalFeatureUpdated3,
                     this,       &FeaturesScrollAreaView::onModelValueChanged);


    /* int pageno = */ _centralStackedWidget->addWidget(scrollWrap);   // was scrollArea
    // _centralStackedWidget->hide();    // no effect
    _centralStackedWidget->setCurrentWidget(scrollWrap);    // was scrollArea

    if (!dimensionReportShown && debugLayout) {
        PRINTFTCM("---------------------> scrollAreaContents in QScrollArea");
        reportWidgetDimensions(scrollAreaContents,    _cls, __func__, "scrollAreaContents in QScrollArea");
        PRINTFTCM("---------------------> QScrollArea in _centralStackedWidget");
        reportWidgetDimensions(scrollArea,            _cls, __func__, "QScrollArea in _centralStackedWidget");
        PRINTFTCM("---------------------> centralStackedWidget" );
        reportWidgetDimensions(_centralStackedWidget, _cls, __func__, "centralStackedWidget");
        dimensionReportShown = true;
    }

    GlobalState& globalState = GlobalState::instance();
    _curNcValuesSource = globalState._otherOptionsState->ncValuesSource;
    _scrollAreaContents = scrollAreaContents;
    _centralStackedWidget->show();

    PRINTFTCM("Done.  feature count: %d", ct);
}


void FeaturesScrollAreaView::onUIValueChanged(uint8_t featureCode, bool writeOnly, uint8_t sh, uint8_t sl) {
   PRINTFTCMF(debugSignals,
             "feature_code = 0x%02x, writeOnly=%s, sh=0x%02x, sl=0x%02x",
             featureCode, sbool(writeOnly), sh, sl);

   FeatureValue * curFv = _baseModel->modelVcpValueFind(featureCode);
   if (curFv && curFv->val().sh == sh && curFv->val().sl == sl) {
      PRINTFTCM("New value matches model value, Suppressing.");
   }
   else {
      VcpRequest * rqst = new VcpSetRequest(featureCode, sl, writeOnly);   // n.b. ignoring sh
      emit signalVcpRequest(rqst);  // used to call into monitor

      if (featureCode == 0x05)  {    // restore factory defaults brightness/contrast
         emit signalVcpRequest( new VcpGetRequest(0x10) );
         emit signalVcpRequest( new VcpGetRequest(0x12) );    // what if contrast is unsupported feature?
      }
      // handle x04  Restore factory defaults
      //        x06  Restore geometry defaults
      //        x08  Restore color defaults
   }
}


void FeaturesScrollAreaView::onModelValueChanged(
      const char* caller,
      uint8_t     featureCode,
      uint8_t     sh,
      uint8_t     sl)
{
   PRINTFTCMF(debugSignals,
             "caller = %s, feature_code = 0x%02x, sh=0x%02x, sl=0x%02x",
             caller, featureCode, sh, sl);

   // find the FeatureWidget for the feature code
   FeatureWidget * curWidget = _widgets[featureCode];

   // set value in the widget
   uint16_t newval = sh << 8 | sl;
   curWidget->setCurrentValue(newval);
}


void FeaturesScrollAreaView::onNcValuesSourceChanged(NcValuesSource newsrc) {
   PRINTFTCMF(debugSignals,
             "newsrc=%d - %s, _curNcValuesSource=%d - %s",
             newsrc,             (char*) ncValuesSourceName(newsrc),
             _curNcValuesSource, (char*) ncValuesSourceName(_curNcValuesSource));

   if (newsrc != _curNcValuesSource) {
      // reportWidgetChildren(_scrollAreaContents, (const char *) "Children of FeatuersScrollAreaView");

      QObjectList  childs = _scrollAreaContents->children();
      for (int ndx = 0; ndx < childs.size(); ndx++) {
          QObject* curobj = childs.at(ndx);
          // QString name   = curobj->objectName();
          // const char *  clsName = curobj->metaObject()->className();
          // printf("   Child: %s, type:%s\n", name.toLatin1().data(), clsName); fflush(stdout);

          // Both .inherits() and dyamic_cast work
         //  if (curobj->inherits("FeatureWidget"))
         //      printf("(%s::%s) inherits()\n", _cls, __func__); fflush(stdout);

          FeatureWidget * curWidget = dynamic_cast<FeatureWidget*>(curobj);
          if (curWidget) {
             // printf("(%s::%s) dynamic_cast succeeded\n", _cls, __func__); fflush(stdout);
             if (curWidget->isSimpleNc()) {
                // printf("(%s::%s) feature_code=0x%02x\n", _cls, __func__, curWidget->_feature_code); fflush(stdout);
                curWidget->setNcValuesSource(newsrc);
             }
          }
      }
      _curNcValuesSource = newsrc;
   }
}


void FeaturesScrollAreaView::onModelDdcError(DdcError* perec) {
    PRINTFTCM("perec=%p, perec->%s", perec, perec->srepr() );
    // std::cout << "typeid(perec):  " << typeid(perec).name()  << std::endl;
    // std::cout << "typeid(*perec): " << typeid(*perec).name() << std::endl;

    DDCA_Display_Info * dinfo = _monitor->_displayInfo;

    QString qstitle = QString("ddcutil API Error");
    QString qsexpl  = perec->expl();
    QMessageBox::Icon icon = QMessageBox::Warning;

    if ( QString::compare(perec->_ddcFunction, QString("ddca_get_capabilities_string")) == 0) {
        // PRINTFTCM("ddca_get_capabilities_string() branch");
        qsexpl = QString::asprintf(
                          "Error reading capabilities string for display %d - %s",
                          dinfo->dispno+1, dinfo->model_name
                       );
        qstitle = "DDC Error";
    }

    MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(
                                   qstitle,
                                   qsexpl,
                                   icon);
    // PRINTFTCM("Calling _msgboxQueue.put() for qe: %s", qs2s(qe->repr()));
    _msgboxQueue->put(qe);

#ifdef OLD
    QMessageBox * msgBox = new QMessageBox();

    if ( QString::compare(perec->_ddcFunction, QString("ddca_get_capabilities_string")) == 0) {
        // PRINTFTCM("ddca_get_capabilities_string() branch");
        msgBox->setText(QString::asprintf(
                          "Error reading capabilities string for display %d - %s",
                          dinfo->dispno+1, dinfo->model_name
                       ));
        msgBox->setWindowTitle("DDC Error");
    }
    else {
        // PRINTFTCM("Normal branch");
        QString detail = perec->expl();
        // how to position over application?
        msgBox->setText(detail);
        msgBox->setWindowTitle("ddcutil API Error");
   }
   delete perec;
   msgBox->setIcon(QMessageBox::Warning);
   msgBox->setModal(true);
   PRINTFTCM("Before msgBox->exec(), thread id=%p", QThread::currentThreadId());
   // errorMsgSemaphore.acquire();
   // PRINTFTCM("acquired semaphore");
   PRINTFTCM("msgBox->exec() disabled");
   // msgBox->exec();
   // PRINTFTCM("before release semaphore");
   // errorMsgSemaphore.release();
   // PRINTFTCM("After msgBox->exec()");
   // std::cout << "Thread id: " << QThread::currentThreadId() << std::endl;
   // msgBox.open();
#endif
}

