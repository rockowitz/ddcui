// features_scrollarea.h
//
// NOT BUILT.  Removed from CMakeLists.txt: its resizeEvent() override manually
// shrank the contents width (hardcoded scrollbar guess) without calling the
// base class implementation, fighting QScrollArea's widgetResizable machinery.
// The view now uses a stock QScrollArea.  Retained as reference code.

// Copyright (C) 2019-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURES_SCROLLAREA_H_
#define FEATURES_SCROLLAREA_H_

#include <QScrollArea>
#include <QResizeEvent>

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
    void layoutWidget();

    const char * _cls;
};

#endif /* FEATURES_SCROLLAREA_H_ */
