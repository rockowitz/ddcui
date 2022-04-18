/** @file enhanced_slider.cpp
 *
 *  Subclass of QSlider that can be set to move the
 *  slider only when the control key is pressed.
 */

// Copyright (C) 2019-2021 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSlider>

#include "c_util/string_util.h"

#include "base/core.h"
#include "base/widget_debug.h"
#include "base/global_state.h"

#include "enhanced_slider.h"


static bool showResizeEvents = false;
static bool showBasicDims = false;

int  EnhancedSlider::idGenerator = 1;
bool EnhancedSlider::classControlKeyRequired;


void EnhancedSlider::setClassControlKeyRequired(bool onoff) {
   bool debug = true;
   if (debug)
      printf("(EnhancedSlider::setClassontrolKeyRequired) onoff=%s\n", SBOOL(onoff));

   EnhancedSlider::classControlKeyRequired = onoff;
}


#ifdef UNUSED_INSTANCE_CONTROL_KEY_REQUIRED
void EnhancedSlider::setInstanceControlKeyRequired(bool onoff) {
   bool debug = false;
   TRACEMCF(debug, "onoff=%s", SBOOL(onoff));

   _instanceControlKeyRequired = onoff;
}
#endif


void EnhancedSlider::layoutWidget() {
    static bool basicDimsShown = false;
  //  TRACEMC("_id (A) = %d", _id);
    if (showBasicDims && !basicDimsShown && _id == 1) {
    //    TRACEMC("_id (B) = %d", _id);
        REPORT_BASIC_WIDGET_DIMENSIONS(this);
        basicDimsShown = true;
     }
}


EnhancedSlider::EnhancedSlider(QWidget * parent)
        : QSlider(parent)
        , _ctrl_key_is_pressed(false)
{
   _cls = metaObject()->className();

   layoutWidget();

#ifdef UNUSED_INSTANCE_CONTROL_KEY_REQUIRED
   setInstanceControlKeyRequired(
         GlobalState::instance()._uiOptionsState->_controlKeyRequired);

   QWidget::connect(
      GlobalState::instance()._uiOptionsState, &UserInterfaceOptionsState::controlKeyRequired_changed,
      this,                                    &EnhancedSlider::setInstanceControlKeyRequired );
#endif
}


EnhancedSlider::EnhancedSlider(Qt::Orientation orientation, QWidget * parent)
  : QSlider(orientation, parent)
  , _ctrl_key_is_pressed(false)
{
   _cls = metaObject()->className();
   _id = EnhancedSlider::idGenerator++;

   layoutWidget();

#ifdef UNUSED
   setInstanceControlKeyRequired(
         GlobalState::instance()._uiOptionsState->_controlKeyRequired);

   QWidget::connect(
      GlobalState::instance()._uiOptionsState, &UserInterfaceOptionsState::controlKeyRequired_changed,
      this,                                    &EnhancedSlider::setInstanceControlKeyRequired );
#endif
}

// EnhancedSlider::~EnhancedSlider() {
//    ~QSlider::QSlider();
// }


//
// Event handlers
//

// Events related to control key
//

void EnhancedSlider::keyPressEvent(QKeyEvent *   ev) {
   bool debug = false;

   TRACEMCF(debug, "Executing. controlKeyRequired=%s, _instanceControlKeyRequired=%s",
            SBOOL(classControlKeyRequired), SBOOL(_instanceControlKeyRequired));
   if (debug)
      dbgrptQKeyEvent(ev);

   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = true;
   QSlider::keyPressEvent(ev);

   ev->ignore();
}


void EnhancedSlider::keyReleaseEvent(QKeyEvent *   ev)
{
   bool debug = false;
   TRACEMCF(debug, "Executing. controlKeyRequired=%s, _instanceControlKeyRequired=%s",
            SBOOL(classControlKeyRequired), SBOOL(_instanceControlKeyRequired));
   if (debug)
      dbgrptQKeyEvent(ev);

   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = false;
   QSlider::keyPressEvent(ev);
   ev->ignore();
}


