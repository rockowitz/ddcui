// features_scrollarea.h

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURES_SCROLLAREA_H_
#define FEATURES_SCROLLAREA_H_

#include <QtWidgets/QScrollArea>
#include <QtGui/QResizeEvent>


class FeaturesScrollArea : public QScrollArea {
    Q_OBJECT

public:
    FeaturesScrollArea(QWidget *parent = Q_NULLPTR);
    virtual ~FeaturesScrollArea();


    // void addPageChangeObserver(PageChangeObserver * observer);
    // void notifyPageChangeObservers(int pageno);

protected:
    void resizeEvent(QResizeEvent * event) override;

private:
    const char * _cls;

    void layoutWidget();

};

#endif /* FEATURES_SCROLLAREA_H_ */
