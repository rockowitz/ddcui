// value_ncplus_widget.cpp

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_cnc_widget_x14.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_ncplus_widget.h"

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>

#include <ddcutil_c_api.h>

#include "base/ddca_utils.h"
#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "base/global_state.h"
#include "base/other_options_state.h"

#include "nongui/feature_base_model.h"

static bool dimensionReportShown = false;

#ifdef UNUSED
void ValueNcplusWidget::layoutWidget() {
   bool debug = true;
   TRACEF(debug, "Starting. feature = 0x%02x", _featureCode);
   QFont font;
     font.setPointSize(8);
     QWidget::setFont(font);

    delete _layout;

    _layout = new QHBoxLayout();

    layoutWidgetAux();

#ifdef OLD
    // additional fields
    _auxTitle = new QLabel("_auxTitle");
    _auxDetail = new QLabel("_auxDetail");

    // _layout = new QHBoxLayout();

    _auxTitle->setAlignment(Qt::AlignLeft);
    // _auxTitle->setFrameStyle(QFrame::Sunken | QFrame::Panel);  // now set in ValueBaseWidget
    _auxTitle->setMinimumSize(20,10);
    _auxTitle->setFrameStyle( QFrame::Plain | QFrame::NoFrame);  // ValueStdWidget has the frame, not Label
    _auxTitle->setFont(font);
    _auxTitle->setIndent(5);

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    sizePolicy->setHorizontalStretch(1);
    _auxTitle->setSizePolicy(*sizePolicy);
    // delete sizePolicy;


    _auxDetail->setMinimumSize(20,10);
      _auxDetail->setFrameStyle( QFrame::Plain | QFrame::NoFrame);  // ValueStdWidget has the frame, not Label
      _auxDetail->setFont(font);
      _auxDetail->setIndent(5);

      // QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
      // sizePolicy->setHorizontalStretch(1);
      // _auxDetail->setSizePolicy(*sizePolicy);
      delete sizePolicy;

     // _layout->addSpacing(5);
     _layout->addWidget(_auxTitle);

     // _layout->addWidget(_auxDetail);
     _layout->addStretch(1);


#ifdef APPLY_CANCEL
    if (useApplyCancel) {
       _layout->addWidget(_applyButton);
       _layout->addWidget(_cancelButton);
    }
    else {
       _layout->addSpacing(10);
    }
#else
    // _layout->addSpacing(10);
#endif


     _layout->setContentsMargins(0,0,0,0);
     setLayout(_layout);


     if (debugLayout) {
         if (!dimensionReportShown) {
             TRACE("combobox dimensions");
             reportWidgetDimensions(_cb, _cls, __func__);
             TRACE("ValueNcWidget dimensions");
             reportWidgetDimensions(this, _cls, __func__, "valueNcplusWidget");
             dimensionReportShown = true;
         _auxTitle->setStyleSheet("background-color:orange;");
         _auxDetail->setStyleSheet("background-color:green;");
         this->setStyleSheet("background-color:cyan;");
         }
     }
#endif
     TRACEF(debug, "Done");
}
#endif



ValueNcplusWidget::ValueNcplusWidget(QWidget *parent):
        ValueNcWidget(parent)
{
    _cls = strdup(metaObject()->className());
    // TRACE("Starting." );
    // layoutWidget();
    // TRACE("Done");

}


void ValueNcplusWidget::setAuxFieldsX14()
{
      // how to pull in x14_color_preset_tolerances from vcp_feature_codes.c instead,
     // or the formatted MH value?

   if ( (_vspec.major == 2 && _vspec.minor >= 2) || _vspec.major == 3) {
       char buf0[100];
       // TODO: Convert to QString
       if (_mh == 0x00) {
          strcpy(buf0, "Tolerance: No tolerance specified. (mh=0x00)");
       }
       else if (_mh >= 0x0b) {
          snprintf(buf0, 100, "Tolerance: Invalid tolerance. (mh=0x%02x)", _mh);
       }
       else {
          snprintf(buf0, 100, "Tolerance: %d%%  (MH=0x%02x)", _mh, _mh);
       }

       // QString& mhtext(mh_msg);
       // _mhdesc->setText(mh_msg);
       _extraInfo->setText(QString(buf0));
   }
}

void ValueNcplusWidget::setAuxFieldsXca() {
   if ( (_vspec.major == 2 && _vspec.minor >= 2) ) {
      // _auxTitle->setText("Power Mode:");
      const char * s = NULL;
      switch(_sh) {
      case 0x00:
         s = "Host control of power function unsupported, display may not report power events (sh=x00)";
         break;
      case 0x01:
         s ="Power button disabled, events enabled (sh=x01)";
         break;
      case 0x02:
         s = "Power button enabled, events enabled (xh=x02)";
         break;
      case 0x03:
         s = "Power button disabled, events disabled (sh=x03)";
         break;
      default:
         char buf[40];
         g_snprintf(buf, 40, "Invalid value (sh=x%02x)", _sh);
      }
      _extraInfo->setText(QString(s));
   }
}

void ValueNcplusWidget::setAuxFields() {
   if (_featureCode == 0x14) {
      setAuxFieldsX14();
   }
   else if (_featureCode == 0xca) {
      setAuxFieldsXca();
   }
}

void ValueNcplusWidget::setFeatureValue(const FeatureValue &fv) {
   DDCA_MCCS_Version_Spec vspec = fv.vspec();
   // TRACE("Starting. feature = 0x%02x, vspec = %d.%d", fv.featureCode(), vspec.major, vspec.minor);
   ValueNcWidget::setFeatureValue(fv);
   // do layout here since we don't know util the feature is loaded what the required layout is
   ValueNcWidget::setFeatureValue(fv);
   setAuxFields();
   // TRACE("Done");
}


void ValueNcplusWidget::setCurrentShSl(uint16_t newval) {
   ValueNcWidget::setCurrentShSl(newval);
   setAuxFields();
}

