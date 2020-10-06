/* value_nc_widget.cpp */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_nc_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>

#include <ddcutil_c_api.h>

#include "c_util/data_structures.h"
#include "base/ddcui_parms.h"
#include "base/core.h"
#include "base/ddca_utils.h"
#include "base/global_state.h"
#include "base/other_options_state.h"
#include "base/widget_debug.h"

#include "nongui/feature_base_model.h"

static bool debugWidget = false;
static bool showDimensionReports = false;
static bool showBasicDims = false || debugFeatureDimensions;
static bool showResizeEvents = false;


void ValueNcWidget::createWidgets() {
   _cb = createFormattedComboBox();   // in ValueBaseWidget

   _extraInfo =  new QLabel("_extraInfo");
   _extraInfo->setMinimumSize(20,10);   // changing has no effect
   _extraInfo->setFrameStyle( QFrame::Plain | QFrame::NoFrame);  // ValueStdWidget has the frame, not Label
   _extraInfo->setFont(FeatureValueTextFont);
   _extraInfo->setIndent(5);

   QSizePolicy extraInfoSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
   extraInfoSizePolicy.setHorizontalStretch(1);
   _extraInfo->setSizePolicy( extraInfoSizePolicy);
}


void ValueNcWidget::layoutWidget() {
    // TRACE("Starting");
    _layout->addSpacing(5);
    _layout->addWidget(_cb);
    // _layout->addStretch(1);
    _layout->addWidget(_extraInfo);
    // _layout->addSpacing(10);
    _layout->setContentsMargins(0,0,0,0);
   // _layout->addStretch(10);     // apply all added size to end

   setLayout(_layout);

   if (debugLayout) {
      static bool dimensionReportShown = false;

      this->setStyleSheet("background-color:cyan;");
      _extraInfo->setStyleSheet("background-color:green;");

      if (showDimensionReports &&!dimensionReportShown) {
           TRACEC("combobox dimensions");
           reportWidgetDimensions(_cb, _cls, __func__);
           TRACEC("ValueNcWidget dimensions");
           reportWidgetDimensions(this, _cls, __func__);
           dimensionReportShown = true;
      }
   }

   static bool basicDimsShown = false;
   if (showBasicDims && !basicDimsShown) {
      REPORT_BASIC_WIDGET_DIMENSIONS(this);
      REPORT_BASIC_WIDGET_DIMENSIONS(this->_cb);

      basicDimsShown = true;
   }
   // TRACE("Done");
}


ValueNcWidget::ValueNcWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    TRACEMCF(debug, "Starting." );

    _layout = new QHBoxLayout();
    createWidgets();
    layoutWidget();
    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );

    TRACEMCF(debug, "Done");
}


void ValueNcWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    debug = debug || debugWidget;
    TRACEMCF(debug, "TRACECMF. ValueNcWidget. featureCode=0x%02x, capVcp=%p, ddcrc=%d",
                    fv.featureCode(), fv.capVcp(), fv.ddcrc());
    TRACEMCF(debug, "Starting. feature 0x%02x, new sl=x%02x, Before ValueBaseWidget::setFeatureValue()",
              fv.featureCode(), fv.val().sl);

    ValueBaseWidget::setFeatureValue(fv);

    _guiChange = false;

    TRACEMCF(debug, "_sl = 0x%02x", _sl);
    _observedValues = bs256_add(_observedValues, _sl);
    if (debug) {
       char * bs1 = bs256_to_string(_observedValues, ""," ");
       TRACEM("_observedValues: %s", bs1);
       free(bs1);
    }
    GlobalState& globalState = GlobalState::instance();
    _curNcValuesSource        = globalState._otherOptionsState->_ncValuesSource;
    _curUseLatestNcValueNames = globalState._otherOptionsState->_useLatestNcValueNames;
    loadComboBox2();

    // _extraInfo->setText("default extra text");
    _extraInfo->setText("");

    _guiChange=true;

    TRACEMCF(debug, "Done");
}

#ifdef OLD
/**
 *  @param  mode
 *  @return dynamically allocated feature value table, caller must free
 */
