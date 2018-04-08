/* features_scroll_area_contents.h
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

#ifndef FEATURES_SCROLL_AREA_CONTENTS_H_
#define FEATURES_SCROLL_AREA_CONTENTS_H_

#include "nongui/feature_value.h"
#include "nongui/feature_base_model.h"
#include "feature_widget_basic.h"
#include "page_change_observer.h"

class QVBoxLayout;
class QScrollArea;

class FeaturesScrollAreaContents : public QWidget {

   Q_OBJECT

public:
   FeaturesScrollAreaContents(QWidget *parent = Q_NULLPTR);
   virtual ~FeaturesScrollAreaContents();

   void setModel(FeatureBaseModel * baseModel);

   void setContainingScrollArea(QScrollArea * scrollArea);

   const char * _cls;    // className

   void addPageChangeObserver(PageChangeObserver * observer);
   void notifyPageChangeObservers(int pageno);

   QVector<PageChangeObserver*> * _pageChangeObservers;


signals:
void showCentralWidgetByWidget(QWidget * widget);
// void showCentralWidgetByPageno(int pageno);



public slots:

   void featureAdded(FeatureValue fv);
   void featureUpdated(char feature_code);
   void startInitialLoad(void);
   void endInitialLoad();

private:
   FeatureBaseModel * _baseModel = NULL;
   QScrollArea *      _containingScrollArea = NULL;
   // QVBoxLayout *      _curLayout = NULL;

   // quick and dirty for now, eventually replace by hash
   FeatureWidgetBasic * _widgets[256] = {0};

};

#endif /* FEATURES_SCROLL_AREA_CONTENTS_H_ */
