// debug_utils.h - Functions for debugging Qt layouts

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef BASE_DEBUG_UTILS_H_
#define BASE_DEBUG_UTILS_H_

class QWidget;
class QFrame;
class QLabel;
class QString;

void reportWidgetChildren(  QWidget * w, const char * msg = nullptr);
void reportLabelDimensions( QLabel  * l, const char * className, const char * funcName, const char * msg=nullptr);
void reportFrameDimensions( QFrame  * f, const char * className, const char * funcName, const char * msg=nullptr);
void reportWidgetDimensions(QWidget * w, const char * className, const char * funcName, const char * msg=nullptr);


#endif /* BASE_DEBUG_UTILS_H_ */
