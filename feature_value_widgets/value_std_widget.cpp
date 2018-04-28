/* value_std_widget.cpp */

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QRect>

#include "ddcutil_c_api.h"

#include "base/ddcui_globals.h"

#include "value_std_widget.h"


ValueStdWidget::ValueStdWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
    _cls = strdup(metaObject()->className());

    // printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
    _valueField = new QLabel();
    _valueField->setAlignment(Qt::AlignLeft);
    // _valueField->setFrameStyle(QFrame::Sunken | QFrame::Panel);  // now set in ValueBaseWidget
    _valueField->setFixedSize(400,10);

    // QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _valueField->setSizePolicy(*sizePolicy);
    QHBoxLayout * layout = new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(_valueField);
    setLayout(layout);

    // int m_left, m_right, m_top, m_bottom;
    // getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
    // printf("(ValueStdWidget::ValueStdWidget) margins after set: left=%d, top=%d, right=%d, bottom=%d)\n",
    //        m_left, m_right, m_top, m_bottom);

    if (debugLayout) {
       this->setStyleSheet("background-color:magenta;");
       // _valueField->setStyleSheet("background-color:fuschia;");
    }
}


void ValueStdWidget::setValueField() {
    char * s_formatted = NULL;
    DDCA_Non_Table_Vcp_Value valrec;
    valrec.mh = _mh;
    valrec.ml = _ml;
    valrec.sh = _sh;
    valrec.sl = _sl;
    DDCA_Status rc = ddca_format_non_table_vcp_value_by_dref(
          _featureCode,
          _dref,
          &valrec,
          &s_formatted);
    if (rc != 0)
        s_formatted = (char*) "invalid formatted value";   // explicit cast to avoid compiler warning

    _valueField->setText(QString::fromUtf8(s_formatted));
}

void ValueStdWidget::setFeatureValue(const FeatureValue &fv) {
    PRINTFCMF(debugValueWidgetSignals, "Starting. feature code: 0x%02x", fv._feature_code);
    ValueBaseWidget::setFeatureValue(fv);
    setValueField();
}


void ValueStdWidget::setCurrentValue(uint16_t newval) {
    ValueBaseWidget::setCurrentValue(newval);
    setValueField();
}

#ifdef DEBUG_PAINT
void ValueStdWidget::paintEvent(QPaintEvent *event) {
                // printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
                const QRect rect = event->rect();
                // const QRegion = event->region();
                int x;
                int y;
                int width;
                int height;
                rect.getRect(&x, &y, &width, &height);
                printf("(ValueStdWidget::%s) event rectangle: x:%d, y:%d, width:%d, height:%d\n",
                       __func__, x, y, width, height);  fflush(stdout);
                this->ValueBaseWidget::paintEvent(event);
}
#endif
