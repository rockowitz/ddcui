/* value_stacked_widget.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_stacked_widget.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

// #include <QtGui/QPaintEvent>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

#include "base/core.h"
#include "base/widget_debug.h"

#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_cnc_widget_x14.h"
#include "feature_value_widgets/value_bytes_widget.h"


static bool dimensionReportShown = false;

ValueStackedWidget::ValueStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , _featureCode(0)              // appease Coverity
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
    _bytesWidget   = new ValueBytesWidget();
    _ncplusWidget  = new ValueNcplusWidget();

    // relying on _pageno_xxx order corresponds to addWidget() order
    _pageno_cont    = 0;
    _pageno_nc      = 1;
    _pageno_std     = 2;
    _pageno_reset   = 3;
    _pageno_2button = 4;
    _pageno_x14     = 5;
    _pageno_bytes   = 6;
    _pageno_ncplus  = 7;

    addWidget(_contWidget);
    addWidget(_ncWidget);
    addWidget(_stdWidget);
    addWidget(_resetWidget);
    addWidget(_2ButtonWidget);
    addWidget(_cncWidgetX14);
    addWidget(_bytesWidget);
    addWidget(_ncplusWidget);

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
                     this,         &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_ncWidget,    &ValueNcWidget::featureValueChanged,
                     this,         &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_ncplusWidget, &ValueNcplusWidget::featureValueChanged,
                     this,         &ValueStackedWidget::forContainedWidgetChanged);

    // QWidget::connect(_cncWidgetX14, &ValueNcWidget::featureValueChanged,
    //                  this,          &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_bytesWidget,  &ValueBaseWidget::featureValueChanged,
                     this,          &ValueStackedWidget::forContainedWidgetChanged);


    QWidget::connect(_resetWidget, &ValueResetWidget::featureValueChanged,
                     this,         &ValueStackedWidget::forContainedWidgetChanged);

#ifdef WORKS
    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(     uint8_t, uint8_t, uint8_t)),
                     curWidget,   SLOT(forContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));


    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(     uint8_t, uint8_t, uint8_t)),
                      curWidget,  SLOT(forContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));
#endif
}


void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    debug = debug || debugValueWidgetSignals;
    TRACEMCF(debug, "Starting. feature code: 0x%02x", fv.featureCode());
    // ValueBaseWidget::setFeatureValue(fv);
    _featureCode = fv.featureCode();   // needed since not calling ValueBaseWidget::setFeatureValue()
    // DDCA_MCCS_Version_Spec vspec = fv.vspec();   // unused

    if (fv.ddcrc() != 0) {
       // use the default standard widget, set in constructor
    }
    // alt, test for PRESET, then xb0 (settings) or normal
    else if ( _featureCode == 0x04 ||    // Restore factory defaults
         _featureCode == 0x05 ||    // Restore factory brightness/contrast defaults
         _featureCode == 0x06 ||    // Restore factory geometry defaults
         _featureCode == 0x08 ||    // Restore factory color defaults
         _featureCode == 0x0a )     // Restore factory TV defaults
    {
       setCurrentIndex(_pageno_reset);
       _cur_stacked_widget = _resetWidget;
    }

    else if (_featureCode == 0x0c) {
       // fv.flags marks it as DDCA_COMPLEX_CONT, but just treat it a normal continuous feature
         // printf("(ValueStackedWidget::%s) x0c\n", __func__); fflush(stdout);
        _pageno_selected = _pageno_cont;
        _cur_stacked_widget = _contWidget;
        setCurrentIndex(_pageno_selected);
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
       TRACECF(debug, "_feature_code == 0x14");
       _pageno_selected = _pageno_ncplus;
       _cur_stacked_widget = _ncplusWidget;
       setCurrentIndex(_pageno_ncplus);
    }

    else if ( _featureCode == 0xca) {
       _pageno_selected = _pageno_ncplus;;
       _cur_stacked_widget = _ncplusWidget;
       setCurrentIndex(_pageno_ncplus);
    }

    else if ( _featureCode >= 0xe0) {
       _pageno_selected = _pageno_bytes;
       _cur_stacked_widget = _bytesWidget;
       setCurrentIndex(_pageno_bytes);
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

    TRACECF(debug, "Calling _cur_stacked_widget->setFeatureValue()" );
    _cur_stacked_widget->setFeatureValue(fv);
    TRACECF(debug, "Done");
}


void ValueStackedWidget::setCurrentValue(uint16_t newval) {
    _cur_stacked_widget->setCurrentShSl(newval);
}

#ifdef UNUSED
uint16_t ValueStackedWidget::getCurrentValue() {
    return _cur_stacked_widget->getCurrentValue();
}
#endif


// QSize ValueStackedWidget::sizeHint() const {
//     // printf("(%s::%s) Starting\n", _cls, __func__);  fflush(stdout);
//     return QSize(100,50);    // ???
// }



void  ValueStackedWidget::forContainedWidgetChanged(uint8_t feature_code, uint8_t sh, uint8_t sl)
{
   bool debug = false;
   debug = debug || debugValueWidgetSignals;
   TRACECF(debug, "feature_code=0x%02x, sh=0x%02x, sl=0x%02x", feature_code, sh, sl);
   assert(feature_code == _featureCode);

   TRACECF(debug,
           "-> Calling emit stackedFeatureValueChanged(), feature_code=0x%02x, sh=0x%02x, sl=0x%02x",
           feature_code, sh, sl);
   emit stackedFeatureValueChanged(feature_code, sh, sl);

   // printf("(%s::%s) Calling simpleFeatueValueNotify() \n", _cls, __func__);  fflush(stdout);
   // simpleFeatureValueNotify(SimpleFeatureValue(feature_code, sh, sl));
}


bool ValueStackedWidget::hasSlTable() {
   bool result = (_pageno_selected == _pageno_nc || _pageno_selected == _pageno_ncplus);
   return result;
}


void ValueStackedWidget::setNcValuesSource(NcValuesSource newsrc) {
   TRACECF(debugNcValues, "newsrc = %d, _pageno_selected=%d, _pageno_nc=%d",
                          newsrc, _pageno_selected, _pageno_nc);

   if (_pageno_selected == _pageno_nc) {
      _ncWidget->reloadComboBox(newsrc);
   }
   TRACECF(debugNcValues, "Done");
}