Local_Feature_Value_Table *
ValueNcWidget::getComboBoxEntries() {
   bool debugFunc = false;
   debugFunc =  debugFunc || debugNcValues;

   NcValuesSource mode = _curNcValuesSource;

   TRACEMF(debugFunc, "feature 0x%02x, mode=%d=%s, _curUseLatestNcValueNames=%s",
                      _featureCode, mode, ncValuesSourceName(mode), SBOOL(_curUseLatestNcValueNames) );

   DDCA_Cap_Vcp dummyCapVcp;
   dummyCapVcp.feature_code = _featureCode;
   dummyCapVcp.value_ct = 0;

   // TRACEMF(debugFunc, "Wolf 1, _finfo=%p", _finfo);
   // TRACEMF(debugFunc, "Wolf 1a, _finfo->sl_values=%p", _finfo->sl_values);
   DDCA_Feature_Value_Entry * entries = _finfo->sl_values;
   // TRACEMF(debugFunc, "Wolf 2");
   Nc_Values_Merge_Mode merge_mode = MccsOnly;  // pointless initialization to avoid -Wmaybe-unitialized
   switch(mode) {
   case NcValuesFromMccs:           merge_mode = MccsOnly;      break;
   case NcValuesFromCapabilities:   merge_mode = CapsOnly;      break;
   case NcValuesFromBoth:           merge_mode = CapsPlusMccs;  break;
   }
   DDCA_Cap_Vcp * capVcp = _capVcp;
   if (!capVcp) {
      TRACEMF(debugFunc, "_featureCode=0x%02x, using dummyCapVcp", _featureCode);
      capVcp = &dummyCapVcp;
   }
   TRACEMF(debugFunc, "Wolf 3");
   Local_Feature_Value_Table * result =
      ddcutil_merge_feature_values(
                capVcp,        // DDCA_Cap_Vcp *, _capVcp or &dummyCapVcp, i.e. feature values from capabilities string
                entries,       // DDCA_Feature_Value_Table, _finfo.sl_values, i.e. feature values/names from MCCS spec
                merge_mode);   // Nc_Values_Merge_Mode

   TRACEMF(debugFunc, "_curUseLatestNcValueNames = %s", SBOOL(_curUseLatestNcValueNames));
   if (_curUseLatestNcValueNames) {
      TRACEMF(debugFunc, "replacing sl values");
      ddcutil_adjust_local_feature_value_names(result, _finfo->latest_sl_values);
   }

   TRACEMF(debugFunc, "Done. Returning: ");
   if (debugFunc)
      ddcutil_dbgrpt_local_feature_value_table(result);

   return result;
}
#endif

#ifdef OLD
void ValueNcWidget::loadComboBox() {
   bool debugFunc = false;
   debugFunc = debugFunc || debugNcValues;

   NcValuesSource mode = _curNcValuesSource;
   TRACEMF(debugFunc, "feature 0x%02x, mode=%d=%s, _useLatestNcValueNames=%s",
                          _featureCode, mode, ncValuesSourceName(mode), SBOOL(_curUseLatestNcValueNames) );

   // In case we're called to reload the combobox values, delete existing values
   for (int ndx = _cb->count()-1; ndx >= 0; ndx--) {
      _cb->removeItem(ndx);
   }

   Local_Feature_Value_Table * table = getComboBoxEntries();
   DDCA_Feature_Value_Entry * cur = table->values;
   if (cur) {
       while (cur->value_name) {
           TRACEMF(debugFunc, "value code: 0x%02x, value_name: %s",
                   cur->value_code, cur->value_name);
           QString s = QString::asprintf("x%02x - %s", cur->value_code, cur->value_name);
           // TRACEMF(debugNcValues, "text being added: %s", QS2S(s));
           _cb->addItem(s, QVariant(cur->value_code));
           cur++;
       }
   }
   ddcutil_free_local_feature_value_table(table);

   // - set current value in combo box
   int cur_ndx = findItem(_sl);
   if (cur_ndx >= 0) {
      TRACEMF(debugFunc, "VCP features 0x%02x, _sl=0x%02x, cur_ndx = %d",
              _featureCode, _sl, cur_ndx);
       _cb->setCurrentIndex(cur_ndx);
       // TRACEMF(debugFunc, "currentIndex after set: %d", _cb->currentIndex());
   }
   else {
       TRACEMF(debugFunc, "VCP feature 0x%02x: Unable to find value 0x%02x", _featureCode, _sl);
       // Add generated entry for observed value
       QString s = QString::asprintf("x%02x - Unrecognized value", _sl);
       _cb->addItem(s, QVariant(_sl));
       cur_ndx = _cb->count()-1;
       _cb->setCurrentIndex(cur_ndx);
   }

   TRACEMF(debugFunc, "Done");
}
#endif

