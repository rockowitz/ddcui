/* hline_widget.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef HLINE_WIDGET_H_
#define HLINE_WIDGET_H_

#include <QtWidgets/QWidget>

#include "base/ddcui_globals.h"

class HLineWidget  : public QFrame {
   Q_OBJECT

public:
   explicit HLineWidget(QWidget * parent = nullptr);
   virtual ~HLineWidget();

private:
   const char * _cls;
};

#endif /* HLINE_WIDGET_H_ */
