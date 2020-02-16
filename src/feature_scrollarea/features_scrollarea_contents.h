// features_scrollarea_contents.h  - Contains rows of FeatureWidgets

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURES_SCROLLAREA_CONTENTS_H_
#define FEATURES_SCROLLAREA_CONTENTS_H_

#include <QtCore/QSize>
#include <QtGui/QResizeEvent>

#include "nongui/feature_value.h"
#include "nongui/feature_base_model.h"

#include "feature_scrollarea/feature_widget.h"
// #include "page_change_observer.h"

class QScrollArea;
class QVBoxLayout;


class FeaturesScrollAreaContents : public QWidget {
    Q_OBJECT

public:
    FeaturesScrollAreaContents(QWidget *parent = Q_NULLPTR);
    virtual ~FeaturesScrollAreaContents();

    void setModel(FeatureBaseModel * baseModel);
    void setContainingScrollArea(QScrollArea * scrollArea);

    // void addPageChangeObserver(PageChangeObserver * observer);
    // void notifyPageChangeObservers(int pageno);

    void resize(int w, int h);
    void resize(QSize sz);


    int maxRowHeight();

protected:
    void resizeEvent(QResizeEvent * event) override;

signals:
    void showCentralWidgetByWidget(QWidget * widget);
    // void showCentralWidgetByPageno(int pageno);

public slots:
#ifdef MAYBE_UNUSED
   void featureAdded(FeatureValue fv);
#endif
#ifdef OLD
   void featureUpdated(char feature_code);
#endif
   void startInitialLoad(void);
   void endInitialLoad();

private:         // member variables
   const char *        _cls;    // className
   FeatureBaseModel *  _baseModel = NULL;
   QScrollArea *       _containingScrollArea = NULL;
#ifdef PAGE_CHANGE_OBSERVER
   QVector<PageChangeObserver*> * _pageChangeObservers;
#endif

   // quick and dirty for now, eventually replace by hash
   // FeatureWidget * _widgets[256] = {0};
};

#endif /* FEATURES_SCROLLAREA_CONTENTS_H_ */
