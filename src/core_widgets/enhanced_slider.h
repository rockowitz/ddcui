/** @file enhanced_slider.h
 *
 *  Subclass of QSlider that can be set to move the
 *  slider only when the control key is pressed.
 */

// Copyright (C) 2019-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>

class QMouseEvent;
class QKeyEvent;

#ifndef ENHANCED_SLIDER_H_
#define ENHANCED_SLIDER_H_

class EnhancedSlider  : public QSlider
{
   Q_OBJECT

public:
   static void setControlKeyRequired(bool onoff);

   EnhancedSlider(Qt::Orientation orientation, QWidget * parent = nullptr);
   explicit EnhancedSlider(QWidget *parent = nullptr);

   // ~EnhancedSlider();

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

#endif /* ENHANCED_SLIDER_H_ */
