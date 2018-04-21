/* value_stacked_widget.cpp */

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtGui/QPaintEvent>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "nongui/simple_feature_value.h"
#include "nongui/simple_feature_value_subject.h"

#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"

#include "feature_value_widgets/value_stacked_widget.h"

static bool dimensionReportShown = false;

static bool debugSignals = false;

ValueStackedWidget::ValueStackedWidget(QWidget *parent):
    ValueBaseWidget(parent)  , SimpleFeatureValueSubject()

    // QStackedWidget(parent)       //
{
   _cls                    = strdup(metaObject()->className());

    // this->setObjectName(QString::fromUtf8("value_stacked_widget"));   // ambiguous
    // setGeometry(QRect(209,6, 181, 20));

    _contWidget    = new ValueContWidget();
    _ncWidget      = new ValueNcWidget();
    _stdWidget     = new ValueStdWidget();
    _resetWidget   = new ValueResetWidget();
    _2ButtonWidget = new Value2ButtonWidget();

    _pageno_cont    = 0;
    _pageno_nc      = 1;
    _pageno_std     = 2;
    _pageno_reset   = 3;
    _pageno_2button = 4;
    _pageno_selected = _pageno_std;    // default

    _stacked = new QStackedWidget();
    _stacked->addWidget(_contWidget);
    _stacked->addWidget(_ncWidget);
    _stacked->addWidget(_stdWidget);
    _stacked->addWidget(_resetWidget);
    _stacked->addWidget(_2ButtonWidget);

    // ???
    QVBoxLayout * layout = new QVBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(_stacked);
    setLayout(layout);

    if (!dimensionReportShown && debugLayout) {
        printf("-------------------------------------------->\n"); fflush(stdout);
        reportWidgetDimensions(this, _cls, __func__);
        dimensionReportShown = true;
    }

    if (debugLayout)
       this->setStyleSheet("background-color:red;");

    _stacked->setCurrentIndex(_pageno_selected);
    _cur_stacked_widget = _stdWidget;

    ValueStackedWidget * curWidget = this;  // still treated as ValueBaseWidget* in SIGNAL/SLOT versions

    QWidget::connect(_contWidget,  &ValueContWidget::featureValueChanged,
                     curWidget,    &ValueStackedWidget::onContainedWidgetChanged);

    QWidget::connect(_ncWidget,    &ValueContWidget::featureValueChanged,
                     this,         &ValueStackedWidget::onContainedWidgetChanged);

    QWidget::connect(_resetWidget, &ValueResetWidget::featureValueChanged,
                     this,         &ValueStackedWidget::onContainedWidgetChanged);

#ifdef WORKS
    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(   uint8_t, uint8_t, uint8_t)),
                     curWidget,      SLOT(onContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));


    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(   uint8_t, uint8_t, uint8_t)),
                      curWidget,     SLOT(onContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));
#endif


}



void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {
    ValueBaseWidget::setFeatureValue(fv);

    // alt, test for PRESET, then xb0 (settings) or normal
    if ( fv._feature_code == 0x04 ||    // Restore factory defaults
         fv._feature_code == 0x05 ||    // Restore factory brightness/contrast defaults
         fv._feature_code == 0x06 ||    // Restore factory geometry defaults
         fv._feature_code == 0x08 ||    // Restore factory color defaults
         fv._feature_code == 0x0a )     // Restore factory TV defaults
    {
       _stacked->setCurrentIndex(_pageno_reset);
       _cur_stacked_widget = _resetWidget;
    }

    else if (fv._feature_code == 0xb0) {
       printf("(%s::%s) B0\n", _cls, __func__);
       _2ButtonWidget->setButtonDetail(
             QString("Store"),
             1,
             QString("Restore"),
             2);
       _stacked->setCurrentIndex(_pageno_2button);
       _cur_stacked_widget = _2ButtonWidget;
    }

    else if (fv.flags() & DDCA_STD_CONT) {
        // printf("(ValueStackedWidget::%s) DDCA_STD_CONT\n", __func__); fflush(stdout);
        _stacked->setCurrentIndex(_pageno_cont);
        _cur_stacked_widget = _contWidget;
    }
    else if ( (fv.flags() & DDCA_SIMPLE_NC) &&
              (fv.flags() & DDCA_WRITABLE)
            )
    {
       // printf("(ValueStackedWidget::%s) DDCA_SIMPLE_NC\n", __func__); fflush(stdout);
        _stacked->setCurrentIndex(_pageno_nc);
        _cur_stacked_widget = _ncWidget;
    }
    else {
       // printf("(ValueStackedWidget::%s) default case, _stdWidget\n",  __func__); fflush(stdout);
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

// QSize ValueStackedWidget::sizeHint() const {
//     // printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
//     return QSize(100,50);    // ???
// }

#ifdef OLD
void ValueStackedWidget::paintEvent(QPaintEvent *event) {
            // printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
            const QRect rect = event->rect();
            // const QRegion = event->region();
            int x;
            int y;
            int width;
            int height;
            rect.getRect(&x, &y, &width, &height);
            // _cls points to base class name, not this class
            // printf("(ValueStackedWidget::%s) event rectangle: x:%d, y:%d, width:%d, height:%d\n",
            //       __func__, x, y, width, height);  fflush(stdout);

            this->ValueBaseWidget::paintEvent(event);
}
#endif


void  ValueStackedWidget::onContainedWidgetChanged(uint8_t feature_code, uint8_t sh, uint8_t sl) {
   if (debugSignals)
       printf("(%s::%s) feature_code=0x%02x, sh=0x%02x, sl=0x%02x\n",
              _cls, __func__, feature_code, sh, sl);  fflush(stdout);
   assert(feature_code == _feature_code);

   // if (debugSignals)
       // printf("(%s::%s) Calling emit featureValueChanged()\n", _cls, __func__);  fflush(stdout);
   // emit featureValueChanged(feature_code, sh, sl);
   if (debugSignals)
       printf("(%s::%s) Calling emit stackedFeatureValueChanged()\n", _cls, __func__);  fflush(stdout);
   emit stackedFeatureValueChanged(feature_code, sh, sl);

   // printf("(%s::%s) Calling simpleFeatueValueNotify() \n", _cls, __func__);  fflush(stdout);
   // simpleFeatureValueNotify(SimpleFeatureValue(feature_code, sh, sl));

}




