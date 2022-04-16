/** @file enhanced_combobox.h
 *
 *  Subclass of QComboBox whose value can be changed
 *  only when the control key is pressed.
 */

// Copyright (C) 2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QWidget>
#include <QtWidgets/qcombobox.h>

class QMouseEvent;
class QKeyEvent;

#ifndef ENHANCED_COMBOBOX_H_
#define ENHANCED_COMBOBOX_H_

class EnhancedComboBox  : public QComboBox
{
   Q_OBJECT

public:
   static void setClassControlKeyRequired(bool onoff);

   explicit EnhancedComboBox(QWidget *parent = nullptr);

   // ~EnhancedComboBox();

// #ifdef UNUSED
public slots:
   void setInstanceControlKeyRequired(bool onoff);
// #endif


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
   static bool    classControlKeyRequired;

   int            _id;
   const char *   _cls;
   bool           _ctrl_key_is_pressed2;
   bool           _instanceControlKeyRequired;
};

#endif /* ENHANCED_COMBOBOX_H_ */
