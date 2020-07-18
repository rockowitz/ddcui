/* value_cont_widget.cpp */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_simple_cont_widget.h"

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

#include "base/ddcui_parms.h"
#include "base/core.h"
#include "base/widget_debug.h"

#include "c_util/string_util.h"

#include "core_widgets/enhanced_slider.h"

int ValueSimpleContWidget::idGenerator = 1;

static bool showDimensionReports = false;
static bool showBasicDims  = false || debugFeatureDimensions;
static bool showResizeEvents = false;

void ValueSimpleContWidget::createWidgets() {
   bool debug = false;
   TRACECF(debug, "Starting.  ValueSimpleContWidget::createWidgets()");
   QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);

 #ifdef UNUSED
    QFont monoValueFont;
    monoValueFont.setPointSize(9);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    QFont monoValueFont8;
    monoValueFont.setPointSize(8);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    QFont nonMonoValueFont;
    nonMonoValueFont.setPointSize(8);
 #endif

    // QFont nonMonoFont9;
    // nonMonoFont9.setPointSize(8);

    // QFont FeatureTextFont= QFont(FEATURE_VALUE_TEXT_FONT_FAMILY, FEATURE_VALUE_TEXT_FONT_SIZE);

    // _curSlider = new QSlider(Qt::Horizontal);
    _curSlider = new EnhancedSlider(Qt::Horizontal);
    _curSlider->setFocusPolicy(Qt::StrongFocus);
    _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _curSlider->setSingleStep(1);
   // _curSlider->setFixedSize(200,18);  // was 18 , 14 stuffs slider to bottom
    _curSlider->setMinimumSize(200,10);

    _curSlider->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    // Trying to force a bit of space above and below slider
    // _curSlider->setContentsMargins(3,0,1,0);  // no effect
    // increases height of rows in at least style adwaita
    //  _curSlider->setStyleSheet("padding-top:2px;padding-bottom:2px");
    if (debugLayout)
       _curSlider->setStyleSheet("background-color:pink;");

    _curSpinBox = new QSpinBox();
    _curSpinBox->setSingleStep(1);
    // _curSpinBox->setFixedSize(100,18);   // extra large for 2 byte values, possible horizontal up/down buttons
    _curSpinBox->setMinimumSize(100,18);   // becomes 52,32  - how?
    _curSpinBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    _curSpinBox->setFont(FeatureValueNumberEntryFont);

    _curSpinBox->setAlignment(Qt::AlignRight);
    if (debugLayout)
       _curSpinBox->setStyleSheet("background-color:green;");

 #ifdef DOESNT_SOLVE_PROBLEM
    QWidget * sliderContainer = new QWidget();
    QLayout * sliderContainerLayout = new QVBoxLayout();     // this??
    sliderContainerLayout->setContentsMargins(0,2,0,2);

    sliderContainerLayout->addWidget(_curSlider);
    // AlignTop, AlignBottom, AlignVCenter have no effect
    bool ok = sliderContainerLayout->setAlignment(_curSlider, Qt::AlignBottom); // Qt::AlignVCenter);
    assert(ok);
    sliderContainer->setLayout(sliderContainerLayout);
 #endif

#ifdef FOR_SUBCLASS
    _maxTitle = new QLabel("Max:");
    _maxTitle->setFixedSize(30,18);
    // _maxTitle->setFont(nonMonoValueFont);
    _maxTitle->setFont(FeatureValueTextFont);
    _maxTitle->setContentsMargins(5,0,0,0);
    if (debugLayout)
       _maxTitle->setStyleSheet("background-color:cyan;");

    _maxValue = new QLabel();
    // _maxValue->setFont(monoValueFont8);
    _maxValue->setFont(FeatureValueTextFont);
    // _maxValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    _maxValue->setFrameStyle(QFrame::Plain | QFrame::NoFrame);
    _maxValue->setFixedSize(35,20);
    _maxValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    if (debugLayout)
    _maxValue->setStyleSheet("background-color:blue;");
#endif
    TRACECF(debug, "Done.  ValueSimpleContWidget::createWidgets()");
}


void ValueSimpleContWidget::layoutWidget(QHBoxLayout * layout) {
   bool debug = false;
   TRACECF(debug, "Starting ValueSimpleContWidget." );

   // createWidgets();

   // unused
   //  QLabel* spacer = new QLabel();
   // spacer->setFixedSize(10,18);

    layout->addSpacing(5);
    // layout->addWidget(sliderContainer);   // DOESNT_SOLVE_PROBLEM
    layout->addWidget(_curSlider);

    // crops off tick marks at bottom of slider:
    // how to shrink the slider size?
    // layout->setAlignment(_curSlider, Qt::AlignVCenter);

    layout->addWidget(_curSpinBox);
#ifdef FOR_SUBCLASS
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);

    layout->addSpacing(5+10);
    // layout->addWidget(spacer);