void EnhancedSlider::mouseMoveEvent(QMouseEvent * ev) {
   bool debug = false;
   TRACEMCF(debug, "Executing, _ctrl_key_is_pressed = %s, "
                   "EnhancedSlider::classControlKeyRequired = %s, _instanceControlKeyRequired=%s,"
                   " enabled=%s",
            SBOOL(_ctrl_key_is_pressed),
            SBOOL(EnhancedSlider::classControlKeyRequired), SBOOL(_instanceControlKeyRequired),
            SBOOL(QSlider::isEnabled() ) );
   if (_ctrl_key_is_pressed || !EnhancedSlider::classControlKeyRequired) {
      // TRACEMC("Passing event to QSlider");
      QSlider::mouseMoveEvent(ev);
      ev->accept();  // o.w. moves entire window
   }
   // From QtEvent::accepted documentation:
   // By default, isAccepted() is set to true, but don't rely on this as subclasses
   // may choose to clear it in their constructors
   // So in this and other methods explicitly invoke ev->ignore() to propagate the event
   else
      ev->ignore();
}


void EnhancedSlider::mousePressEvent(QMouseEvent *   ev)
{
   bool debug = false;
   TRACEMCF(debug, "Executing, _ctrl_key_is_pressed = %s, "
                   "EnhancedSlider::controlKeyRequired = %s, _instanceControlKeyRequired=%s, "
                   "enabled=%s",
            SBOOL(_ctrl_key_is_pressed),
            SBOOL(EnhancedSlider::classControlKeyRequired), SBOOL(_instanceControlKeyRequired),
            SBOOL(QSlider::isEnabled() ) );
   if (_ctrl_key_is_pressed|| !EnhancedSlider::classControlKeyRequired)
      QSlider::mousePressEvent(ev);

   ev->ignore();
}


void EnhancedSlider::mouseReleaseEvent(QMouseEvent * ev)
{
   bool debug = false;
   TRACEMCF(debug, "Executing, _ctrl_key_is_pressed = %s, "
                   "EnhancedSlider::controlKeyRequired = %s, _instanceControlKeyRequired=%s, "
                   "enabled=%s",
            SBOOL(_ctrl_key_is_pressed),
            SBOOL(EnhancedSlider::classControlKeyRequired), SBOOL(_instanceControlKeyRequired),
            SBOOL(QSlider::isEnabled() ) );
   // FUTURE: use _classControlKeyRequired
   if (_ctrl_key_is_pressed|| !EnhancedSlider::classControlKeyRequired)
      QSlider::mouseReleaseEvent(ev);

   ev->ignore();
}


//
// Other events
//

void EnhancedSlider::wheelEvent(QWheelEvent * ev) {
   bool debug = false;
    TRACEMCF(debug, "Starting, _ctrl_key_is_pressed = %s, "
                    "EnhancedSlider::controlKeyRequired = %s, _instanceControlKeyRequired=%s, "
                    "enabled=%s, accepted=%s",
             SBOOL(_ctrl_key_is_pressed),
             SBOOL(EnhancedSlider::classControlKeyRequired), SBOOL(_instanceControlKeyRequired),
             SBOOL(QSlider::isEnabled()),
             SBOOL(ev->isAccepted()) );

   ev->ignore();
}


void EnhancedSlider::resizeEvent(QResizeEvent * evt)
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

   TRACECF(show, "_id=%d, old size = %d, %d, new size = %d, %d adjusted: %d,%d ",
                 _id, oldSz.width(), oldSz.height(), newSz.width(), newSz.height());

   // QSlider::resize(newSz);

   // evt->ignore();
   evt->accept();  // o.w. get infintely recursive resize()
   // successfully sets height widget, which forces height of FeatureValueWidget
   setMinimumHeight(newSz.height() + 5);
   QSize afterSz = size();
   TRACECF(show, "after base class resize(): size = %d, %d",
                afterSz.width(), afterSz.height());
}

