// number_entry_widget.cpp

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtCore/QRegExp>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QLineEdit>
#include <QtGui/QValidator>

#include "base/core.h"
#include "base/ddcui_parms.h"
#include "base/widget_debug.h"

#include "hex_number_validator.h"
#include "number_entry_widget.h"


void NumberEntryWidget::layoutWidget(
      uint8_t bytect,
      QFont   font,
      int     style,      // not currently used
      QSize   size)
{
    setMaxLength( 2*bytect);
    setFont(font);
    // setFrameStyle(style); ???
    setFixedSize(size);
    setAlignment(Qt::AlignRight);

    if (debugLayout)
       setStyleSheet("background-color:orange;");
}

 
// initializes _widgetState to StateOldValid
NumberEntryWidget::NumberEntryWidget(
      uint8_t bytect,
      QFont   font,
      int     style,      // not currently used
      QSize   size,
      QWidget *parent)
   : QLineEdit(nullptr)
   , _bytect(bytect)
   , _curval(0)
   , _widgetState(NumberEntryWidget::StateOldValid)
{
   _cls = metaObject()->className();
    // TODO: use an enum instead of bytect
  // ***TEMP*** assert(bytect == 1 || bytect == 2);    // could allow 4, just more cases in switch stmts etc.

    // *** Constrain what character strings are accepted

 /* All three methods work.  All provide that at most 2 hexadecimal characters
  * are entered, so the value shown is always valid.  true for 1, 2, what about 3?
  * All three have the same deviciency.
  */

#ifdef METHOD1
   // Method 1, a regular expression validator
      // Unlike the custom validator, does not even allow invalid chars to be entered
    // same problem as methods 1 and 2, only backspace works
    QString rxs = QString("^[A-Fa-f0-9]{%1}$").arg(2*bytect,1,10,QLatin1Char('0')) ;
    QRegExp rx = QRegExp(rxs);
    setValidator( new QRegExpValidator(rx));
#endif

// #ifdef METHOND2
    // Method 2: use Input Mask
    // also allows only valid hex characters to be entered.
    // but hard to change data, have to click on right then basckspace to be able to enter text
    QString hexByteInputMask("HH");
    setInputMask(hexByteInputMask);
// #endif

#ifdef METHOD3
    // similar problem, delete key never works, must position cursor to right of a digit, then
    // can use backspace
    // now it's blocking all input???
    // Method 3: Custom validator
    QValidator* v = new HexNumberValidator();
    setValidator(new HexNumberValidator());
#endif

    layoutWidget(bytect,font,style,size);

    connect(this, &NumberEntryWidget::textEdited,  this, &NumberEntryWidget::onTextEdited);
}


uint16_t
NumberEntryWidget::getExistingValue()
{
   bool debug = false;
   TRACECF(debug, "widget = %s, returning: x%04x", QS2S(this->objectName()), _curval);
   return _curval;
}


uint16_t
NumberEntryWidget::getNewValue()
{
   bool debug = false;
   TRACECF(debug, "widget = %s, returning: x%04x", QS2S(this->objectName()), _valueEntered);
   return _valueEntered;
}


// sets the current value
// changes state to StateOldValue
void
NumberEntryWidget::setValue(uint16_t newval)
{
   bool debug = false;
   TRACECF(debug, "widget = %s, newval = x%04x", QS2S(this->objectName()), newval);

   // Set text value in widget
   int textLength = 2 * _bytect;
   QString sval = QString("%1").arg(newval,textLength,16,QLatin1Char('0'));
   setText(sval);

   _curval = newval;
   _widgetState = NumberEntryWidget::StateOldValid;
}


NumberEntryWidget::States
NumberEntryWidget::getState()
{
   return _widgetState;
}


// if widgestate = OldState, do nothing
// o.w. sets the displayed value to curval
//      set state to OldState
void
NumberEntryWidget::reset()
{
   bool debug = false;
   TRACECF(debug, "Starting. widget = %s, _curState = %d", QS2S(this->objectName()),_widgetState);
   if (_widgetState != NumberEntryWidget::StateOldValid ) {
      int textLength = 2*_bytect;
      _valueEntered = 0;
      setText( QString("%1").arg(_curval,textLength,16,QLatin1Char('0')) );
      _widgetState = NumberEntryWidget::StateOldValid;
   }
   TRACECF(debug, "Done. _curState = %d", _widgetState);
}
      

#ifdef OLD
void
NumberEntryWidget::onTextEdited(const QString &text) {
   TRACECF(true, "Starting. text=%s", text.toLatin1().data());
   bool ok = false;
   if (text.length() > 0) {


      int newval = text.toInt(&ok, 16);
      if (ok) {          // should already have been checked
         _curval = newval;
         TRACECF(true, "About to emit, _curval = newval = 0x%02x", newval);

         emit NumberEntryWidget::valueChanged8( newval & 0xff);
      }
      else {
         TRACEC("Not a number: %s", QS2S(text));
      }
   }
   TRACEC("emitting NumberEntryWidet::isValidValue, _fieldNumber = %d, ok=%s",
            _fieldNumber, sbool(ok) );
   emit NumberEntryWidget::isValidValue(_fieldNumber, ok);
}
#endif


// sets newval to the int value of the tex text
// if value shown has length 0, sets state to Invalid
// if value shown (valueEntered)  == oldval, sets state to OldValid
//  o.w.                sets state to NewValid

void
NumberEntryWidget::onTextEdited(const QString &text) {
   bool debug = false;
   TRACECF(debug, "Starting. text=%s", text.toLatin1().data());

   NumberEntryWidget::States newState = NumberEntryWidget::StateInvalid;
   bool ok = false;
   _valueEntered = 0;
   if (text.length() > 0) {
      _valueEntered = text.toInt(&ok, 16);
      if (ok) {          // should already have been checked
         if (_curval == _valueEntered)
            newState = NumberEntryWidget::StateOldValid;
         else
            newState = NumberEntryWidget::StateNewValid;
      }
      else {
         TRACEC("Not a number: %s", QS2S(text));
      }
   }

   if (newState != _widgetState && newState != NumberEntryWidget::StateInvalid)
   {
      TRACECF(debug, "About to emit stateChanged, newstate = %d", newState);
      emit NumberEntryWidget::stateChanged(this, newState);
   }

   _widgetState = newState;
   // if (newState ==  NumberEntryWidget::StateNewValid )
   //    _curval = newval;
   TRACECF(debug, "Done. setting _widgetState to %d", _widgetState);
}


// For exploring behavior

void
NumberEntryWidget::focusInEvent(QFocusEvent * event) {
   // TRACE("Executing, _curState = %d", _widgetState);
}

void
NumberEntryWidget::focusOutEvent(QFocusEvent * event) {
   // TRACE("Executing, _curState = %d", _widgetState);
}

void
NumberEntryWidget::mousePressEvent(QMouseEvent * event) {
   // TRACE("Executing, _curState = %d", _widgetState);
}

void
NumberEntryWidget::mouseReleaseEvent(QMouseEvent * event) {
   // TRACE("Executing, _curState = %d", _widgetState);
}