#endif
    layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
    layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
    setLayout(layout);

    if (debugLayout && _id == 1) {
       this->setStyleSheet("background-color:yellow;");

       static bool dimensionReportShown = false;
       if (showDimensionReports && !dimensionReportShown) {
          TRACEC("curSlider dimensions");
          reportWidgetDimensions(_curSlider, _cls, __func__);

           TRACEC("_curSpinBox dimensions");
           reportWidgetDimensions(_curSpinBox, _cls, __func__);

#ifdef FOR_SUBCLASS
           TRACEC("_maxTitle dimensions");
           reportWidgetDimensions(_maxTitle, _cls, __func__);

           TRACEC("_maxValue dimensions");
           reportWidgetDimensions(_maxValue, _cls, __func__);
#endif

           TRACEC("ValueSimpleContWidget dimensions");
           reportWidgetDimensions(this, _cls, __func__);
           dimensionReportShown = true;
       }
    }

    // if (_id == 1) {
    //   TRACEC("widget background color %s", QS2S(this->palette().color(QWidget::backgroundRole()).name()));
    //   TRACEC("slider background color %s", QS2S(_curSlider->palette().color(QWidget::backgroundRole()).name()));
    // }

    static bool basicDimsShown = false;
    if (showBasicDims && !basicDimsShown && _id == 1) {
       REPORT_BASIC_WIDGET_DIMENSIONS(this);
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_curSlider);
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_curSpinBox);
#ifdef FOR_SUBCLASS
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_maxTitle);
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_maxValue);
#endif

       QObjectList  childs = this->children();
       for (int ndx = 0; ndx < childs.size(); ndx++) {
           QObject* curobj = childs.at(ndx);
           QString name   = curobj->objectName();
           const char *  clsName = curobj->metaObject()->className();
           printf("   Child: %s, type:%s\n", name.toLatin1().data(), clsName);
       }
       fflush(stdout);

       printf("Children of _curSpinBox:\n");
       childs = _curSpinBox->children();
       for (int ndx = 0; ndx < childs.size(); ndx++) {
           QObject* curobj = childs.at(ndx);
           QString name   = curobj->objectName();
           const char *  clsName = curobj->metaObject()->className();
           printf("   Child: %s, type:%s\n", name.toLatin1().data(), clsName);
       }
       fflush(stdout);

       basicDimsShown = true;
    }

    TRACECF(debug, "Done. ValueSimpleContWidget" );
}


ValueSimpleContWidget::ValueSimpleContWidget(int minval, int maxval,QWidget *parent)
    : ValueBaseWidget(parent)
    , _newval(0)
    , _minval(minval)
    , _maxval(maxval)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACEMCF(debug, "ValueSimpleContWidget. Starting." );
    _id = ValueSimpleContWidget::idGenerator++;

    // TRACEMC("===========================> Instance: %d", _id);
    createWidgets();
    QHBoxLayout * layout = new QHBoxLayout();
    layoutWidget(layout);

    TRACEMCF(debug, "TRACECMF.  After ValueBaseWidget constructor");

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
    connect( _curSlider,  SIGNAL(valueChanged(int)),
             _curSpinBox, SLOT(  setValue(int)));
    connect(_curSpinBox,  SIGNAL(valueChanged(int)),
            _curSlider,   SLOT(  setValue(int)));

#ifdef UNUSED
    connect( _curSpinBox, SIGNAL(editingFinished()),
             this,        SLOT(  onSpinBoxEditingFinished()));
#endif

    _spinBoxTimer = new QTimer();
    _spinBoxTimer->setSingleShot(true);
    _spinBoxTimer->setInterval(1000);

    connect(_spinBoxTimer,   SIGNAL(timeout()),
            this,            SLOT(onSpinBoxTimedOut()));
    TRACEMCF(debug, "ValueSimpleContWidget. Done." );
}


void ValueSimpleContWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = debugValueWidgetSignals;
    // debug = true;
    TRACEMCF(debug, "ValueSimpleContWidget. Starting. feature code: 0x%02x, before ValueBaseWidget::setFeatureValue()", fv.featureCode());
    ValueBaseWidget::setFeatureValue(fv);

#ifdef FOR_SUBCLASS
    int _maxval = _mh << 8 | _ml;
