/** @file value_simple_cont_widget.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

#include "base/ddcui_parms.h"
#include "base/core.h"
#include "base/widget_debug.h"

#include "feature_value_widgets/value_simple_cont_widget.h"


static bool showDimensionReports = false;
static bool showBasicDims  = false || debugFeatureDimensions;
static bool showResizeEvents = false;

void ValueSimpleContWidget::additionalWidgets() {
   bool debug = false;
   TRACEMCF(debug, "Starting." );

   // max value fields
   _maxTitle = new QLabel("Max:");
   _maxTitle->setFixedSize(40,18);
   _maxTitle->setFont(FeatureValueTextFont);
   _maxTitle->setContentsMargins(5,0,0,0);
   if (debugLayout)
      _maxTitle->setStyleSheet("background-color:cyan;");

   _maxValue = new QLabel();
   _maxValue->setFont(FeatureValueTextFont);
   // _maxValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);
   _maxValue->setFrameStyle(QFrame::Plain | QFrame::NoFrame);
   _maxValue->setFixedSize(35,20);
   _maxValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
   if (debugLayout)
      _maxValue->setStyleSheet("background-color:orange;");

   TRACEMCF(debug, "Done." );
}


void ValueSimpleContWidget::createWidgets() {
    bool debug = false;
    TRACEMCF(debug, "Starting.");

    _spinSlider = new SpinSlider();

    TRACEMCF(debug, "Done.");
}


void ValueSimpleContWidget::layoutWidget(QHBoxLayout * layout) {
    bool debug = false;
    TRACEMCF(debug, "Starting." );

    layout->addSpacing(5);
    layout->addWidget(_spinSlider);
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);
    layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
    layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
    setLayout(layout);

    debugSimpleContLayout();
    TRACEMCF(debug, "Done." );
}


ValueSimpleContWidget::ValueSimpleContWidget(QWidget *parent)
    : ValueBaseWidget(parent)
    , _cls( strdup(metaObject()->className()) )
   // , _newval(0)
{
    bool debug = false;
    TRACEMCF(debug, "Starting. After ValueBaseWidget() constructor." );

    _newval = 0;
    createWidgets();
    additionalWidgets();
    QHBoxLayout * layout = new QHBoxLayout();
    layoutWidget(layout);
    connect( _spinSlider, SIGNAL(featureValueChanged(uint8_t, uint8_t, uint8_t)),
             this,        SLOT(onFeatureValueChanged(uint8_t, uint8_t, uint8_t)));

    TRACEMCF(debug, "Done." );
}


ValueSimpleContWidget::~ValueSimpleContWidget() {
   bool debug = false;
   TRACEMCF(debug, "Executing. this._id = %d", _id);
   free((void*) _cls);
}


void   ValueSimpleContWidget::setEnabled(bool onoff) {
   bool debug = false;
   TRACEMCF(debug, "_id=%d, _featureCode=0x%02x, onoff=%s", _id, _featureCode, SBOOL(onoff));
   ValueBaseWidget::setEnabled(onoff);
#ifdef OUT
   if (onoff) {
      _cb->setBackgroundRole(_savedBackgroundColor);
   }
   else {
      _cb->setBackgroundRole(QPalette::Dark);
   }
#endif
   TRACEMCF(debug, "Done.");
}


void ValueSimpleContWidget::setRange(int minval, int maxval) {
   bool debug = false;
   TRACEMCF(debug, "minval=%d, maxval=%d", minval, maxval);
   assert (_minval < 0 && _maxval < 0);
   assert (minval >= 0 && maxval >= 0);
   _minval = minval;
   _maxval = maxval;

   _maxTitle->setText(QString("Range:"));
   _maxValue->setText( QString::asprintf("%d-%d", minval, maxval) );
   TRACEMCF(debug, "Done");
}

void ValueSimpleContWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    debug = debug || debugValueWidgetSignals;
    // _guiChange = false;
    TRACEMCF(debug, "Starting. feature code: 0x%02x, before ValueBaseWidget::setFeatureValue()", fv.featureCode());
    ValueBaseWidget::setFeatureValue(fv);

    if (_maxval < 0) {     // setRange has not been called
       _maxval = _mh << 8 | _ml;
       _minval = 0;
    }
    int curval = _sh << 8 | _sl;

    _featureCode = fv.featureCode();  // needed when signaling value changed

    TRACEMCF(debug, "feature=0x%02x, curval=%d, maxval=%d", _featureCode, curval, _maxval);

    // maxval = 99999;   // for testing big numbers
    // curval = 99999;
    // _guiChange = false;

    TRACEMCF(debug, "Calling  _spinSlider->setFeatureCode(), setRange(), setShSl()");
    _spinSlider->setFeatureCode(fv.featureCode());
    _spinSlider->setRange(_minval, _maxval);
    _spinSlider->setShSl(curval);

    TRACEMCF(debug, "Done.");
}


void ValueSimpleContWidget::setCurrentShSl(uint16_t newval) {
    bool debug = false;
    debug = debug || debugValueWidgetSignals;
    TRACEMCF(debug, "Starting. newval = 0x%04x", newval);

    ValueBaseWidget::setCurrentShSl(newval);
    int curval = _sh << 8 | _sl;
    _spinSlider->setShSl(curval);

    TRACEMCF(debug, "Done. feature=0x%02x, curval=%d", _featureCode , curval);
}


// model is querying value
uint16_t ValueSimpleContWidget::getCurrentShSl() {
   int curval = _spinSlider->getShSl();
    _sh = (curval >> 8) & 0xff;
    _sl = curval & 0xff;
    uint16_t result = (_sh << 8) | _sl;
    return result;
}


void ValueSimpleContWidget::onFeatureValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl) {
   bool debug = false;
   TRACEMCF(debug, "featureCode=0x%02x, sh=0x%02x, sl=0x%02x,_featureCode=0x%02x",
                  featureCode, sh, sl, _featureCode);
   TRACEMCF(debug, "Emitting featureValueChanged(0x%02x, 0x%02x, 0x%02x)", _featureCode, sh, sl);
   emit featureValueChanged(_featureCode, sh, sl);
}


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


//
// Debug Methods
//

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


void ValueSimpleContWidget::debugSimpleContLayout()
{
    if (debugLayout && _id == 1) {
       this->setStyleSheet("background-color:yellow;");

       static bool dimensionReportShown = false;

       if (showDimensionReports && !dimensionReportShown) {

          TRACEC("_spinSlider dimensions");
          reportWidgetDimensions(_spinSlider, _cls, __func__);

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
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_spinSlider);
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_maxTitle);
       REPORT_BASIC_WIDGET_DIMENSIONS(this->_maxValue);

       QObjectList  childs = this->children();
       for (int ndx = 0; ndx < childs.size(); ndx++) {
           QObject* curobj = childs.at(ndx);
           QString name   = curobj->objectName();
           const char *  clsName = curobj->metaObject()->className();
           printf("   Child: %s, type:%s\n", name.toLatin1().data(), clsName);
       }
       fflush(stdout);
       basicDimsShown = true;
    }
}

