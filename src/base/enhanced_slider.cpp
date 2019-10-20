/** @file enhanced_slider.cpp
 */

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QSlider>

#include "c_util/string_util.h"

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "enhanced_slider.h"

EnhancedSlider::EnhancedSlider(QWidget * parent)
        : QSlider(parent)
        , _ctrl_key_is_pressed(false)
        , _ctrl_key_required(false)
{
   _cls = strdup(metaObject()->className());
}

EnhancedSlider::EnhancedSlider(Qt::Orientation orientation, QWidget * parent)
  : QSlider(orientation, parent)
  , _ctrl_key_is_pressed(false)
  , _ctrl_key_required(false)
{
   _cls = strdup(metaObject()->className());
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


void EnhancedSlider::mouseMoveEvent(QMouseEvent *    ev)
{
   // PRINTFCM("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required));
   if (_ctrl_key_is_pressed || !_ctrl_key_required)
      QSlider::mouseMoveEvent(ev);

   // ev->ignore();    // needed?
}

void EnhancedSlider::mousePressEvent(QMouseEvent *   ev)
{
   // PRINTFCM("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required));
   if (_ctrl_key_is_pressed|| !_ctrl_key_required)
      QSlider::mousePressEvent(ev);

   // ev->ignore();    // needed?
}

void EnhancedSlider::mouseReleaseEvent(QMouseEvent * ev)
{
   // PRINTFCM("Starting, _ctrl_key_is_pressed = %s, _ctrl_key_required = %s",
   //          SBOOL(_ctrl_key_is_pressed), SBOOL(_ctrl_key_required));
   if (_ctrl_key_is_pressed|| !_ctrl_key_required)
      QSlider::mouseReleaseEvent(ev);

   // ev->ignore();    // needed?
}

void EnhancedSlider::keyPressEvent(QKeyEvent *   ev)
{
   // PRINTFCM("Starting");
   // dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = true;
   QSlider::keyPressEvent(ev);

   ev->ignore();    // needed?
}

void EnhancedSlider::keyReleaseEvent(QKeyEvent *   ev)
{
   // PRINTFCM("Starting");
   // dbgrptQKeyEvent(ev);
   if (ev->key() == Qt::Key_Control)    // 68
      _ctrl_key_is_pressed = false;
   QSlider::keyPressEvent(ev);
   ev->ignore();    // needed?
}





