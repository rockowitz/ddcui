/** @file enhanced_spinbox.cpp
 *
 *  Subclass of QSpinBox that can be set to move the
 *  spinbox only when the control key is pressed.
 */

// Copyright (C) 2019-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSpinBox>

#include "c_util/string_util.h"

#include "base/core.h"
#include "base/widget_debug.h"

#include "enhanced_spinbox.h"


static bool showResizeEvents = true;
static bool showBasicDims = true;

int  EnhancedSpinBox::idGenerator = 1;
bool EnhancedSpinBox::controlKeyRequired;


void EnhancedSpinBox::setControlKeyRequired(bool onoff) {
   // TRACE("onoff=%s", sbool(onoff));
   // printf(" (setStaticControlKeyRequired)\n");
   EnhancedSpinBox::controlKeyRequired = onoff;
}


void EnhancedSpinBox::layoutWidget() {
    static bool basicDimsShown = false;
  //  TRACEMC("_id (A) = %d", _id);
    if (showBasicDims && !basicDimsShown && _id == 1) {
    //    TRACEMC("_id (B) = %d", _id);
        REPORT_BASIC_WIDGET_DIMENSIONS(this);
        basicDimsShown = true;
     }
}

#ifdef DUP
EnhancedSpinBox::EnhancedSpinBox(QWidget * parent)
        : QSpinBox(parent)
        , _ctrl_key_is_pressed(false)
{
   _cls = metaObject()->className();

   layoutWidget();
}
#endif


EnhancedSpinBox::EnhancedSpinBox(QWidget * parent)
  : QSpinBox(parent)
  , _ctrl_key_is_pressed(false)
{
   _cls = metaObject()->className();
   _id = EnhancedSpinBox::idGenerator++;

   layoutWidget();
}

// EnhancedSpinBox::~EnhancedSpinBox() {
//    ~QSpinBox::QSpinBox();
// }


//
// Event handlers
//

// Events related to control key
//

void EnhancedSpinBox::keyPressEvent(QKeyEvent *   ev)
{
   TRACEMC("Starting");
      dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = true;
   QSpinBox::keyPressEvent(ev);

   ev->ignore();
}


void EnhancedSpinBox::keyReleaseEvent(QKeyEvent *   ev)
{
     TRACEMC("Starting");
    dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = false;
   QSpinBox::keyPressEvent(ev);
   ev->ignore();
}


void EnhancedSpinBox::mouseMoveEvent(QMouseEvent * ev) {
   TRACEMC("Starting, _ctrl_key_is_pressed = %s, EnhancedSpinBox::controlKeyRequired = %s, enabled=%s",
            SBOOL(_ctrl_key_is_pressed), SBOOL(EnhancedSpinBox::controlKeyRequired), SBOOL(QSpinBox::isEnabled() ) );
   if (_ctrl_key_is_pressed || !EnhancedSpinBox::controlKeyRequired) {
      // TRACEMC("Passing event to QSpinBox");
      QSpinBox::mouseMoveEvent(ev);
      ev->accept();  // o.w. moves entire window
   }
   // From QtEvent::accepted documentation:
   // By default, isAccepted() is set to true, but don't rely on this as subclasses
   // may choose to clear it in their constructors
   // So in this and other methods explicitly invoke ev->ignore() to propagate the event
   else
      ev->ignore();
}


void EnhancedSpinBox::mousePressEvent(QMouseEvent *   ev)
{
    TRACEMC("Starting, _ctrl_key_is_pressed = %s, EnhancedSpinBox::controlKeyRequired = %s",
             SBOOL(_ctrl_key_is_pressed), SBOOL(EnhancedSpinBox::controlKeyRequired));
   // FUTURE: use _classControlKeyRequired
   if (_ctrl_key_is_pressed|| !EnhancedSpinBox::controlKeyRequired)
      QSpinBox::mousePressEvent(ev);

   ev->ignore();
}


void EnhancedSpinBox::mouseReleaseEvent(QMouseEvent * ev)
{
   TRACEMC("Starting, _ctrl_key_is_pressed = %s, EnhancedSpinBox::controlKeyRequired = %s",
             SBOOL(_ctrl_key_is_pressed), SBOOL(EnhancedSpinBox::controlKeyRequired));
   // FUTURE: use _classControlKeyRequired
   if (_ctrl_key_is_pressed|| !EnhancedSpinBox::controlKeyRequired)
      QSpinBox::mouseReleaseEvent(ev);

   ev->ignore();
}


//
// Other events
//

void EnhancedSpinBox::wheelEvent(QWheelEvent * ev) {
    TRACEMC("Starting, _ctrl_key_is_pressed = %s, EnhancedSpinBox::controlKeyRequired = %s, enabled=%s, accepted=%s",
             SBOOL(_ctrl_key_is_pressed), SBOOL(EnhancedSpinBox::controlKeyRequired),
             SBOOL(QSpinBox::isEnabled()), SBOOL(ev->isAccepted()) );

   ev->ignore();
}


void EnhancedSpinBox::resizeEvent(QResizeEvent * evt)
{
   bool showDebug = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool sizeShown = false;
   if (showResizeEvents && !sizeShown && _id == 1) {
      showDebug = true;
      sizeShown = true;
   }

#ifdef ALT
   int oldWidth  = oldSz.width();
   int oldHeight = oldSz.height();
   int newWidth  = newSz.width();
   int newHeight = newSz.width();
   if (oldHeight != newHeight || oldWidth != newWidth) {
      showDebug = true;
   }
#endif

   TRACECF(showDebug, "_id=%d, old size = %d, %d, new size = %d, %d adjusted: %d,%d ",
                 _id, oldSz.width(), oldSz.height(), newSz.width(), newSz.height());

   // QSpinBox::resize(newSz);

   evt->accept();  // o.w. get infintely recursive resize()
   // successfully sets height widget, which forces height of FeatureValueWidget
   setMinimumHeight(newSz.height() + 5);
   QSize afterSz = size();
   TRACECF(showDebug, "after base class resize(): size = %d, %d",
                afterSz.width(), afterSz.height());
}

