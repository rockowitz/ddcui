/** @file enhanced_slider.cpp
 */

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSlider>

#include "c_util/string_util.h"

#include "base/core.h"
#include "base/widget_debug.h"

#include "enhanced_slider.h"


static bool showResizeEvents = false;
static bool showBasicDims = false;

int EnhancedSlider::idGenerator = 1;

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
        , _ctrl_key_required(false)
{
   _cls = metaObject()->className();

   layoutWidget();
}

EnhancedSlider::EnhancedSlider(Qt::Orientation orientation, QWidget * parent)
  : QSlider(orientation, parent)
  , _ctrl_key_is_pressed(false)
  , _ctrl_key_required(false)
{
   _cls = metaObject()->className();
   _id = EnhancedSlider::idGenerator++;

   // TRACEMC("===========================> Instance: %d", _id);

   layoutWidget();
}

// EnhancedSlider::~EnhancedSlider() {
//    ~QSlider::QSlider();
// }

// This is wrong.  Need to set at class level, not for individual instances
void EnhancedSlider::setControlKeyRequired(bool onoff) {
   // TRACE("onoff=%s", sbool(onoff));
   _ctrl_key_required = onoff;
}


void dbgrptQKeyEvent(QKeyEvent * event) {
   printf("  key:  %d\n", event->key());
   printf("  modifiers: 0x%x\n", (unsigned int) event->modifiers());
   printf("  nativeModifiers: 0x%08x\n", event->nativeModifiers());
   printf("  nativeScanCode:  0x%08x\n", event->nativeScanCode());
}


void EnhancedSlider::mouseMoveEvent(QMouseEvent * ev)
{
   // TRACEMC("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s, enabled=%s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required), SBOOL(QSlider::isEnabled() ) );
   if (_ctrl_key_is_pressed || !_ctrl_key_required) {
      // TRACEMC("Passing event to QSlider");
      QSlider::mouseMoveEvent(ev);
      ev->accept();
   }


   // From QtEvent::accepted documentation:
   // By default, isAccepted() is set to true, but don't rely on this as subclasses
   // may choose to clear it in their constructors
   // So in this and other methods explicitly invoke ev->ignore() to propagate the event
   else
   ev->ignore();
}

void EnhancedSlider::wheelEvent(QWheelEvent * ev) {
   // TRACEMC("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s, enabled=%s, accepted=%s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required), SBOOL(QSlider::isEnabled()),
   //          SBOOL(ev->isAccepted()) );

   ev->ignore();
}


void EnhancedSlider::mousePressEvent(QMouseEvent *   ev)
{
   // TRACEMC("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required));
   if (_ctrl_key_is_pressed|| !_ctrl_key_required)
      QSlider::mousePressEvent(ev);

   ev->ignore();
}


void EnhancedSlider::mouseReleaseEvent(QMouseEvent * ev)
{
   // PRINTFCM("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required));
   if (_ctrl_key_is_pressed|| !_ctrl_key_required)
      QSlider::mouseReleaseEvent(ev);

   ev->ignore();
}


void EnhancedSlider::keyPressEvent(QKeyEvent *   ev)
{
   // PRINTFCM("Starting");
   // dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = true;
   QSlider::keyPressEvent(ev);

   ev->ignore();
}


void EnhancedSlider::keyReleaseEvent(QKeyEvent *   ev)
{
   // PRINTFCM("Starting");
   // dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = false;
   QSlider::keyPressEvent(ev);
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

