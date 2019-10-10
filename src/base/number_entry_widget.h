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

public:
    NumberEntryWidget(
          uint8_t bytect,
          QFont   font,
          int     style,
          QSize   size,
          QWidget *parent = nullptr);


private:
    uint8_t _bytect;



};
#endif /* NUMBER_ENTRY_WIDGET_H_ */