// copied from feature_metadata.c
char *
ValueNcWidget::sl_value_table_lookup(
      DDCA_Feature_Value_Entry * value_entries,
      uint8_t                    value_id)
{
   bool debug = false;
   TRACEMF(debug, "Starting. value_entries=%p, value_id=0x%02x", value_entries, value_id);
   char * result = NULL;
   if (value_entries) {
      DDCA_Feature_Value_Entry *  cur_value = value_entries;
      while (cur_value->value_name != NULL) {
         TRACEMF(debug, "value_code=0x%02x, value_name = %s", cur_value->value_code, cur_value->value_name);
         if (cur_value->value_code == value_id) {
            result = cur_value->value_name;
            // DBGMSG("Found");
            break;
         }
         cur_value++;
      }
   }
   TRACEMF(debug, "Returning %p -> %s", result, result);
   return result;
}


void ValueNcWidget::loadComboBox2() {
   bool debugFunc = false;
   debugFunc = debugFunc || debugNcValues;

   NcValuesSource mode = _curNcValuesSource;
   TRACEMF(debugFunc, "feature 0x%02x, mode=%d=%s, _useLatestNcValueNames=%s",
                          _featureCode, mode, ncValuesSourceName(mode), SBOOL(_curUseLatestNcValueNames) );

   // In case we're called to reload the combobox values, delete existing values
   for (int ndx = _cb->count()-1; ndx >= 0; ndx--) {
      _cb->removeItem(ndx);
   }

   _validValues = EMPTY_BIT_SET_256;
   _validValues = bs256_or(_validValues, _observedValues);
   if (debugFunc) {
      char * bs1 = bs256_to_string(_validValues, ""," ");
      TRACEM("initial _validValues: %s", bs1);
      free(bs1);
      char * bs2 = bs256_to_string(_observedValues, ""," ");
      TRACEM("_observedValues: %s", bs2);
      free(bs2);
;
   }
   if (mode == NcValuesFromCapabilities || mode == NcValuesFromBoth) {
      _validValues = bs256_or(_validValues, bs256_from_cfr(_capVcp));

   }
   if (mode == NcValuesFromMccs || mode == NcValuesFromBoth) {
      DDCA_Feature_Value_Entry * slValues = _finfo->sl_values;
      _validValues = bs256_or(_validValues, bs256_from_sl_values(slValues));
   }
   if (debugFunc) {
      char * bs1 = bs256_to_string(_validValues, "","");
      TRACEM("final _validValues: %s", bs1);
      free(bs1);
   }

   DDCA_Feature_Value_Entry * valueNames = _finfo->sl_values;
   if (_curUseLatestNcValueNames)
      valueNames = _finfo->latest_sl_values;
   Bit_Set_256_Iterator iter = bs256_iter_new(_validValues);
   while(true){
      int iValueCode = bs256_iter_next(iter);
      if (iValueCode < 0)
         break;
      uint8_t valueCode = iValueCode & 0xff;

      TRACEMF(debugFunc, "Adding value 0x%02x to combobox", valueCode);
      char * valueName = sl_value_table_lookup(valueNames, valueCode);
      QString s;
      if (valueName)
         s = QString::asprintf("x%02x - %s", valueCode, valueName);
      else
         s = QString::asprintf("x%02x - Unrecognized value", valueCode);
      TRACEMF(debugFunc, "inserting 0x%02x into combobox: %s", valueCode, QS2S(s));
      _cb->addItem(s, QVariant(valueCode));
   }
   // - set current value in combo box
   int cur_ndx = findItem(_sl);
   assert (cur_ndx >= 0);    // must be in _observedValues
   TRACEMF(debugFunc, "VCP features 0x%02x, _sl=0x%02x, cur_ndx = %d",
           _featureCode, _sl, cur_ndx);
    _cb->setCurrentIndex(cur_ndx);
    // TRACEMF(debugFunc, "currentIndex after set: %d", _cb->currentIndex());

   TRACEMF(debugFunc, "Done");
}




