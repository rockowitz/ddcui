/** @file enhanced_slider.h
 *
 *  Subclass of QSlider that only moves the slider when the control
 *  key is pressed.
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
   EnhancedSlider(Qt::Orientation orientation, QWidget * parent = nullptr);
   explicit EnhancedSlider(QWidget *parent = nullptr);

   // ~EnhancedSlider();

   void setControlKeyRequired(bool onoff);

protected:
   void mouseMoveEvent(QMouseEvent *    ev) override;
   void mousePressEvent(QMouseEvent *   ev) override;
   void mouseReleaseEvent(QMouseEvent * ev) override;
   void wheelEvent(QWheelEvent * ev) override;

   void keyPressEvent(QKeyEvent *    ev) override;
   void keyReleaseEvent(QKeyEvent *   ev) override;

   void resizeEvent(QResizeEvent * evt) override;

private:
   void layoutWidget();



private:
   static int idGenerator;
   int _id;
   const char *   _cls;
   bool           _ctrl_key_is_pressed;
   bool           _ctrl_key_required;

};

#endif /* ENHANCED_SLIDER_H_ */
