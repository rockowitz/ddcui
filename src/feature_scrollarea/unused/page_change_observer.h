#ifndef PAGE_CHANGE_OBSERVER_H
#define PAGE_CHANGE_OBSERVER_H

#include <inttypes.h>

class QWidget;

class PageChangeObserver
{
public:
    // PageChangeObserver();
    // virtual ~PageChangeObserver();

    virtual void pageChanged(int pageno) = 0;
    virtual void pageChangedByWidget(QWidget * widget) = 0;
};

#endif // PAGE_CHANGE_OBSERVER_H