void ValueNcWidget::reloadComboBox(NcValuesSource newSource, bool newUseLatestNames) {
   bool debugFunc = false;
   debugFunc = debugFunc || debugNcValues;
   TRACEMF(debugFunc, "feature=0x%02x, newSource=%d=%s, _curNcValuesSource=%d=%s,",
                      _featureCode,
                       newSource,         ncValuesSourceName(newSource),
                      _curNcValuesSource, ncValuesSourceName(_curNcValuesSource) );
   TRACEMF(debugFunc, "              newUseLatestNames=%s, _curUseLatestNcValueNames=%s",
                      SBOOL(newUseLatestNames),
                      SBOOL(_curUseLatestNcValueNames) );

   if (newSource != _curNcValuesSource || newUseLatestNames != _curUseLatestNcValueNames) {
      _curNcValuesSource = newSource;
      _curUseLatestNcValueNames = newUseLatestNames;
      _guiChange = false;
      loadComboBox2();
      _guiChange = true;
   }

   TRACEMF(debugFunc, "Done");
}


void ValueNcWidget::setCurrentShSl(uint16_t newval) {
   TRACEMF(debugWidget, "Starting. feature 0x%02x, newval=x%04x", _featureCode, newval);

   _guiChange = false;

    ValueBaseWidget::setCurrentShSl(newval);
    if (!bs256_contains(_observedValues, _sl)) {
       bs256_add(_observedValues, _sl);
       loadComboBox2();   // reloads combo box, then sets current value
    }
    else {
       // - set current value in combo box
       int cur_ndx = findItem(_sl);
       if (cur_ndx >= 0) {
           _cb->setCurrentIndex(cur_ndx);
       }
       else {
           TRACEC("Unable to find value 0x%02x", _sl);
       }
    }

    _guiChange = true;
}

// returns -1 if not found
int ValueNcWidget::findItem(uint8_t sl_value) {
    int result = _cb->findData(QVariant(sl_value));
    return result;
}


uint16_t ValueNcWidget::getCurrentShSl() {
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
   bool debug = false;
   debug = debug || debugWidget;
   TRACEMCF(debug, "feature 0x%02x, index=%d", _featureCode, index);
   int ndx = _cb->currentIndex();
   assert(ndx == index);

   QVariant qv = _cb->itemData(ndx);
   uint i = qv.toUInt();
   uint8_t new_sh = 0;
   uint8_t new_sl = i & 0xff;

   if (new_sh != _sh || new_sl != _sl) {
      TRACEMCF(debug, "Value changed.  New sl: %u, _guiChange=%d", new_sl, _guiChange);
      if (_guiChange) {
         TRACEMCF(debug, "Emitting featureValueChanged, featureCode = 0x%02x, sh=0, new_sl=0x%02x",
                         _featureCode, new_sl);
         emit featureValueChanged(_featureCode, 0, new_sl);
      }
      _sh = 0;
      _sl = new_sl;
   }
   else {
      TRACEMCF(debug, "Value not changed.");
   }
}


void ValueNcWidget::resizeEvent(QResizeEvent * evt)
{
   bool show = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool resizeEventsShown = false;
   if (showResizeEvents && !resizeEventsShown) {
      show = true;
      resizeEventsShown = true;
   }

   if (show) {
      TRACEC("old size = %d, %d", oldSz.width(), oldSz.height());
      TRACEC("new size = %d, %d", newSz.width(), newSz.height());
   }

   evt->ignore();
}

