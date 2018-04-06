#include "feature_value_widgets/value_cont_widget.h"

#include <string.h>


#include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QVBoxLayout>


ValueContWidget::ValueContWidget(QWidget *parent):
    ValueBaseWidget(parent)
{
    printf("============> (ValueContWidget::ValueContWidget)\n" ); fflush(stdout);

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);


    _curSlider = new QSlider(Qt::Horizontal);
    _curSlider->setFocusPolicy(Qt::StrongFocus);
    _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _curSlider->setSingleStep(1);
    _curSlider->setFixedSize(200,20);

    _curSpinBox = new QSpinBox();
    _curSpinBox->setSingleStep(1);
    _curSpinBox->setFixedSize(20,20);

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
    layout->setStretch(1,0);
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
