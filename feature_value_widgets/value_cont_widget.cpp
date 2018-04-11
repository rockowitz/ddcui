
#include <string.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>

#include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QVBoxLayout>

#include "base/ddcui_globals.h"
#include "value_cont_widget.h"

static bool debugSignals = false;

ValueContWidget::ValueContWidget(QWidget *parent):
    ValueBaseWidget(parent)
{
   _cls                    = strdup(metaObject()->className());
    // printf("(ValueContWidget::ValueContWidget) Starting.\n" ); fflush(stdout);

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);

    QFont monoValueFont;
    monoValueFont.setPointSize(9);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    QFont nonMonoValueFont;
    nonMonoValueFont.setPointSize(9);

    _curSlider = new QSlider(Qt::Horizontal);
    _curSlider->setFocusPolicy(Qt::StrongFocus);
    _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _curSlider->setSingleStep(1);
    _curSlider->setFixedSize(200,18);
    if (debugLayout)
    _curSlider->setStyleSheet("background-color:pink;");

    _curSpinBox = new QSpinBox();
    _curSpinBox->setSingleStep(1);
    _curSpinBox->setFixedSize(45,18);
    if (debugLayout)
    _curSpinBox->setStyleSheet("background-color:green;");

    _maxTitle = new QLabel("Max:");
    _maxTitle->setFixedSize(35,20);
    _maxTitle->setFont(nonMonoValueFont);
    if (debugLayout)
    _maxTitle->setStyleSheet("background-color:cyan;");

    _maxValue = new QLabel();
    _maxValue->setFont(monoValueFont);
    _maxValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    _maxValue->setFixedSize(30,20);
    if (debugLayout)
    _maxValue->setStyleSheet("background-color:blue;");


    connect( _curSlider,  SIGNAL(valueChanged(int)),
             _curSpinBox, SLOT(  setValue(int)));
    connect(_curSpinBox,  SIGNAL(valueChanged(int)),
            _curSlider,   SLOT(  setValue(int)));

    connect( _curSlider,  SIGNAL(valueChanged(int)),
             this,        SLOT(  onSliderValueChanged(int)));

    connect( _curSlider,  SIGNAL(valueChanged(int)),
             this,        SLOT(  onSliderValueChanged(int)));

    connect( _curSlider,  SIGNAL(sliderReleased()),
             this,        SLOT(  onSliderReleased()));
    connect( _curSpinBox, SIGNAL(valueChanged(int)),
             this,        SLOT(  onSpinBoxValueChanged(int)));
    connect( _curSpinBox, SIGNAL(editingFinished()),
             this,        SLOT(  onSpinBoxEditingFinished()));


    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_curSlider);
    layout->addWidget(_curSpinBox);
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);
    layout->setStretch(1,0);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (debugLayout)
    this->setStyleSheet("background-color:yellow;");

    // int m_left, m_right, m_top, m_bottom;
    // getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
    // printf("(ValueContWidget::ValueContWidget) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
    //        m_left, m_top, m_right, m_bottom);
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

void ValueContWidget::onSliderValueChanged(int value) {
   if (debugSignals)
       printf("(%s::%s) value=%d\n", _cls, __func__, value); fflush(stdout);
}

void ValueContWidget::onSliderReleased() {
   printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
   int curval = _curSpinBox->value();

   uint8_t sh = (curval >> 8) & 0xff;
   uint8_t sl = curval & 0xff;
   if (debugSignals)
       printf("(%s::%s) sh=9x%02x, sl=0x%02x \n", _cls, __func__, sh, sl); fflush(stdout);
   emit featureValueChanged(_feature_code, sh, sl);
}

void ValueContWidget::onSpinBoxValueChanged(int value) {
   if (debugSignals)
       printf("(%s::%s) value=%d\n", _cls, __func__, value); fflush(stdout);
}

void ValueContWidget::onSpinBoxEditingFinished() {
   if (debugSignals)
       printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
}


