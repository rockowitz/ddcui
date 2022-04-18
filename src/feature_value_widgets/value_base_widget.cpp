/* value_base_widget.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_base_widget.h"

#include <QtCore/QMargins>

#include "base/core.h"
#include "base/global_state.h"
#include "base/feature_selector.h"
#include "base/widget_debug.h"

#include "core_widgets/enhanced_combobox.h"

#include "ddcutil_c_api.h"

// class variable
bool ValueBaseWidget::classControlKeyRequired;
// bool showValueWidgetResizeEvents = false;


void ValueBaseWidget::setClassControlKeyRequired(bool onoff) {
   bool debug = false;
   // TRACEMF(debug, "onoff=%s", SBOOL(onoff));
   if (debug)
      printf("(ValueBaseWidget::setControlKeyRequired), onoff=%s\n", SBOOL(onoff));

   ValueBaseWidget::classControlKeyRequired = onoff;
}


// slot
void ValueBaseWidget::setInstanceControlKeyRequired(bool onoff) {
   bool debug = false;
   TRACEMCF(debug, "onoff=%s", SBOOL(onoff));
   _instanceControlKeyRequired = onoff;
   // setEnabled(!_instanceControlKeyRequired);
}


// utility method that provides consistent formatting for comboboxes
EnhancedComboBox *
// QComboBox *
ValueBaseWidget::createFormattedComboBox() {
   bool debug = false;
   TRACECF(debug, "Starting. _id=%d", _id);
   EnhancedComboBox * cb = new EnhancedComboBox();
   // QComboBox * cb = new QComboBox();
   TRACECF(debug, "Allocated");

   QSizePolicy cbSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
   cbSizePolicy.setControlType(QSizePolicy::ComboBox);
   cb->setSizePolicy(cbSizePolicy);
   cb->setFont(FeatureValueComboBoxFont);

   // cb->setMaximumHeight(15);  // clips
   cb->setMaximumWidth(320);
   cb->setMinimumWidth(80);

   // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
   cb->setStyleSheet("background-color:white;color:black;padding-left:2px;");
   cb->setContentsMargins(0,0,0,0);

   TRACECF(debug, "Done, _id=%d", _id);
   return cb;
}


int ValueBaseWidget::nextId = 0;


ValueBaseWidget::ValueBaseWidget(QWidget *parent)
     : QFrame(parent)             // ValueAbstractWidget(parent)
{
   bool debug = false;
   _cls = strdup(metaObject()->className());  // private to this class
   _id = ++nextId;

   TRACECF(debug, "Executing. this._id = %d, _featureCode=%d", _id, _featureCode);
   //QMargins margins = contentsMargins();
   //printf("(ValueBaseWidget::ValueBaseWidget) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //       margins.m_left, margins.m_top, margins.m_right, margins.m_bottom);

   // try setting frame style at this level,
   setFrameStyle(QFrame::Sunken | QFrame::Panel);

   setInstanceControlKeyRequired(
         GlobalState::instance()._uiOptionsState->_controlKeyRequired);

   QWidget::connect(
      GlobalState::instance()._uiOptionsState, &UserInterfaceOptionsState::controlKeyRequired_changed,
      this,                                    &ValueBaseWidget::setInstanceControlKeyRequired );

   TRACECF(debug, "Done");
}


ValueBaseWidget::~ValueBaseWidget() {
   bool debug = false;
   TRACEMCF(debug, "Executing. this._id = %d", _id);

   ddca_free_feature_metadata(_finfo);
   free((void*)_cls);

   TRACEMCF(debug, "Done.");
}


void ValueBaseWidget::setFeatureValue(const FeatureValue &fv) {
   bool debug = false;
   TRACEMCF(debug, "ValueBaseWidget. this._id=%d, featureCode=0x%02x, capVcp=%p, ddcrc=%d",
                    _id, fv.featureCode(), fv.capVcp(), fv.ddcrc());

    _featureCode    = fv.featureCode();
    _dref           = fv.dref();
    _finfo          = fv.finfo();
    _capVcp         = fv.capVcp();
    _vspec          = fv.vspec();
    _mh             = fv.val().mh;
    _ml             = fv.val().ml;
    _sh             = fv.val().sh;
    _sl             = fv.val().sl;
    _ddcrc          = fv.ddcrc();
}


#ifdef UNNEEDED
// hack to give ValueNcWidget access to parsed capabilities
void ValueBaseWidget::setBaseModel(FeatureBaseModel * model) {
   printf("(%s::%s) feature_code=0x%02x, model=%p\n",
         _cls, __func__, _featureCode, model); fflush(stdout);
   _baseModel = model;
}
#endif


void ValueBaseWidget::setCurrentShSl(uint16_t newval) {
    _sh = newval >> 8;
    _sl = newval & 0xff;
}


uint16_t ValueBaseWidget::getCurrentShSl() {
    uint16_t result = (_sh << 8) | _sl;
    return result;
}


#ifdef NO
QSize ValueBaseWidget::sizeHint() const {
    int w = 300;
    int h = 20;
    printf("============> (ValueBaseWidget::sizeHint) returning (%d,%d)\n", w,h ); fflush(stdout);
    return QSize(w,h);
}
#endif

#ifdef NO
void ValueBaseWidget::resizeEvent(QResizeEvent * evt)
{
   bool show = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool resizeEventsShown = false;
   if (showValueWidgetResizeEvents && !resizeEventsShown) {
      show = true;
      resizeEventsShown = true;
   }

   if (show) {
      TRACEC("old size = %d, %d", oldSz.width(), oldSz.height());
      TRACEC("new size = %d, %d", newSz.width(), newSz.height());
   }

   evt->ignore();
}
#endif


//
// Event debugging
//


//
// Event handlers
//

// Events related to control key
//

void ValueBaseWidget::keyPressEvent(QKeyEvent *   ev)
{
   bool debug = true;
   TRACECF(debug,"Executing. controlKeyRequired=%s, _instanceControlKeyRequired=%s",
                 SBOOL(classControlKeyRequired), SBOOL(_instanceControlKeyRequired));
   if (debug)
      dbgrptQKeyEvent(ev);

   if (ev->key() == Qt::Key_Control)    // 68
      _base_ctrl_key_is_pressed = true;
   QFrame::keyPressEvent(ev);

   ev->ignore();
}


void ValueBaseWidget::keyReleaseEvent(QKeyEvent *   ev)
{
   bool debug = true;
   TRACECF(debug,"Executing. controlKeyRequired=%s, _instanceControlKeyRequired=%s",
                 SBOOL(classControlKeyRequired), SBOOL(_instanceControlKeyRequired));
   if (debug)
      dbgrptQKeyEvent(ev);

   if (ev->key() == Qt::Key_Control)    // 68
      _base_ctrl_key_is_pressed = false;
   QFrame::keyPressEvent(ev);
   ev->ignore();
}


void ValueBaseWidget::mouseMoveEvent(QMouseEvent * ev) {
   bool debug = true;
   TRACEMCF(debug, "Executing, _base_ctrl_key_is_pressed = %s, "
                   "ValueBaseWidget::classControllKeyRequired = %s, enabled=%s",
                   SBOOL(_base_ctrl_key_is_pressed),
                   SBOOL(ValueBaseWidget::classControlKeyRequired),
                   SBOOL(ValueBaseWidget::isEnabled() ) );
   if (_base_ctrl_key_is_pressed || !ValueBaseWidget::classControlKeyRequired) {
      TRACEMCF(debug, "Passing event to Qframe");
      QFrame::mouseMoveEvent(ev);
        ev->accept();
   }
   // From QtEvent::accepted documentation:
   // By default, isAccepted() is set to true, but don't rely on this as subclasses
   // may choose to clear it in their constructors
   // So in this and other methods explicitly invoke ev->ignore() to propagate the event
   else
      ev->ignore();
}


void ValueBaseWidget::mousePressEvent(QMouseEvent *   ev)
{
   bool debug = true;
   TRACEMCF(debug, "Executing, _ctrl_key_is_pressed = %s, "
                   "ValueBaseWidget::classControlKeyRequired = %s",
                   SBOOL(_base_ctrl_key_is_pressed),
                   SBOOL(ValueBaseWidget::classControlKeyRequired));
   if (_base_ctrl_key_is_pressed|| !ValueBaseWidget::classControlKeyRequired)
      QFrame::mousePressEvent(ev);

   ev->ignore();
}


void ValueBaseWidget::mouseReleaseEvent(QMouseEvent * ev)
{
   bool debug = true;
   TRACEMCF(debug, "Executing, _ctrl_key_is_pressed = %s, "
                   "ValueBaseWidget::classControlKeyRequired = %s",
                   SBOOL(_base_ctrl_key_is_pressed),
                   SBOOL(ValueBaseWidget::classControlKeyRequired));
   if (_base_ctrl_key_is_pressed|| !ValueBaseWidget::classControlKeyRequired)
      QFrame::mouseReleaseEvent(ev);

   ev->ignore();
}


//
// Other events
//

void ValueBaseWidget::wheelEvent(QWheelEvent * ev) {
   bool debug = true;
   TRACEMCF(debug, "Executing, _ctrl_key_is_pressed = %s, "
                   "ValueBaseWidget::classControlKeyRequired=%s, _instanceControlKeyRequired=%s. "
                   "enabled=%s, accepted=%s",
                   SBOOL(_base_ctrl_key_is_pressed),
                   SBOOL(ValueBaseWidget::classControlKeyRequired),
                   SBOOL(_instanceControlKeyRequired),
                   SBOOL(ValueBaseWidget::isEnabled()),
                   SBOOL(ev->isAccepted()) );

   if (_base_ctrl_key_is_pressed || !ValueBaseWidget::classControlKeyRequired) {
      // TRACEMC("Passing event to QFrame");
      // QFrame::wheelEvent(ev);    // not in EnhancedComboBox
        ev->ignore();  // propagate the event
   }
   // From QtEvent::accepted documentation:
   // By default, isAccepted() is set to true, but don't rely on this as subclasses
   // may choose to clear it in their constructors
   // So in this and other methods explicitly invoke ev->ignore() to propagate the event
   else
      ev->accept();  // soak up the event,
}

