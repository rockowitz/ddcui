/* features_scroll_area.cpp
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

#include "features_scroll_area.h"

#include <string.h>
#include <stdio.h>


FeaturesScrollArea::FeaturesScrollArea(QWidget * parent) :
    QScrollArea(parent)
{
   _cls = metaObject()->className();

}

FeaturesScrollArea::~FeaturesScrollArea() {
   // TODO Auto-generated destructor stub
}

void FeaturesScrollArea::setModel(FeatureBaseModel * baseModel) {
   _baseModel = baseModel;
}


void FeaturesScrollArea::featureAdded(FeatureValue& fv)
{
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
   fv.report();
}

void FeaturesScrollArea::featureUpdated(char feature_code)
{
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
}

void FeaturesScrollArea::startInitialLoad(void)
{
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
}

void FeaturesScrollArea::endInitialLoad()
{
   printf("(%s::%s)\n", _cls, __func__);  fflush(stdout);
}
