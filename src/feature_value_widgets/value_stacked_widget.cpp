/* value_stacked_widget.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_stacked_widget.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

// #include <QtGui/QPaintEvent>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

// #include "nongui/simple_feature_value.h"
// #include "nongui/simple_feature_value_subject.h"

#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_cnc_widget_x14.h"


static bool dimensionReportShown = false;

ValueStackedWidget::ValueStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , _featureCode(0)              // appease Coverity

    //    QWidget(parent)
    // ValueBaseWidget(parent)

    // , SimpleFeatureValueSubject()
{
    _cls = strdup(metaObject()->className());

    // this->setObjectName(QString::fromUtf8("value_stacked_widget"));   // ambiguous
    // setGeometry(QRect(209,6, 181, 20));

    _contWidget    = new ValueContWidget();
    _ncWidget      = new ValueNcWidget();
    _stdWidget     = new ValueStdWidget();
    _resetWidget   = new ValueResetWidget();
    _2ButtonWidget = new Value2ButtonWidget();
    _cncWidgetX14  = new ValueCncWidgetX14();

    // relying on _pageno_xxx order corresponds to addWidget() order
    _pageno_cont    = 0;
    _pageno_nc      = 1;
    _pageno_std     = 2;
    _pageno_reset   = 3;
    _pageno_2button = 4;
    _pageno_x14     = 5;

    addWidget(_contWidget);
    addWidget(_ncWidget);
    addWidget(_stdWidget);
    addWidget(_resetWidget);
    addWidget(_2ButtonWidget);
    addWidget(_cncWidgetX14);

    if (debugLayout) {
        if (!dimensionReportShown) {
            // printf("-------------------------------------------->\n");
            reportFrameDimensions(this, _cls, __func__, "ValueStackedWidget dimensions");
            dimensionReportShown = true;
        }
        this->setStyleSheet("background-color:red;");
    }

    _pageno_selected = _pageno_std;    // default
    setCurrentIndex(_pageno_selected);
    _cur_stacked_widget = _stdWidget;

    // ValueStackedWidget * curWidget = this;  // still treated as ValueBaseWidget* in SIGNAL/SLOT versions

    QWidget::connect(_contWidget,  &ValueContWidget::featureValueChanged,
                     this,         &ValueStackedWidget::onContainedWidgetChanged);

    QWidget::connect(_ncWidget,    &ValueNcWidget::featureValueChanged,
                     this,         &ValueStackedWidget::onContainedWidgetChanged);

    QWidget::connect(_cncWidgetX14, &ValueNcWidget::featureValueChanged,
                     this,          &ValueStackedWidget::onContainedWidgetChanged);


    QWidget::connect(_resetWidget, &ValueResetWidget::featureValueChanged,
                     this,         &ValueStackedWidget::onContainedWidgetChanged);

#ifdef WORKS
    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(     uint8_t, uint8_t, uint8_t)),
                     curWidget,   SLOT(onContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));


    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(     uint8_t, uint8_t, uint8_t)),
                      curWidget,  SLOT(onContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));
#endif
}


void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    debug = debug || debugValueWidgetSignals;
    PRINTFCMF(debug, "Starting. feature code: 0x%02x", fv.featureCode());
    // ValueBaseWidget::setFeatureValue(fv);
    _featureCode = fv.featureCode();   // needed since not calling ValueBaseWidget::setFeatureValue()

    // alt, test for PRESET, then xb0 (settings) or normal
    if ( _featureCode == 0x04 ||    // Restore factory defaults
         _featureCode == 0x05 ||    // Restore factory brightness/contrast defaults
         _featureCode == 0x06 ||    // Restore factory geometry defaults
         _featureCode == 0x08 ||    // Restore factory color defaults
         _featureCode == 0x0a )     // Restore factory TV defaults
    {
       setCurrentIndex(_pageno_reset);
       _cur_stacked_widget = _resetWidget;
    }

    else if (_featureCode == 0xb0) {
       // printf("(%s::%s) B0\n", _cls, __func__);
       _2ButtonWidget->setButtonDetail(
             QString("Store"),
             1,
             QString("Restore"),
             2);
       _pageno_selected = _pageno_2button;
       _cur_stacked_widget = _2ButtonWidget;
       setCurrentIndex(_pageno_selected);
    }

    else if (_featureCode == 0x14) {
       PRINTFCMF(debug, "_feature_code == 0x14");
       _pageno_selected = _pageno_x14;
       _cur_stacked_widget = _cncWidgetX14;
       setCurrentIndex(_pageno_x14);
    }


    else if (fv.flags() & DDCA_STD_CONT) {
         // printf("(ValueStackedWidget::%s) DDCA_STD_CONT\n", __func__); fflush(stdout);
        _pageno_selected = _pageno_cont;
        _cur_stacked_widget = _contWidget;
        setCurrentIndex(_pageno_selected);
    }
    else if ( (fv.flags() & DDCA_SIMPLE_NC) &&
              (fv.flags() & DDCA_WRITABLE)
            )
    {
        // printf("(ValueStackedWidget::%s) DDCA_SIMPLE_NC\n", __func__); fflush(stdout);
        _pageno_selected = _pageno_nc;
        _cur_stacked_widget = _ncWidget;
        setCurrentIndex(_pageno_selected);
    }
    else {
        // printf("(ValueStackedWidget::%s) default case, _stdWidget\n",  __func__); fflush(stdout);

        _pageno_selected = _pageno_std;
        setCurrentIndex(_pageno_selected);
        _cur_stacked_widget = _stdWidget;
    }

    PRINTFCMF(debug, "Calling _cur_stacked_widget->setFeatureValue()" );
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



void  ValueStackedWidget::onContainedWidgetChanged(uint8_t feature_code, uint8_t sh, uint8_t sl)
{
   bool debug = debugValueWidgetSignals;
   debug = true;
   PRINTFCMF(debug,
             "feature_code=0x%02x, sh=0x%02x, sl=0x%02x", feature_code, sh, sl);
   assert(feature_code == _featureCode);

   PRINTFCMF(debug,
             "Calling emit stackedFeatureValueChanged(), feature_code=0x%02x, sh=0x%02x, sl=0x%02x",
             feature_code, sh, sl);
   emit stackedFeatureValueChanged(feature_code, sh, sl);

   // printf("(%s::%s) Calling simpleFeatueValueNotify() \n", _cls, __func__);  fflush(stdout);
   // simpleFeatureValueNotify(SimpleFeatureValue(feature_code, sh, sl));
}


bool ValueStackedWidget::isSimpleNc() {
   bool result = (_pageno_selected == _pageno_nc);
   return result;
}


void ValueStackedWidget::setNcValuesSource(NcValuesSource newsrc) {
   PRINTFCMF(debugNcValues,
             "newsrc = %d, _pageno_selected=%d, _pageno_nc=%d",
             newsrc, _pageno_selected, _pageno_nc);

   if (_pageno_selected == _pageno_nc) {
      _ncWidget->reloadComboBox(newsrc);
   }
   PRINTFCMF(debugNcValues, "Done");
}

