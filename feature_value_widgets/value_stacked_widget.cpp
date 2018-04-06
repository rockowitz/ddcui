/* value_stacked_widget.cpp */

#include <QtWidgets/QVBoxLayout>

#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"

#include "feature_value_widgets/value_stacked_widget.h"

ValueStackedWidget::ValueStackedWidget(QWidget *parent):
    ValueBaseWidget(parent)
    // QStackedWidget(parent)       //
{
   _cls                    = metaObject()->className();

    // this->setObjectName(QString::fromUtf8("value_stacked_widget"));   // ambiguous
    // setGeometry(QRect(209,6, 181, 20));

    _contWidget = new ValueContWidget();
    _ncWidget   = new ValueNcWidget();
    _stdWidget  = new ValueStdWidget();

    _pageno_cont = 0;
    _pageno_nc   = 1;
    _pageno_std  = 2;
    _pageno_selected = _pageno_std;    // default

    _stacked = new QStackedWidget();
    _stacked->addWidget(_contWidget);
    _stacked->addWidget(_ncWidget);
    _stacked->addWidget(_stdWidget);

    // ???
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(_stacked);
    setLayout(layout);

    _stacked->setCurrentIndex(_pageno_selected);
    _cur_stacked_widget = _stdWidget;
}



void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {
    ValueBaseWidget::setFeatureValue(fv);

    if (fv._feature_flags & DDCA_STD_CONT) {
        printf("(%s::%s) DDCA_STD_CONT\n", _cls, __func__); fflush(stdout);
        _stacked->setCurrentIndex(_pageno_cont);
        _cur_stacked_widget = _contWidget;
    }
    else if (fv._feature_flags & DDCA_SIMPLE_NC) {
       printf("(%s::%s) DDCA_SIMPLE_NC\n", _cls, __func__); fflush(stdout);
        _stacked->setCurrentIndex(_pageno_nc);
        _cur_stacked_widget = _ncWidget;
    }
    else {
       printf("(%s::%s) default case, _stdWidget\n", _cls, __func__); fflush(stdout);
        _stacked->setCurrentIndex(_pageno_std);
        _cur_stacked_widget = _stdWidget;
    }
    _cur_stacked_widget->setFeatureValue(fv);
}


void ValueStackedWidget::setCurrentValue(uint16_t newval) {
    _cur_stacked_widget->setCurrentValue(newval);
}


uint16_t ValueStackedWidget::getCurrentValue() {
    return _cur_stacked_widget->getCurrentValue();
}

QSize ValueStackedWidget::sizeHint() const {
    // printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
    return QSize(100,50);    // ???
}


void ValueStackedWidget::paintEvent(QPaintEvent *event) {
            printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
            const QRect rect = event->rect();
            // const QRegion = event->region();
            int x;
            int y;
            int width;
            int height;
            rect.getRect(&x, &y, &width, &height);
            printf("(%s::%s) event rectangle: x:%d, y:%d, width:%d, height:%d\n",
                   _cls, __func__, x, y, width, height);  fflush(stdout);

            this->ValueBaseWidget::paintEvent(event);
}
