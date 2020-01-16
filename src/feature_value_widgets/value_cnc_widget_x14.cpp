// value_cnc_widget_x14.cpp

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_cnc_widget_x14.h"
#include "feature_value_widgets/value_nc_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>

#include <ddcutil_c_api.h>

#include "base/core.h"
#include "base/ddca_utils.h"
#include "base/global_state.h"
#include "base/other_options_state.h"
#include "base/widget_debug.h"

#include "nongui/feature_base_model.h"


ValueCncWidgetX14::ValueCncWidgetX14(QWidget *parent):
        ValueNcWidget(parent)
{
    _cls = strdup(metaObject()->className());
    // TRACE("Starting." );

#ifdef REF
   QFont nonMonoFont8;
   nonMonoFont8.setPointSize(8);

   QFont nonMonoFont9;
   nonMonoFont9.setPointSize(9);

    _cb = new QComboBox();

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy->setControlType(QSizePolicy::ComboBox);
    _cb->setSizePolicy(*sizePolicy);
    _cb->setFont(nonMonoFont8);
    _cb->setMaximumHeight(20);
    // whatever the size, large or small, causes big gap between RW and feature value
    _cb->setMaximumWidth(320);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    _cb->setStyleSheet("background-color:white;color:black;");

#ifdef APPLY_CANCEL
    if (useApplyCancel) {
       sizePolicy->setControlType(QSizePolicy::PushButton);
       _applyButton  = new QPushButton("Apply");
       _cancelButton = new QPushButton("Cancel");
       _applyButton->setMaximumSize(55,20);
       _applyButton->setSizePolicy(*sizePolicy);
       _applyButton->setFont(nonMonoFont9);
       _cancelButton->setMaximumSize(55,20);
       _cancelButton->setSizePolicy(*sizePolicy);
       _cancelButton->setFont(nonMonoFont9);
    }
#endif

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addSpacing(5);
    layout->addWidget(_cb);
    layout->addStretch(1);
#ifdef APPLY_CANCEL
    if (useApplyCancel) {
       layout->addWidget(_applyButton);
       layout->addWidget(_cancelButton);
    }
    else {
       layout->addSpacing(10);
    }
#else
    layout->addSpacing(10);
#endif
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (debugLayout) {
        if (!dimensionReportShown) {
            TRACE("combobox dimensions");
            reportWidgetDimensions(_cb, _cls, __func__);
            TRACE("ValueNcWidget dimensions");
            reportWidgetDimensions(this, _cls, __func__);
            dimensionReportShown = true;
        }
        this->setStyleSheet("background-color:cyan;");
    }

    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );

#endif

    _mhlabel = new QLabel("Tolerance:");
    _mhdesc  = new QLabel();


    delete _layout;

    _layout = new QHBoxLayout();
    _layout->addSpacing(5);
    _layout->addWidget(_cb);
    _layout->addStretch(1);

#ifdef UNUSED
    _layout->addWidget(_mhlabel);
#endif
    _layout->addWidget(_mhdesc);
    _layout->addStretch(2);

 #ifdef APPLY_CANCEL
     if (useApplyCancel) {
        _layout->addWidget(_applyButton);
        _layout->addWidget(_cancelButton);
     }
     else {
        _layout->addSpacing(10);
     }
 #else
     _layout->addSpacing(10);
 #endif
     _layout->setContentsMargins(0,0,0,0);
     setLayout(_layout);
}


void     ValueCncWidgetX14::setFeatureValue(const FeatureValue &fv) {
   ValueNcWidget::setFeatureValue(fv);

   const char * mh_msg = "";
    // how to pull in x14_color_preset_tolerances from vcp_feature_codes.c instead,
   // or the formatted MH value?

   char buf0[100];
   // TODO, either define ddcui versions of vcp_version_le(), etc, or put into api
   DDCA_MCCS_Version_Spec vspec = fv.vspec();
   if ( (vspec.major == 2 && vspec.minor >= 2) || vspec.major == 3) {
      if (_mh == 0x00)
         mh_msg = "No tolerance specified. (mh=0x00)";
      else if (_mh >= 0x0b) {
         snprintf(buf0, 100, "Invalid tolerance. (mh=0x%02x)", _mh);
         mh_msg = buf0;
      }
      else {
         snprintf(buf0, 100, "Tolerance: %d%%  (MH=0x%02x)", _mh, _mh);
         mh_msg = buf0;
      }
   }

   // QString& mhtext(mh_msg);
   _mhdesc->setText(mh_msg);
}

// void     ValueCncWidgetX14::setCurrentValue(uint16_t newval) {
// }





