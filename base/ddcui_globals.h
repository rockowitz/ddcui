#ifndef DDCUI_GLOBALS_H
#define DDCUI_GLOBALS_H

/* <copyright>
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

#include "config.h"

#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc

const int FeatureWidgetType = QListWidgetItem::UserType+1;

extern const bool debugSignals            ;
extern const bool debugValueWidgetSignals ;
extern const bool debugFeatureLists       ;
extern const bool debugFeatureSelection   ;
extern const bool debugNcValues;
extern const bool debugLayout             ;



#define PRINTFCM(__FMT__, ...) \
   do { \
     printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
     fflush(stdout); \
   } while(0)

#define PRINTFCMF(__FLAG__, __FMT__, ...) \
   if (__FLAG__) { \
      printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
      fflush(stdout); \
   }

inline const char * sbool(bool val) { return (val) ? "true" : "false"; }

#define SBOOL(__v) ( (__v) ? "true" : "false")

// #define APPLY_CANCEL
// #define ALT_MOCK_FEATURES

#ifdef APPLY_CANCEL
const bool useApplyCancel    = true;
#endif
#ifdef ALT_FEATURES
const bool enableAltFeatures = true;
#endif


#endif // DDCUI_GLOBALS_H
