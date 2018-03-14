#include "value_stacked_widget.h"

ValueStackedWidget::ValueStackedWidget(QWidget *parent):
    ValueAbstractWidget(parent), QStackedWidget(parent)       //
{

    // this->setObjectName(QString::fromUtf8("value_stacked_widget"));   // ambiguous
    // setGeometry(QRect(209,6, 181, 20));



}



void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {

}

void ValueStackedWidget::setCurrentValue(uint16_t newval) {

}

uint16_t ValueStackedWidget::getCurrentValue() {
    return 0;
}

QSize ValueStackedWidget::sizeHint() const {

    return QSize(100,50);
}
