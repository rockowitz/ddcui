/** @file enhanced_combobox.cpp
 *
 *  Subclass of QComboBox whose value can be changed
 *  only when the control key is pressed.
 */

// Copyright (C) 2019-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QComboBox>

#include "c_util/string_util.h"

#include "base/core.h"
#include "base/global_state.h"
#include "base/widget_debug.h"

#include "enhanced_combobox.h"


static bool showResizeEvents = true;
static bool showBasicDims = false;

int  EnhancedComboBox::idGenerator = 0;
bool EnhancedComboBox::classControlKeyRequired = false;


void EnhancedComboBox::setClassControlKeyRequired(bool onoff) {
   bool debug = true;
   printf("(EnhancedComboBox::setGlobalControlKeyRequired) onoff=%s\n", SBOOL(onoff));
   EnhancedComboBox::classControlKeyRequired = onoff;
}

// #ifdef UNUSED
void EnhancedComboBox::setInstanceControlKeyRequired(bool onoff) {
   bool debug = true;
   printf("(EnhancedComboBox::setInstanceControlKeyRequired) onoff=%s\n", SBOOL(onoff));
   _instanceControlKeyRequired = onoff;
}
// #endif

void EnhancedComboBox::layoutWidget() {
    static bool basicDimsShown = false;
  //  TRACEMC("_id (A) = %d", _id);
    if (showBasicDims && !basicDimsShown && _id == 1) {
    //    TRACEMC("_id (B) = %d", _id);
        REPORT_BASIC_WIDGET_DIMENSIONS(this);
        basicDimsShown = true;
     }
}


EnhancedComboBox::EnhancedComboBox(QWidget * parent)
        : QComboBox(parent)
        , _ctrl_key_is_pressed2(false)
{
   bool debug = true;
   _cls = metaObject()->className();
   _id = ++idGenerator;
   TRACECF(debug, "_id=%d", _id);

   layoutWidget();

// #ifdef UNUSED
   setInstanceControlKeyRequired(
         GlobalState::instance()._uiOptionsState->_controlKeyRequired);

   QWidget::connect(
      GlobalState::instance()._uiOptionsState, &UserInterfaceOptionsState::controlKeyRequired_changed,
      this,                                    &EnhancedComboBox::setInstanceControlKeyRequired );
// #endif

   TRACECF(debug, "Done");
}


// EnhancedComboBox::~EnhancedComboBox() {
//    ~QComboBox::QComboBox();
// }


//
// Event handlers
//

// Events related to control key
//

void EnhancedComboBox::keyPressEvent(QKeyEvent *   ev)
{
   bool debug = false;
   TRACEMF(debug, "Starting");
   dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed2 = true;
   QComboBox::keyPressEvent(ev);

   ev->ignore();
}


void EnhancedComboBox::keyReleaseEvent(QKeyEvent *   ev)
{
   bool debug = false;
   TRACEMF(debug, "Starting");
   dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed2 = false;
   QComboBox::keyPressEvent(ev);
   ev->ignore();
}


void EnhancedComboBox::mouseMoveEvent(QMouseEvent * ev) {
   bool debug = false;
   TRACEMCF(debug, "Starting, _ctrl_key_is_pressed = %s, EnhancedComboBox::controlKeyRequired = %s, enabled=%s",
            SBOOL(_ctrl_key_is_pressed2),
            SBOOL(EnhancedComboBox::classControlKeyRequired),
            SBOOL(QComboBox::isEnabled() ) );
   if (_ctrl_key_is_pressed2 || !EnhancedComboBox::classControlKeyRequired) {
      TRACEMCF(debug, "Passing event to QComboBox");
      QComboBox::mouseMoveEvent(ev);
      ev->accept();
   }
   // From QtEvent::accepted documentation:
   // By default, isAccepted() is set to true, but don't rely on this as subclasses
   // may choose to clear it in their constructors
   // So in this and other methods explicitly invoke ev->ignore() to propagate the event
   else
      ev->ignore();
}


void EnhancedComboBox::mousePressEvent(QMouseEvent *   ev)
{
   bool debug = false;
   TRACEMCF(debug, "Starting, _ctrl_key_is_pressed = %s, EnhancedComboBox::controlKeyRequired = %s",
            SBOOL(_ctrl_key_is_pressed2),
            SBOOL(EnhancedComboBox::classControlKeyRequired));
   // FUTURE: use _classControlKeyRequired
   if (_ctrl_key_is_pressed2|| !EnhancedComboBox::classControlKeyRequired)
      QComboBox::mousePressEvent(ev);

   ev->ignore();
}


void EnhancedComboBox::mouseReleaseEvent(QMouseEvent * ev)
{
   bool debug = false;
   TRACEMCF(debug, "Starting, _ctrl_key_is_pressed = %s, EnhancedComboBox::controlKeyRequired = %s",
            SBOOL(_ctrl_key_is_pressed2), SBOOL(EnhancedComboBox::classControlKeyRequired));
   // FUTURE: use _classControlKeyRequired
   if (_ctrl_key_is_pressed2|| !EnhancedComboBox::classControlKeyRequired)
      QComboBox::mouseReleaseEvent(ev);

   ev->ignore();
}


//
// Other events
//

void EnhancedComboBox::wheelEvent(QWheelEvent * ev) {
   bool debug = false;
   TRACEMCF(debug, "Starting, _ctrl_key_is_pressed = %s, EnhancedComboBox::controlKeyRequired = %s, enabled=%s, accepted=%s",
            SBOOL(_ctrl_key_is_pressed2),
            SBOOL(EnhancedComboBox::classControlKeyRequired),
            SBOOL(QComboBox::isEnabled()),
            SBOOL(ev->isAccepted()) );

   if (_ctrl_key_is_pressed2|| !EnhancedComboBox::classControlKeyRequired)
      ev->ignore();
   else
      ev->accept();
}


void EnhancedComboBox::resizeEvent(QResizeEvent * evt)
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

   // QSlider::resize(newSz);

   // evt->ignore();
   evt->accept();  // o.w. get infintely recursive resize()


   // successfully sets height widget, which forces height of FeatureValueWidget
   // setMinimumHeight(newSz.height() + 5);  // causes infinitely recursive resize

   QSize afterSz = size();
   TRACECF(showDebug, "after base class resize(): size = %d, %d",
                afterSz.width(), afterSz.height());
}

