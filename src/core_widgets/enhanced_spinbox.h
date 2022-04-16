/** @file enhanced_spinbox.h
 *
 *  Subclass of QSpinBox that can be set to change value
 *  only when the control key is pressed.
 */

// Copyright (C) 2019-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>

class QMouseEvent;
class QKeyEvent;

#ifndef ENHANCED_SPINBOX_H_
#define ENHANCED_SPINBOX_H_

class EnhancedSpinBox  : public QSpinBox
{
   Q_OBJECT

public:
   static void setControlKeyRequired(bool onoff);

   explicit EnhancedSpinBox(QWidget *parent = nullptr);

   // ~EnhancedSpinBox();

protected:
   void mouseMoveEvent(   QMouseEvent * ev) override;
   void mousePressEvent(  QMouseEvent * ev) override;
   void mouseReleaseEvent(QMouseEvent * ev) override;
   void wheelEvent(       QWheelEvent * ev) override;
   void keyPressEvent(      QKeyEvent * ev) override;
   void keyReleaseEvent(    QKeyEvent * ev) override;
   void resizeEvent(     QResizeEvent * ev) override;

private:
   void layoutWidget();


private:
   static int     idGenerator;
   static bool    controlKeyRequired;

   int            _id;
   const char *   _cls;
   bool           _ctrl_key_is_pressed;
};

#endif /* ENHANCED_SPINBOX_H_ */
