#include <string.h>

#include <QtWidgets>

#include "value_cont_widget.h"

ValueContWidget::ValueContWidget(QWidget *parent):
    ValueBaseWidget(parent)
{
    printf("============> (ValueContWidget::ValueContWidget)\n" ); fflush(stdout);

    _curSlider = new QSlider(Qt::Horizontal);
    _curSlider->setFocusPolicy(Qt::StrongFocus);
    _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _curSlider->setSingleStep(1);

    _curSpinBox = new QSpinBox();
    _curSpinBox->setSingleStep(1);

    _maxTitle = new QLabel("Max:");
    _maxValue = new QLabel();

    connect( _curSlider, SIGNAL(valueChanged(int)),
             _curSpinBox, SLOT(setValue(int)));
    connect(_curSpinBox, SIGNAL(valueChanged(int)),
            _curSlider,   SLOT(setValue(int)));

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_curSlider);
    layout->addWidget(_curSpinBox);
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);
    setLayout(layout);
}


void ValueContWidget::setFeatureValue(const FeatureValue &fv) {
    ValueBaseWidget::setFeatureValue(fv);

    int maxval = _mh << 8 | _ml;
    int curval = _sh << 8 | _sl;

    _curSlider->setTickInterval(maxval/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(maxval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(maxval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

    QString s = QString::number(maxval);
    _maxValue->setText(s);
}


void ValueContWidget::setCurrentValue(uint16_t newval) {
    ValueBaseWidget::setCurrentValue(newval);

     int curval = _sh << 8 | _sl;
    _curSpinBox->setValue(curval);
    _curSlider->setValue(curval);
}


uint16_t ValueContWidget::getCurrentValue() {
    int curval = _curSpinBox->value();

    _sh = (curval >> 8) & 0xff;
    _sl = curval & 0xff;

    uint16_t result = (_sh << 8) | _sl;
    return result;
}
