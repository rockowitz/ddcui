// value_bytes_widget.cpp

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "value_bytes_widget.h"

#include <assert.h>
#include <string.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHBoxLayout>

#include "base/ddcui_parms.h"
#include "base/core.h"
#include "base/widget_debug.h"

#include "feature_value_widgets/value_base_widget.h"

int ValueBytesWidget::idGenerator = 1;
static bool showDimensionReports = false;

static bool titleDimensionReportShown = false;
static bool dimensionReportShown = false;


QLabel *
ValueBytesWidget::newTitle(QString title, int titleHeight) {
   QLabel* lab = new QLabel(title);

   // TRACECF(true, "titleHeight=%d",titleHeight);

   // QFont nonMonoValueFont;
   // nonMonoValueFont.setPointSize(8);

   QSize   titleSize = QSize(20,titleHeight);
   // QFont   titleFont = nonMonoValueFont;
   // QFont   titleFont = FeatureValueTextFont;
   int     titleFrameStyle = QFrame::Plain | QFrame::NoFrame;

   lab->setFixedSize(titleSize);
   // lab->setFont(nonMonoValueFont);
   lab->setFont(FeatureValueTextFont);
   lab->setFrameStyle(titleFrameStyle);

   if (debugLayout) {
      QString titleDebugStyle("background-color:cyan;");
      lab->setStyleSheet(titleDebugStyle);

      if (showDimensionReports && !titleDimensionReportShown) {
      }
   }

   return lab;
}


void
ValueBytesWidget::createWidgets() {
   int buttonHeight = widgetHeight;
   int titleHeight  = widgetHeight - 2;
   // if (_id == 1) {
   //    TRACEC("_id=%d, _widgetHeight=%d, widgetHeight=%d", _id, _featureValueWidgetHeight, widgetHeight);
   // }
   QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
   fixedSizePolicy.setHorizontalStretch(0);    // needed?
   fixedSizePolicy.setVerticalStretch(0);
   // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
   fixedSizePolicy.setHeightForWidth(false);

   // QFont monoValueFont;
   //  monoValueFont.setPointSize(9);
   // monoValueFont.setFamily(QString::fromUtf8("Monospace"));

   _mhTitle = newTitle("mh:", titleHeight);
   _mlTitle = newTitle("ml:", titleHeight);
   _shTitle = newTitle("sh:", titleHeight);
   _slTitle = newTitle("sl:", titleHeight);

   QFont valueFont = FeatureValueNumberEntryFont;
   //QFont valueFont(monoValueFont);
   // int   valueFrameStyle = QFrame::Plain | QFrame::NoFrame;
   QSize valueSize(30,widgetHeight);

   _mhValue  = newTitle("x00", titleHeight);
   _mlValue  = newTitle("x00", titleHeight);
   _shValue = createFormattedComboBox();
   _slValue = createFormattedComboBox();

   _shValue->setMinimumWidth(60);
   _slValue->setMinimumWidth(60);

   // _shValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method

   for (int ndx = 0; ndx <= 255; ndx++) {
        // TRACEMF(debugFunc, "value code: 0x%02x, value_name: %s",
        //         cur->value_code, cur->value_name);
        QString s = QString::asprintf("x%02x  (%d)", ndx, ndx);
        // TRACEMF(debugNcValues, "text being added: %s", QS2S(s));
        _shValue->addItem(s, QVariant(ndx));
        _slValue->addItem(s, QVariant(ndx));
    }

    QSizePolicy sizePolicy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   _applyButton  = new QPushButton("Apply");
   _cancelButton = new QPushButton("Cancel");
   _applyButton->setMaximumSize(55,buttonHeight);
   _applyButton->setSizePolicy(sizePolicy);
   _applyButton->setFont(FeatureValueButtonFont);
   _cancelButton->setMaximumSize(55,buttonHeight);
   _cancelButton->setSizePolicy(sizePolicy);
   _cancelButton->setFont(FeatureValueButtonFont);

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}


void
ValueBytesWidget::layoutWidget() {
   int afterValueSpace = 15;
   int preValueSpace = 10;

   QHBoxLayout * layout = new QHBoxLayout();
   layout->addSpacing(10);
   layout->addWidget(_mhTitle);
   layout->addSpacing(preValueSpace);
   layout->addWidget(_mhValue);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_mlTitle);
   layout->addSpacing(preValueSpace);
   layout->addWidget(_mlValue);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_shTitle);
   layout->addSpacing(afterValueSpace);
   layout->addWidget(_shValue);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_slTitle);
   layout->addSpacing(preValueSpace);
   layout->addWidget(_slValue);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_applyButton);
   layout->addWidget(_cancelButton);

   layout->addStretch(10);     // apply all added size to end

   layout->setSpacing(0);
   layout->setContentsMargins(0,0,0,0);
   setLayout(layout);

   if (debugLayout && _id == 1) {
      this->setStyleSheet("background-color:yellow;");

      if (showDimensionReports && !dimensionReportShown  && _id == 1) {

          TRACEC("_mhTitle dimensions");
          reportWidgetDimensions(_mhTitle, _cls, __func__);

          //TRACEC("_mhWidget dimensions");
          //reportWidgetDimensions(_mhWidget, _cls, __func__);

          TRACEC("_mhvalue dimensions");
          reportWidgetDimensions(_mhValue, _cls, __func__);


          TRACEC("_mlTitle dimensions");
          reportWidgetDimensions(_mlTitle, _cls, __func__);

          // TRACEC("_mlWidget dimensions");
          // reportWidgetDimensions(_mlWidget, _cls, __func__);

          TRACEC("_mlvalue dimensions");
          reportWidgetDimensions(_mlValue, _cls, __func__);

          TRACEC("_applyButton dimensions");
          reportWidgetDimensions(_applyButton, _cls, __func__);

          TRACEC("ValueBytesWidget dimensions");
          reportWidgetDimensions(this, _cls, __func__);
          dimensionReportShown = true;
      }
   }
}


