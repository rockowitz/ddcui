// number_entry_widget.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NUMBER_ENTRY_WIDGET_H_
#define NUMBER_ENTRY_WIDGET_H_

#include <stdint.h>

class QWidget;
class QLineEdit;


class NumberEntryWidget : public QLineEdit
{
    Q_OBJECT
    // Q_ENUMS(States)

public:
    enum States {StateOldValid, StateInvalid, StateNewValid};

    NumberEntryWidget(
          uint8_t  bytect,
          QFont    font,
          int      style,
          QSize    size,
          QWidget *parent = nullptr);

    void setValue(uint16_t byteval);
    NumberEntryWidget::States getState();
    uint16_t getExistingValue();
    uint16_t getNewValue();
    void reset();

protected:
    void focusInEvent(QFocusEvent * event)      override;
    void focusOutEvent(QFocusEvent * event)     override;
    void mousePressEvent(QMouseEvent * event)   override;
    void mouseReleaseEvent(QMouseEvent * event) override;

private slots:
    void onTextEdited(const QString &text);

signals:
   // void valueChanged8(uint8_t  value);
   // void valueChanged16(uint16_t value);
   // void valueChanged32(uint32_t value);
   // void isValidValue(uint8_t fieldNumber, bool tf);
   // void setCurrentField(int fieldNumber);
   void stateChanged(NumberEntryWidget * widget, NumberEntryWidget::States newState);

private:
    uint8_t   _bytect;
    uint16_t  _curval;
    uint16_t  _valueEntered;  // valid value displayed but not yet accepted
    char *    _cls;
    NumberEntryWidget::States
              _widgetState;
};
#endif /* NUMBER_ENTRY_WIDGET_H_ */