#endif
    int curval = _sh << 8 | _sl;

    TRACEMCF(debug,
              "feature=0x%02x, curval=%d, maxval=%d", _featureCode, curval, _maxval);

    // maxval = 99999;   // for testing big numbers
    // curval = 99999;
    _guiChange = false;

    int range = _maxval - _minval;
    _curSlider->setTickInterval(range/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(_maxval);
    if (_minval > 0)
       _curSlider->setMinimum(_minval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(_maxval);
    if (_minval > 0)
       _curSpinBox->setMinimum(_minval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

#ifdef FOR_SUBCLASS
    QString s = QString::number(_maxval);
    _maxValue->setText(s);
#endif

    _guiChange = true;
    TRACEMCF(debug, "Done. ValueSimpleContWidget::setFeatureValue()");
}


void ValueSimpleContWidget::setCurrentShSl(uint16_t newval) {
    bool debug = false;
    TRACEMCF(debug, "newval = 0x%04x", newval);
    ValueBaseWidget::setCurrentShSl(newval);
    _guiChange = false;

    int curval = _sh << 8 | _sl;
    TRACEMCF(debugValueWidgetSignals, "feature=0x%02x, curval=%d", _featureCode , curval);

    // in case the timer is running, don't trigger
    _spinBoxTimer->stop();
    _curSpinBox->setValue(curval);
    _curSlider->setValue(curval);

    _guiChange = true;
    TRACECF(debug, "Done");
}


// model is changing value
uint16_t ValueSimpleContWidget::getCurrentShSl() {
    int curval = _curSpinBox->value();
    _sh = (curval >> 8) & 0xff;
    _sl = curval & 0xff;
    uint16_t result = (_sh << 8) | _sl;
    return result;
}

#ifdef UNUSED
void ValueSimpleContWidget::onSliderValueChanged(int value) {
   TRACECF(debugValueWidgetSignals, "NO ACTION feature=0x%02x, value=%d",  _featureCode, value);
}
#endif


void ValueSimpleContWidget::onSliderReleased() {
   bool debug = debugValueWidgetSignals;
   // debug = true;
   TRACECF(debug, "feature=0x%02x",  _featureCode);

   int newval = _curSpinBox->value();

   uint8_t new_sh = (newval >> 8) & 0xff;
   uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //     printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);

   _newval = newval;

   if (_guiChange)
      emit featureValueChanged(_featureCode, new_sh, new_sl);
}


void ValueSimpleContWidget::onSpinBoxValueChanged(int value) {
   bool debug = debugValueWidgetSignals;
   // debug = true;
   TRACECF(debug,
             "feature=0x%02x, value=%d, _guiChange=%d", _featureCode, value, _guiChange);

   int newval = _curSpinBox->value();
   // uint8_t new_sh = (newval >> 8) & 0xff;
   // uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //    printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);
   // emit featureValueChanged(_feature_code, new_sh, new_sl);
   _newval = newval;


   // QTimer::singleShot(1000, this, SLOT(onSpinBoxTimedOut()));
   if (_guiChange) {
      TRACECF(debug, "Starting spinbox timer");
      _spinBoxTimer->start();
   }
   else {
      TRACECF(debug,"Not starting spinbox timer");
   }
}


void ValueSimpleContWidget::onSpinBoxTimedOut() {
   bool debug = debugValueWidgetSignals;
   // debug = true;
   TRACECF(debug,
             "feature 0x%02x, _newval=%d, emitting featureValueChanged()", _featureCode, _newval);

   uint8_t new_sh = (_newval >> 8) & 0xff;
   uint8_t new_sl = _newval & 0xff;
   emit featureValueChanged(_featureCode, new_sh, new_sl);
}


void ValueSimpleContWidget::setControlKeyRequired(bool onoff) {
   // TRACE("onoff=%s", sbool(onoff) );
   ValueBaseWidget::setControlKeyRequired(onoff);
     _curSlider->setControlKeyRequired(onoff);
}


#ifdef UNNEEDED
void ValueSimpleContWidget::when_ckrChanged(bool onoff) {
   // should probably use a function, but quick and dirty
   _curSlider->setControlKeyRequired(onoff);
}
#endif

#ifdef UNUSED
void ValueSimpleContWidget::onSpinBoxEditingFinished() {
   if (debugValueWidgetSignals)
       printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
}
#endif


#ifdef USELESS

// works for mouse
// called when mouse leaves the widget's screen space.
// n. not when focus is lost
void ValueSimpleContWidget::leaveEvent(QEvent * event) {
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


void ValueSimpleContWidget::resizeEvent(QResizeEvent * evt)
{
   bool show = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool sizeShown = false;
   if (showResizeEvents && !sizeShown && _id == 1) {
      show = true;
      sizeShown = true;
   }

#ifdef ALT

   int oldWidth = oldSz.width();
   int oldHeight = oldSz.height();
   int newWidth = newSz.width();
   int newHeight = newSz.width();
   if (oldHeight != newHeight || oldWidth != newWidth) {
      show = true;
   }
#endif

   if (show) {
      TRACEC("_id=%d, old size = %d, %d, new size = %d, %d ",
              oldSz.width(), oldSz.height(), newSz.width(), newSz.height(), _id);
   }

   evt->ignore();
}

