// ddcui_parms.h

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDCUI_PARMS_H_
#define DDCUI_PARMS_H_

#ifdef TOO_MUCH_INDIRECTION
#define FEATURE_VALUE_MONO_FONT_SIZE  8
#define FEATURE_VALUE_MONO_FONT_FAMILY  "Monospace"
#define FEATURE_VALUE_MONO_FONT_WEIGHT QFont::Normal

#define FEATURE_VALUE_TEXT_FONT_SIZE  8
#define FEATURE_VALUE_TEXT_FONT_FAMILY  "SansSerif"
#define FEATURE_VALUE_TEXT_FONT_WEIGHT QFont::Normal

#define FEATURE_VALUE_BUTTON_FONT_SIZE  8
#define FEATURE_VALUE_BUTTON_FONT_FAMILY  "Serif"
#define FEATURE_VALUE_BUTTON_FONT_WEIGHT QFont::Normal

#define FEATURE_VALUE_NUMBER_ENTRY_FONT_SIZE     8
#define FEATURE_VALUE_NUMBER_ENTRY_FONT_FAMILY  "Monospace"
#define FEATURE_VALUE_NUMBER_ENTRY_FONT_WEIGHT   QFont::Normal

#define FEATURE_VALUE_COMBO_BOX_FONT_SIZE     8
#define FEATURE_VALUE_COMBO_BOX_FONT_FAMILY  "SansSerif"
#define FEATURE_VALUE_COMBO_BOX_FONT_WEIGHT   QFont::Normal

#define FEATURE_VALUE_HEADER_FONT_SIZE     8
#define FEATURE_VALUE_HEADER_FONT_FAMILY  "SansSerif"
#define FEATURE_VALUE_HEADER_FONT_WEIGHT   QFont::Bold

#define MAIN_MENU_FONT_SIZE     10
#define MAIN_MENU_FONT_FAMILY  "DejaVu Sans [Qt Embedded]"
#define MAIN_MENU_FONT_WEIGHT   QFont::Normal
#endif

#define PERSISTENT_SERIAL_MSG_BOX

#define MSGBOX_THREAD_RUN_DELAY_MILLIS 200

extern const bool debugSignals            ;
extern const bool debugValueWidgetSignals ;
extern const bool debugFeatureLists       ;
extern const bool debugFeatureSelection   ;
extern const bool debugNcValues           ;
extern const bool debugLayout             ;
extern const bool debugFeatureDimensions  ;

#endif /* DDCUI_PARMS_H_ */
