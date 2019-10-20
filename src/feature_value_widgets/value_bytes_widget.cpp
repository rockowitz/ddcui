// value_bytes_widget.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


#include "value_bytes_widget.h"

#include <assert.h>
#include <string.h>

#include <QtWidgets/QWidget>
// #include <QtWidgets/QSlider>
// #include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QVBoxLayout>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"


static bool titleDimensionReportShown = false;
static bool dimensionReportShown = false;


QLabel *
newTitle(QString title) {
   QLabel* lab = new QLabel(title);

   QFont nonMonoValueFont;
   nonMonoValueFont.setPointSize(8);

   QSize   titleSize = QSize(25,18);
   QFont   titleFont = nonMonoValueFont;
   int     titleFrameStyle = QFrame::Plain | QFrame::NoFrame;
   QString titleDebugStyle("background-color:cyan;");

   lab->setFixedSize(titleSize);
   lab->setFont(nonMonoValueFont);
   lab->setFrameStyle(titleFrameStyle);

   if (!titleDimensionReportShown) {
   }

   if (debugLayout)
      lab->setStyleSheet(titleDebugStyle);

   return lab;
}


ValueBytesWidget::ValueBytesWidget(QWidget *parent)
    : ValueBaseWidget(parent)
{
    _cls = strdup(metaObject()->className());
    bool debug = false;
    TRACEF(debug, "Starting." );

    nonMonoFont9.setPointSize(8);

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);

    QFont monoValueFont;
    monoValueFont.setPointSize(9);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    _mhTitle = newTitle("mh:");
    _mlTitle = newTitle("ml:");
    _shTitle = newTitle("sh:");
    _slTitle = newTitle("sl:");

    QFont valueFont(monoValueFont);
    int   valueFrameStyle = QFrame::Plain | QFrame::NoFrame;
    QSize valueSize(30,20);

    // TRACEF(debug, "Before NumberEntryWidget constructors");
    _mhWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    _mlWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    _shWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    _slWidget = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    // TRACE("===> After NumberEntryWidget constructors, before setting object names");
    _mhWidget->setObjectName("mhWidget");
    _mlWidget->setObjectName("mlWidget");
    _shWidget->setObjectName("shWidget");
    _slWidget->setObjectName("slWidget");
    // TRACE("===> After setting object names");

    _mhWidget->setReadOnly(true);       //  which to use?
    _mlWidget->setReadOnly(true);      // or setReadOnly(true)  ??
    bool _slOk = true;
    bool _shOk = true;

// #ifdef APPLY_CANCEL
  //  if (useApplyCancel) {
        QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
       _applyButton  = new QPushButton("Apply");
       _cancelButton = new QPushButton("Cancel");
       _applyButton->setMaximumSize(55,20);
       _applyButton->setSizePolicy(*sizePolicy);
       _applyButton->setFont(nonMonoFont9);
       _cancelButton->setMaximumSize(55,20);
       _cancelButton->setSizePolicy(*sizePolicy);
       _cancelButton->setFont(nonMonoFont9);

       _applyButton->setEnabled(false);
       _cancelButton->setEnabled(false);


   //
