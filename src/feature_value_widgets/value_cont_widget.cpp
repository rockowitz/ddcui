/* value_cont_widget.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_cont_widget.h"

#include <string.h>

#include <QtWidgets/QWidget>
// #include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QtGui/QKeyEvent>
#include <QtGui/QFocusEvent>
#include <QtCore/QEvent>

#include <QtWidgets/QHBoxLayout>

#include "c_util/string_util.h"

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "base/enhanced_slider.h"


static bool dimensionReportShown = false;

ValueContWidget::ValueContWidget(QWidget *parent)
    : ValueBaseWidget(parent)
    , _newval(0)
{
    _cls = strdup(metaObject()->className());
    // TRACE("Starting." );

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);

    QFont monoValueFont;
    monoValueFont.setPointSize(9);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    QFont nonMonoValueFont;
    nonMonoValueFont.setPointSize(8);

    QFont nonMonoFont9;
    nonMonoFont9.setPointSize(8);

    // _curSlider = new QSlider(Qt::Horizontal);
    _curSlider = new EnhancedSlider(Qt::Horizontal);
    _curSlider->setFocusPolicy(Qt::StrongFocus);
    _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _curSlider->setSingleStep(1);
    _curSlider->setFixedSize(200,18);
    if (debugLayout)
    _curSlider->setStyleSheet("background-color:pink;");

    _curSpinBox = new QSpinBox();
    _curSpinBox->setSingleStep(1);
    _curSpinBox->setFixedSize(85,18);   // extra large for 2 byte values, possible horizontal up/down buttons
    _curSpinBox->setAlignment(Qt::AlignRight);
    if (debugLayout)
    _curSpinBox->setStyleSheet("background-color:green;");

    _maxTitle = new QLabel("Max:");
    _maxTitle->setFixedSize(25,18);
    _maxTitle->setFont(nonMonoValueFont);
    if (debugLayout)
    _maxTitle->setStyleSheet("background-color:cyan;");

    _maxValue = new QLabel();
    _maxValue->setFont(monoValueFont);
    // _maxValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    _maxValue->setFrameStyle(QFrame::Plain | QFrame::NoFrame);
    _maxValue->setFixedSize(30,20);
    _maxValue->setAlignment(Qt::AlignRight);

    if (debugLayout)
    _maxValue->setStyleSheet("background-color:blue;");

    _spinBoxTimer = new QTimer();
    _spinBoxTimer->setSingleShot(true);
    _spinBoxTimer->setInterval(1000);

    connect(_spinBoxTimer,   SIGNAL(timeout()),
            this,            SLOT(onSpinBoxTimedOut()));


    connect( _curSlider,  SIGNAL(valueChanged(int)),
             _curSpinBox, SLOT(  setValue(int)));
    connect(_curSpinBox,  SIGNAL(valueChanged(int)),
            _curSlider,   SLOT(  setValue(int)));

    // duplicate
    // connect( _curSlider,  SIGNAL(valueChanged(int)),
    //         this,        SLOT(  onSliderValueChanged(int)));

#ifdef UNUSED
    connect( _curSlider,  SIGNAL(valueChanged(int)),
             this,        SLOT(  onSliderValueChanged(int)));
#endif

    connect( _curSlider,  SIGNAL(sliderReleased()),
             this,        SLOT(  onSliderReleased()));
    connect( _curSpinBox, SIGNAL(valueChanged(int)),
             this,        SLOT(  onSpinBoxValueChanged(int)));

#ifdef UNUSED
    connect( _curSpinBox, SIGNAL(editingFinished()),
             this,        SLOT(  onSpinBoxEditingFinished()));
#endif

#ifdef APPLY_CANCEL
    if (useApplyCancel) {
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

       connect(_applyButton,  &QPushButton::clicked,
               this,          &ValueContWidget::onApplyButtonClicked);
       connect(_cancelButton, &QPushButton::clicked,
               this,          &ValueContWidget::onCancelButtonClicked);
    }
#endif

   QLabel* spacer = new QLabel();
   spacer->setFixedSize(10,18);


    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_curSlider);
    layout->addWidget(_curSpinBox);
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);
    // not the culprit
  // layout->addWidget(spacer);  adds space here, but also whole valuestackedwidget shifts right

    layout->setStretch(1,0);

#ifdef APPLY_CANCEL
    if (useApplyCancel) {
       layout->addSpacing(5);
       layout->addWidget(_applyButton);
       layout->addWidget(_cancelButton);
    }
    else {
#endif
       layout->addSpacing(5);
       layout->addWidget(spacer);
#ifdef APPLY_CANCEL
    }
#endif
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (debugLayout) {
       this->setStyleSheet("background-color:yellow;");

       if (!dimensionReportShown) {
          TRACE("curSlider dimensions");
          reportWidgetDimensions(_curSlider, _cls, __func__);

           TRACE("_curSpinBox dimensions");
           reportWidgetDimensions(_curSpinBox, _cls, __func__);

           TRACE("_maxTitle dimensions");
           reportWidgetDimensions(_maxTitle, _cls, __func__);

           TRACE("_maxValue dimensions");
           reportWidgetDimensions(_maxValue, _cls, __func__);

           TRACE("ValueContWidget dimensions");
           reportWidgetDimensions(this, _cls, __func__);
           dimensionReportShown = true;
       }
    }
}


void ValueContWidget::setFeatureValue(const FeatureValue &fv) {
    TRACEF(debugValueWidgetSignals, "Starting. feature code: 0x%02x", fv.featureCode());
    ValueBaseWidget::setFeatureValue(fv);

    int maxval = _mh << 8 | _ml;
    int curval = _sh << 8 | _sl;

    TRACEF(debugValueWidgetSignals,
              "feature=0x%02x, curval=%d, maxval=%d", _featureCode, curval, maxval);

    // maxval = 99999;   // for testing big numbers
    // curval = 99999;
    _guiChange = false;

    _curSlider->setTickInterval(maxval/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(maxval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(maxval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

    QString s = QString::number(maxval);
    _maxValue->setText(s);

#ifdef APPLY_CANCEL
    if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
    }
#endif

    _guiChange = true;
}


void ValueContWidget::setCurrentShSl(uint16_t newval) {
    ValueBaseWidget::setCurrentShSl(newval);
    _guiChange = false;

     int curval = _sh << 8 | _sl;
     TRACEF(debugValueWidgetSignals, "feature=0x%02x, curval=%d", _featureCode , curval);

    _curSpinBox->setValue(curval);
    _curSlider->setValue(curval);

#ifdef APPLY_CANCEL
    if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
    }
#endif

    _guiChange = true;
}


uint16_t ValueContWidget::getCurrentShSl() {
    int curval = _curSpinBox->value();
    _sh = (curval >> 8) & 0xff;
    _sl = curval & 0xff;
    uint16_t result = (_sh << 8) | _sl;
    return result;
}

#ifdef UNUSED
void ValueContWidget::onSliderValueChanged(int value) {
   TRACEF(debugValueWidgetSignals, "NO ACTION feature=0x%02x, value=%d",  _featureCode, value);
}
#endif


void ValueContWidget::onSliderReleased() {
   TRACEF(debugValueWidgetSignals, "feature=0x%02x",  _featureCode);

   int newval = _curSpinBox->value();

   uint8_t new_sh = (newval >> 8) & 0xff;
   uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //     printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);

   _newval = newval;
#ifdef APPLY_CANCEL
   if (useApplyCancel) {
      _applyButton->setEnabled(true);
      _cancelButton->setEnabled(true);
   }
   else {
#endif
      if (_guiChange)
         emit featureValueChanged(_featureCode, new_sh, new_sl);
#ifdef APPLY_CANCEL
   }
#endif
}


void ValueContWidget::onSpinBoxValueChanged(int value) {
   TRACEF(debugValueWidgetSignals,
             "feature=0x%02x, value=%d, _guiChange=%d", _featureCode, value, _guiChange);

   int newval = _curSpinBox->value();
   // uint8_t new_sh = (newval >> 8) & 0xff;
   // uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //    printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);
   // emit featureValueChanged(_feature_code, new_sh, new_sl);
   _newval = newval;

#ifdef APPLY_CANCEL
   if (useApplyCancel) {
      _applyButton->setEnabled(true);
      _cancelButton->setEnabled(true);
   }
   else {
#endif
      // QTimer::singleShot(1000, this, SLOT(onSpinBoxTimedOut()));
      if (_guiChange) {
         TRACEF(debugValueWidgetSignals, "Starting timer");
         _spinBoxTimer->start();
      }
      else {
         TRACEF(debugValueWidgetSignals,"Not starting timer");
      }
#ifdef APPLY_CANCEL
   }
#endif

}

void ValueContWidget::onSpinBoxTimedOut() {
   TRACEF(debugValueWidgetSignals,
             "feature 0x%02x, _newval=%d, emitting featureValueChanged()", _featureCode, _newval);

   uint8_t new_sh = (_newval >> 8) & 0xff;
   uint8_t new_sl = _newval & 0xff;
   emit featureValueChanged(_featureCode, new_sh, new_sl);
}


void ValueContWidget::setControlKeyRequired(bool onoff) {
   // TRACE("onoff=%s", sbool(onoff) );
   ValueBaseWidget::setControlKeyRequired(onoff);
     _curSlider->setControlKeyRequired(onoff);
}




#ifdef UNNEEDED
void ValueContWidget::when_ckrChanged(bool onoff) {
   // should probably use a function, but quick and dirty
   _curSlider->setControlKeyRequired(onoff);
}
#endif

#ifdef UNUSED
void ValueContWidget::onSpinBoxEditingFinished() {
   if (debugValueWidgetSignals)
       printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
}
#endif


#ifdef APPLY_CANCEL
void  ValueContWidget::onApplyButtonClicked(bool checked) {
   TRACEF(debugValueWidgetSignals, "Executing");

   uint16_t oldval = _sh << 8 | _sl;
   if (_newval != oldval) {
      emit featureValueChanged(_featureCode, _newval >> 8, _newval & 0xff);
   }

   // what to do while we're waiting for the update to apply or fail?
   _curSpinBox->setValue(oldval);

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}


void  ValueContWidget::onCancelButtonClicked(bool checked) {
    TRACEF(debugValueWidgetSignals, "Executing");

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
   uint16_t oldval = _sh<<8 | _sl;
   _curSpinBox->setValue(oldval);
   _newval = oldval;
}
#endif




#ifdef USELESS

// works for mouse
// called when mouse leaves the widget's screen space.
// n. not when focus is lost
void ValueContWidget::leaveEvent(QEvent * event) {
   if (debugValueWidgetSignals)
       printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
   ValueBaseWidget::leaveEvent(event);
   uint16_t oldval = _sh << 8 | _sl;
   if (oldval != _newval) {
      if (debugValueWidgetSignals)
          printf("(%s::%s) Discarding changed value\n", _cls, __func__); fflush(stdout);
      _curSpinBox->setValue(oldval);
      _newval = oldval;
   }
}

#endif