ValueBytesWidget::ValueBytesWidget(QWidget *parent)
    : ValueBaseWidget(parent)
{
    _cls = strdup(metaObject()->className());
    bool debug = false;
    TRACEMCF(debug, "Starting." );

    _id = ValueBytesWidget::idGenerator++;
    createWidgets();
    layoutWidget();

    QObject::connect(_shValue,  SIGNAL(activated(int)),
                     this, SLOT(when_combobox_activated(int)) );

    QObject::connect(_slValue,  SIGNAL(activated(int)),
                     this, SLOT(when_combobox_activated(int)) );

    connect(_applyButton,  &QPushButton::clicked, this, &ValueBytesWidget::onApplyButtonClicked);
    connect(_cancelButton, &QPushButton::clicked, this, &ValueBytesWidget::onCancelButtonClicked);

    TRACEMCF(debug, "Done");
}


ValueBytesWidget::~ValueBytesWidget() {
   free((void*) _cls);
}


void ValueBytesWidget::when_combobox_activated(int ndx) {
   uint8_t new_sh = ndx >> 8;
   uint8_t new_sl = ndx & 0xff;
   if (new_sh != _sh  || new_sl != _sl) {
      _shNew = new_sh;
      _slNew = new_sl;
      _applyButton->setEnabled(true);
      _cancelButton->setEnabled(true);
   }
}


void ValueBytesWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = debugValueWidgetSignals;
    // debug = false;
    TRACEMCF(debug,
              "Starting. feature code: 0x%02x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x sl: 0x%02x",
              fv.featureCode(),
              fv.val().mh,
              fv.val().ml,
              fv.val().sh,
              fv.val().sl);

    ValueBaseWidget::setFeatureValue(fv);

    TRACEMCF(debug,
              "After valueBaseWidget::setFeatureValue() called, feature code: 0x%02x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x sl: 0x%02x",
              _featureCode,
              _mh,
              _ml,
              _sh,
              _sl);

    _guiChange = false;

    _mhValue->setText(QString::asprintf("x%02x", _mh));
    _mlValue->setText(QString::asprintf("x%02x", _ml));

   _shValue->setCurrentIndex(_sh);
   _slValue->setCurrentIndex(_sl);

    // cf:
    // QString s = QString::number(maxval);
    // _maxValue->setText(s);

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);

   _guiChange = true;
   TRACEMCF(debug, "Done");
}


void ValueBytesWidget::setCurrentShSl(uint16_t newval) {
    ValueBaseWidget::setCurrentShSl(newval);
    _guiChange = false;

    // will update what's shown
    _shValue->setCurrentIndex(newval >> 8);
    _slValue->setCurrentIndex(newval & 0xff);

    // int baseval = _sh << 8 | _sl;
    // assert(baseval == newval);
    TRACECF(debugValueWidgetSignals, "feature=0x%02x, newval=0%04x", _featureCode , newval);

    _applyButton->setEnabled(false);
    _cancelButton->setEnabled(false);

    _guiChange = true;
}


// MOC not finding base class implementation #ifdef SHOULD_USE_BASE
uint16_t ValueBytesWidget::getCurrentShSl() {
    return ValueBaseWidget::getCurrentShSl();
    // uint16_t result = (_sh << 8) | _sl;
    // return result;
}
// #endif


void  ValueBytesWidget::onApplyButtonClicked(bool checked) {
   bool debug = debugValueWidgetSignals;
   // debug = true;
   // TRACEF(debug, "Executing. checked=%s", sbool(checked));

   if (_shNew != _sh || _slNew != _sl) {
      TRACECF(debug, "Emitting featureValueChanged(). feature code: 0x%02x, new sh: 0x%02x, new sl: 0x%02x",
                _featureCode, _shNew, _slNew);
      _sh = _shNew;
      _sl = _slNew;
      emit featureValueChanged(_featureCode, _shNew, _slNew);
   }

   // what to do while we're waiting for the update to apply or fail?

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}


void ValueBytesWidget::onCancelButtonClicked(bool checked) {
   _shNew = _sh;
   _slNew = _sl;

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}

