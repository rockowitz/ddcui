/* list_widget_ui.h
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

#ifndef LIST_WIDGET_UI_H_
#define LIST_WIDGET_UI_H_

//class ListWidgetUI {
//public:
//   ListWidgetUI();
//   virtual ~ListWidgetUI();
//};

class Monitor;
class FeatureBaseModel;
class QStackedWidget;
class QWidget;

QWidget * initListWidget(
      Monitor *         curMonitor,
      int               monitorNumber,         // monitor index number
      FeatureBaseModel* baseModel,
      QStackedWidget *  stackedWidget);

#endif /* LIST_WIDGET_UI_H_ */
