// value_bytes_widget.cpp

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
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
ValueBytesWidget::layoutWidget() {
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
   int   valueFrameStyle = QFrame::Plain | QFrame::NoFrame;
   QSize valueSize(30,widgetHeight);

   _mhWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
   _mlWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
   _shWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
   _slWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
   _mhWidget->setObjectName("mhWidget");
   _mlWidget->setObjectName("mlWidget");
   _shWidget->setObjectName("shWidget");
   _slWidget->setObjectName("slWidget");

   _mhWidget->setReadOnly(true);
   _mlWidget->setReadOnly(true);

    QSizePolicy sizePolicy = QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   _applyButton  = new QPushButton("Apply");
   _cancelButton = new QPushButton("Cancel");
   _applyButton->setMaximumSize(55,buttonHeight);
   _applyButton->setSizePolicy(sizePolicy);
   //_applyButton->setFont(nonMonoFont9);
   _applyButton->setFont(FeatureValueButtonFont);
   _cancelButton->setMaximumSize(55,buttonHeight);
   _cancelButton->setSizePolicy(sizePolicy);
   _cancelButton->setFont(FeatureValueButtonFont);
   //_cancelButton->setFont(nonMonoFont9);

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);

   // QLabel* spacer = new QLabel();
   // spacer->setFixedSize(10,18);

   int afterValueSpace = 15;

   QHBoxLayout * layout = new QHBoxLayout();
   // layout->addWidget(spacer);
   layout->addSpacing(10);
   layout->addWidget(_mhTitle);
   layout->addWidget(_mhWidget);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_mlTitle);
   layout->addWidget(_mlWidget);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_shTitle);
   layout->addWidget(_shWidget);
   layout->addSpacing(afterValueSpace);

   layout->addWidget(_slTitle);
   layout->addWidget(_slWidget);
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

          TRACEC("_mhWidget dimensions");
          reportWidgetDimensions(_mhWidget, _cls, __func__);

          TRACEC("_mlTitle dimensions");
          reportWidgetDimensions(_mlTitle, _cls, __func__);

          TRACEC("_mlWidget dimensions");
          reportWidgetDimensions(_mlWidget, _cls, __func__);

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
    layoutWidget();

    //  connect(_shWidget, &NumberEntryWidget::setCurrentField, this, &ValueBytesWidget::whenEventFieldChanged);
    // connect(_slWidget, &NumberEntryWidget::setCurrentField, this, &ValueBytesWidget::whenEventFieldChanged);

    // connect(_shWidget, &NumberEntryWidget::valueChanged8, this, &ValueBytesWidget::whenShChanged);
    // connect(_slWidget, &NumberEntryWidget::valueChanged8, this, &ValueBytesWidget::whenSlChanged);

    connect(_shWidget, &NumberEntryWidget::stateChanged, this,  &ValueBytesWidget::whenStateChanged);
    connect(_slWidget, &NumberEntryWidget::stateChanged, this,  &ValueBytesWidget::whenStateChanged);

    connect(_applyButton,  &QPushButton::clicked, this, &ValueBytesWidget::onApplyButtonClicked);
    connect(_cancelButton, &QPushButton::clicked, this, &ValueBytesWidget::onCancelButtonClicked);

    TRACEMCF(debug, "Done");
}




void ValueBytesWidget::checkAcceptCancelEnabled() {
   // TRACE("Starting");
   bool enabled =
          (_shState == NumberEntryWidget::StateNewValid && _slState == NumberEntryWidget::StateNewValid) ||
          (_shState == NumberEntryWidget::StateNewValid && _slState == NumberEntryWidget::StateOldValid) ||
          (_shState == NumberEntryWidget::StateOldValid && _slState == NumberEntryWidget::StateNewValid) ;
   // PRINTFTCM("_shState=%d, _slState=%d, enable=%s", _shState, _slState, sbool(enabled) );
   _applyButton->setEnabled(enabled);
   _cancelButton->setEnabled(enabled);
}

#ifdef UNUSED
void ValueBytesWidget::whenShChanged(uint8_t val) {
   TRACEC("val = 0x%02x", val);
   _shNew = val;
   _shGood = true;

   this->checkAcceptCancelEnabled();
}


void ValueBytesWidget::whenSlChanged(uint8_t val) {
   TRACEC("val = 0x%02x", val);
   _slNew = val;
   _shGood = true;
   this->checkAcceptCancelEnabled();
}
#endif

void ValueBytesWidget::whenStateChanged(NumberEntryWidget * whichWidget, NumberEntryWidget::States newState)
{
   bool debug = false;
   TRACECF(debug, "whichWidget = %s, newState = %d", QS2S(whichWidget->objectName()), newState);

   if (whichWidget == _shWidget)
      _shState = newState;
   else
      _slState = newState;

   checkAcceptCancelEnabled();
}

#ifdef UNUSEDS
void ValueBytesWidget::whenTextValueChanged(bool ok) {
   // check_all_values_ok();
}
#endif


void ValueBytesWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = debugValueWidgetSignals;
    // debug = true;
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

   _mhWidget->setValue(_mh);
   _mlWidget->setValue(_ml);
   _shWidget->setValue(_sh);
   _slWidget->setValue(_sl);

    // cf:
    // QString s = QString::number(maxval);
    // _maxValue->setText(s);

// #ifdef APPLY_CANCEL
//     if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
//     }
// #endif

    _guiChange = true;
    TRACEMCF(debug, "Done");
}




void ValueBytesWidget::setCurrentShSl(uint16_t newval) {
    ValueBaseWidget::setCurrentShSl(newval);
    _guiChange = false;

    // whill update what's shown
    _shWidget->setValue(newval >> 8);
    _slWidget->setValue(newval & 0xff);

     int baseval = _sh << 8 | _sl;
     assert(baseval == newval);
     TRACECF(debugValueWidgetSignals, "feature=0x%02x, curval=0%04x", _featureCode , baseval);

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

// ifdef APPLY_CANCEL
void  ValueBytesWidget::onApplyButtonClicked(bool checked) {
   bool debug = debugValueWidgetSignals;
   // debug = true;
   // TRACEF(debug, "Executing. checked=%s", sbool(checked));

   uint16_t newval = ( _shWidget->getNewValue() <<8 | _slWidget->getNewValue() );

   uint16_t oldval = _sh << 8    | _sl;

   TRACECF(debug, "oldval = 0x%04x, newval = 0x%04x", oldval, newval);
   if (newval != oldval) {
      TRACECF(debug, "Emitting featureValueChanged(). feature code: 0x%02x, new sh: 0x%02x, new sl: 0x%02x",
                _featureCode, newval >>8, newval&0xff);
      emit featureValueChanged(_featureCode, newval >> 8, newval & 0xff);
   }

  // uint8_t slNew =

   // what to do while we're waiting for the update to apply or fail?

  //  _shWidget->setText( QString::number( shNew, 16 ) );
   // etc

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}


void  ValueBytesWidget::onCancelButtonClicked(bool checked) {
    TRACECF(debugValueWidgetSignals, "Executing");

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
   _shWidget->reset();
   _slWidget->reset();
}

#ifdef OLD
void ValueBytesWidget::whenEventFieldChanged(int field_number)
{
   TRACEC("field_number = %d", field_number);
   _currentField = field_number;


}
#endif

