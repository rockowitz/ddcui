/* features_scrollarea_ui.h */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURES_SCROLLAREA_UI_H_
#define FEATURES_SCROLLAREA_UI_H_

// class FeaturesScrollAreaUI {
// };

class Monitor;
class FeatureBaseModel;
class QStackedWidget;
class MsgBoxQueue;

void initFeaturesScrollAreaView(
      Monitor*          curMonitor,
      FeatureBaseModel* baseModel,
      QStackedWidget*   stackedWidget,
      MsgBoxQueue *     msgboxQueue);

#endif /* FEATURES_SCROLLAREA_UI_H_ */
