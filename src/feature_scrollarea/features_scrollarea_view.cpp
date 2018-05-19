/* features_scrollarea_view.cpp */

/* <copyright>
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

#include "feature_scrollarea/features_scrollarea_view.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>

#include <stdio.h>
#include <string.h>

#include "base/global_state.h"
#include "base/other_options_state.h"
// #include "base/vertical_scroll_area.h"
#include "base/monitor.h"
#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "nongui/ddc_error.h"

#include "feature_scrollarea/feature_widget.h"
#include "feature_scrollarea/feature_widget_header.h"
#include "feature_scrollarea/features_scrollarea_contents.h"


static bool dimensionReportShown = false;

FeaturesScrollAreaView::FeaturesScrollAreaView(
        Monitor *          monitor,
        FeatureBaseModel * model,
        QStackedWidget *   centralStackedWidget,
        QObject *          parent)
    : QObject(parent)
    , _cls(metaObject()->className())
    , _monitor(monitor)
    , _baseModel(model)
    , _centralStackedWidget(centralStackedWidget)
    , _curNcValuesSource(NcValuesSourceUnset)
{
   //  _cls  = metaObject()->className();
   //  _monitor = monitor;
   //  _centralStackedWidget = centralStackedWidget;
   //  _baseModel = model;
}


void FeaturesScrollAreaView::freeContents(void) {
   // TODO
}


void FeaturesScrollAreaView::onEndInitialLoad(void) {
   // PRINTFCM("Starting");
   if (_monitor->_curFeaturesView != Monitor::FEATURES_VIEW_SCROLLAREA_VIEW) {
      PRINTFCM("Not FEATURES_VIEW_SCROLLAREA, skipping");
      return;
   }

   // TODO:
   // free existing QScrollArea, QScrollAreaContents

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
        PRINTFCM("---------------------> scrollAreaContents in QScrollArea");
        reportWidgetDimensions(scrollAreaContents,    _cls, __func__, "scrollAreaContents in QScrollArea");
        PRINTFCM("---------------------> QScrollArea in _centralStackedWidget");
        reportWidgetDimensions(scrollArea,            _cls, __func__, "QScrollArea in _centralStackedWidget");
        PRINTFCM("---------------------> centralStackedWidget" );
        reportWidgetDimensions(_centralStackedWidget, _cls, __func__, "centralStackedWidget");
        dimensionReportShown = true;
    }

    GlobalState& globalState = GlobalState::instance();
    _curNcValuesSource = globalState._otherOptionsState->ncValuesSource;
    _scrollAreaContents = scrollAreaContents;
    _centralStackedWidget->show();

    PRINTFCM("Done.  feature count: %d", ct);
}


void FeaturesScrollAreaView::onUIValueChanged(uint8_t featureCode, bool writeOnly, uint8_t sh, uint8_t sl) {
   PRINTFCMF(debugSignals,
             "feature_code = 0x%02x, writeOnly=%s, sh=0x%02x, sl=0x%02x",
             featureCode, sbool(writeOnly), sh, sl);

   FeatureValue * curFv = _baseModel->modelVcpValueFind(featureCode);
   if (curFv && curFv->val().sh == sh && curFv->val().sl == sl) {
      PRINTFCM("New value matches model value, Suppressing.");
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
   PRINTFCMF(debugSignals,
             "caller = %s, feature_code = 0x%02x, sh=0x%02x, sl=0x%02x",
             caller, featureCode, sh, sl);

   // find the FeatureWidget for the feature code
   FeatureWidget * curWidget = _widgets[featureCode];

   // set value in the widget
   uint16_t newval = sh << 8 | sl;
   curWidget->setCurrentValue(newval);
}


void FeaturesScrollAreaView::onNcValuesSourceChanged(NcValuesSource newsrc) {
   PRINTFCMF(debugSignals,
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


void FeaturesScrollAreaView::onModelDdcError(DdcError erec) {
    PRINTFCM("erec=%s", erec.srepr() );
    QMessageBox * msgBox = new QMessageBox();
    //  if (strcmp(erec._ddcFunction.toLatin1().data(), "ddca_get_capabilities_string") == 0) {
    if ( QString::compare(erec._ddcFunction, QString("ddca_get_capabilities_string")) == 0) {
        msgBox->setText(QString("Error reading capabilities string"));
        msgBox->setWindowTitle("DDC Error");
    }
    else {
        QString detail = erec.repr();
        // msgBox.setDetailedText(detail);  // detailed text adds details button, not what we want
        // how to position over application?
        msgBox->setText(detail);
        msgBox->setWindowTitle("ddcutil API Error");
   }
   msgBox->setIcon(QMessageBox::Warning);
   msgBox->setModal(true);
   msgBox->exec();
   // msgBox.open();
}

