#ifndef VERTICAL_SCROLL_AREA_H
#define VERTICAL_SCROLL_AREA_H

// From https://forum.qt.io/topic/13374/solved-qscrollarea-vertical-scroll-only/8

#include <QtWidgets/QScrollArea>

class VerticalScrollArea : public QScrollArea
{
   Q_OBJECT

public:
    explicit VerticalScrollArea(QWidget* parent = 0);

    virtual bool eventFilter(QObject * obj, QEvent * evt);
};

#endif // VERTICAL_SCROLL_AREA_H
