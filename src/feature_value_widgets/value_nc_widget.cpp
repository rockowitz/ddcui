/* value_nc_widget.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_nc_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>

#include <ddcutil_c_api.h>

#include "base/ddca_utils.h"
#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "base/global_state.h"
#include "base/other_options_state.h"

#include "nongui/feature_base_model.h"


static bool dimensionReportShown = false;
static bool debugWidget = false;


ValueNcWidget::ValueNcWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
    _cls = strdup(metaObject()->className());
    // PRINTFCM("Starting." );

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
            PRINTFCM("combobox dimensions");
            reportWidgetDimensions(_cb, _cls, __func__);
            PRINTFCM("ValueNcWidget dimensions");
            reportWidgetDimensions(this, _cls, __func__);
            dimensionReportShown = true;
        }
        this->setStyleSheet("background-color:cyan;");
    }

    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );
}


void ValueNcWidget::setFeatureValue(const FeatureValue &fv) {
    PRINTFCMF(debugWidget, "Starting. feature 0x%02x, new sl=x%02x",
              fv.featureCode(), fv.val().sl);
    ValueBaseWidget::setFeatureValue(fv);

    _guiChange = false;

    GlobalState& globalState = GlobalState::instance();
    NcValuesSource sourceMode = globalState._otherOptionsState->ncValuesSource;
    _curNcValuesSource = sourceMode;
    loadComboBox(sourceMode);

    _guiChange=true;
}


/**
 *  @param  mode
 *  @return dynamically allocated feature value table, caller must free
 */
DDCA_Feature_Value_Entry * ValueNcWidget::getComboBoxEntries(NcValuesSource mode) {
   PRINTFCMF(debugNcValues, "feature 0x%02x, newSource=%d-%s", _featureCode, mode, ncValuesSourceName(mode) );

   DDCA_Feature_Value_Entry * entries = _finfo.sl_values;
   Nc_Values_Merge_Mode merge_mode = MccsOnly;  // pointless initialization to avoid -Wmaybe-unitialized
   switch(mode) {
   case NcValuesFromMccs:           merge_mode = MccsOnly;      break;
   case NcValuesFromCapabilities:   merge_mode = CapsOnly;      break;
   case NcValuesFromBoth:           merge_mode = CapsPlusMccs;  break;
   }
   entries = ddcutil_merge_feature_values(
             _capVcp,             // DDCA_Cap_Vcp *
             entries,             // DDCA_Feature_Value_Table
             merge_mode);         // Nc_Values_Merge_Mode
   return entries;
}


void ValueNcWidget::loadComboBox(NcValuesSource mode) {
   PRINTFCMF(debugNcValues, "feature 0x%02x, newSource=%d-%s", _featureCode, mode, ncValuesSourceName(mode) );

   // In case we're called to reload the combobox values, delete existing values
   for (int ndx = _cb->count()-1; ndx >= 0; ndx--) {
      _cb->removeItem(ndx);
   }

   DDCA_Feature_Value_Entry * table = getComboBoxEntries(mode);
   DDCA_Feature_Value_Entry * cur = table;
   if (cur) {
       while (cur->value_name) {
           // printf("(%s) value code: 0x%02x, value_name: %s\n",
           //        __func__, cur->value_code, cur->value_name);  fflush(stdout);
           QString s;
           s.sprintf("x%02x - %s", cur->value_code, cur->value_name);
           _cb->addItem(s, QVariant(cur->value_code));
           cur++;
       }
   }
   ddcutil_free_dynamic_feature_value_table(table);

   // - set current value in combo box
   int cur_ndx = findItem(_sl);
   if (cur_ndx >= 0) {
       _cb->setCurrentIndex(cur_ndx);
   }
   else {
       PRINTFCM("Unable to find value 0x%02x", _sl);
       // TODO: add generated entry for observed value
       QString s;
       s.sprintf("x%02x - Unrecognized value", _sl);
       _cb->addItem(s, QVariant(_sl));
       cur_ndx = _cb->count()-1;
       _cb->setCurrentIndex(cur_ndx);
   }
}


void ValueNcWidget::reloadComboBox(NcValuesSource newSource) {
   PRINTFCMF(debugNcValues, "newSource=%d-%s, _curNcValuesSource=%d-%s",
         newSource, ncValuesSourceName(newSource),
         _curNcValuesSource, ncValuesSourceName(_curNcValuesSource) );  fflush(stdout);
   if (newSource != _curNcValuesSource) {
      _guiChange = false;
      loadComboBox(newSource);
      _guiChange = true;

      _curNcValuesSource = newSource;
   }
}

void ValueNcWidget::setCurrentValue(uint16_t newval) {
   PRINTFCMF(debugWidget, "Starting. feature 0x%02x, newval=x%04x", _featureCode, newval);

   _guiChange = false;

    ValueBaseWidget::setCurrentValue(newval);

    // - set current value in combo box
    int cur_ndx = findItem(_sl);
    if (cur_ndx >= 0) {
        _cb->setCurrentIndex(cur_ndx);
    }
    else {
        PRINTFCM("Unable to find value 0x%02x", _sl);
    }

    _guiChange = true;
}


int ValueNcWidget::findItem(uint8_t sl_value) {
    int result = _cb->findData(QVariant(sl_value));
    return result;
}


uint16_t ValueNcWidget::getCurrentValue() {
    // get sl from combobox
    int ndx = _cb->currentIndex();
    QVariant qv = _cb->itemData(ndx);
    uint i = qv.toUInt();
    _sh = 0;
    _sl = i & 0xff;

    uint16_t result = (_sh << 8) | _sl;
    return result;
}

void ValueNcWidget::combobox_activated(int index) {
   PRINTFCMF(debugWidget, "feature 0x%02x, index=%d", _featureCode, index);
   int ndx = _cb->currentIndex();
   assert(ndx == index);

   QVariant qv = _cb->itemData(ndx);
   uint i = qv.toUInt();
   uint8_t new_sh = 0;
   uint8_t new_sl = i & 0xff;

   if (new_sh != _sh || new_sl != _sl) {
      PRINTFCMF(debugWidget, "Value changed.  New sl: %u, _guiChange=%d\n", new_sl, _guiChange);
      if (_guiChange)
         emit featureValueChanged(_featureCode, 0, new_sl);
      _sh = 0;
      _sl = new_sl;
   }
   else {
      PRINTFCMF(debugWidget, "Value not changed.");
   }
}