// #endif

   QLabel* spacer = new QLabel();
   spacer->setFixedSize(10,18);


    QHBoxLayout * layout = new QHBoxLayout();
    // layout->addWidget(spacer);
    layout->addWidget(_mhTitle);
    layout->addWidget(_mhWidget);
    // not the culprit
  // layout->addWidget(spacer);  adds space here, but also whole valuestackedwidget shifts right

    layout->setStretch(1,0);

    layout->addWidget(_mlTitle);
    layout->addWidget(_mlWidget);
    layout->setStretch(1,0);
    layout->addSpacing(5);

    layout->addWidget(_shTitle);
    layout->addWidget(_shWidget);
    layout->setStretch(1,0);

    layout->addWidget(_slTitle);
    layout->addWidget(_slWidget);
    layout->setStretch(4,0);


       layout->addSpacing(5);
       layout->addWidget(_applyButton);
       layout->addWidget(_cancelButton);

    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    //  connect(_shWidget, &NumberEntryWidget::setCurrentField, this, &ValueBytesWidget::whenEventFieldChanged);
    // connect(_slWidget, &NumberEntryWidget::setCurrentField, this, &ValueBytesWidget::whenEventFieldChanged);

    // connect(_shWidget, &NumberEntryWidget::valueChanged8, this, &ValueBytesWidget::whenShChanged);
    // connect(_slWidget, &NumberEntryWidget::valueChanged8, this, &ValueBytesWidget::whenSlChanged);

    connect(_shWidget, &NumberEntryWidget::stateChanged, this,  &ValueBytesWidget::whenStateChanged);
    connect(_slWidget, &NumberEntryWidget::stateChanged, this,  &ValueBytesWidget::whenStateChanged);

    connect(_applyButton,  &QPushButton::clicked, this, &ValueBytesWidget::onApplyButtonClicked);
    connect(_cancelButton, &QPushButton::clicked, this, &ValueBytesWidget::onCancelButtonClicked);

    if (debugLayout) {
       this->setStyleSheet("background-color:yellow;");

       if (!dimensionReportShown) {

           TRACE("_mhTitle dimensions");
           reportWidgetDimensions(_mhTitle, _cls, __func__);

           TRACE("_mhWidget dimensions");
           reportWidgetDimensions(_mhWidget, _cls, __func__);

           TRACE("_mlTitle dimensions");
           reportWidgetDimensions(_mlTitle, _cls, __func__);

           TRACE("_mlWidget dimensions");
           reportWidgetDimensions(_mlWidget, _cls, __func__);

           TRACE("_applyButton dimensions");
           reportWidgetDimensions(_applyButton, _cls, __func__);

           TRACE("ValueBytesWidget dimensions");
           reportWidgetDimensions(this, _cls, __func__);
           dimensionReportShown = true;
       }
    }
    TRACEF(debug, "Done");
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
   TRACE("val = 0x%02x", val);
   _shNew = val;
   _shGood = true;

   this->checkAcceptCancelEnabled();
}


void ValueBytesWidget::whenSlChanged(uint8_t val) {
   TRACE("val = 0x%02x", val);
   _slNew = val;
   _shGood = true;
   this->checkAcceptCancelEnabled();
}
#endif

void ValueBytesWidget::whenStateChanged(NumberEntryWidget * whichWidget, NumberEntryWidget::States newState)
{
   bool debug = false;
   TRACEF(debug, "whichWidget = %s, newState = %d", qs2s(whichWidget->objectName()), newState);

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
    TRACEF(debug,
              "Starting. feature code: 0x%02x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x sl: 0x%02x",
              fv.featureCode(),
              fv.val().mh,
              fv.val().ml,
              fv.val().sh,
              fv.val().sl);

    ValueBaseWidget::setFeatureValue(fv);

    TRACEF(debug,
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
    TRACEF(debug, "Done");
}


void ValueBytesWidget::setCurrentShSl(uint16_t newval) {
    ValueBaseWidget::setCurrentShSl(newval);
    _guiChange = false;

    // whill update what's shown
    _shWidget->setValue(newval >> 8);
    _slWidget->setValue(newval & 0xff);

     int baseval = _sh << 8 | _sl;
     assert(baseval == newval);
     TRACEF(debugValueWidgetSignals, "feature=0x%02x, curval=0%04x", _featureCode , baseval);

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
   debug = false;
   // TRACEF(debug, "Executing. checked=%s", sbool(checked));

   uint16_t newval = ( _shWidget->getNewValue() <<8 | _slWidget->getNewValue() );

   uint16_t oldval = _sh << 8    | _sl;

   TRACEF(debug, "oldval = 0x%04x, newval = 0x%04x", oldval, newval);
   if (newval != oldval) {
      TRACEF(debug, "Emitting featureValueChanged(). feature code: 0x%02x, new sh: 0x%02x, new sl: 0x%02x",
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
    TRACEF(debugValueWidgetSignals, "Executing");

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
   _shWidget->reset();
   _slWidget->reset();
}

#ifdef OLD
void ValueBytesWidget::whenEventFieldChanged(int field_number)
{
   TRACE("field_number = %d", field_number);
   _currentField = field_number;


}
#endif

