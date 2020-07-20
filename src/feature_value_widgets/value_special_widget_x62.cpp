// value_special_widget_x62.cpp

// Copyright (C) 2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "base/ddcui_parms.h"
#include "core_widgets/enhanced_slider.h"
#include "value_simple_cont_widget.h"
#include "value_special_widget_x62.h"

static bool debugWidget = true;

void ValueSpecialWidgetX62::createInitialWidgets() {
   bool debug = false;
   TRACECF(debug, "Starting.  ValueSpecialWidgetX62::createInitialWidgets()");
   _cb = newFormattedComboBox();

   if (debugLayout)
      _cb->setStyleSheet("background-color:cyan;");

   TRACECF(debug, "Done.  ValueSpecialWidgetX62::createInitialWidgets()");
}


void ValueSpecialWidgetX62::layoutWidget(QHBoxLayout * layout) {
   bool debug = false;
   TRACECF(debug, "Starting. ValueSpecialWidgetX62::layoutWidget" );

    layout->addSpacing(5);
    _cb->setVisible(true);
    layout->addWidget(_cb);

    layout->addWidget(_curSlider);
    layout->addWidget(_curSpinBox);

    layout->addSpacing(5+10);
    // layout->addWidget(spacer);
    layout->addStretch(10);    // take up all the space at the end - stretch factor = 10
    // layout->setContentsMargins(1,0,1,0);    // was 0,0,0,0
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    TRACECF(debug, "Done. ValueSpecialWidgetX62::layoutWidget" );
}


ValueSpecialWidgetX62::ValueSpecialWidgetX62(QWidget *parent)
    : ValueSimpleContWidget(0,255,parent)
//    , _newval(0)
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    // printf("(ValueSpecialWidget62::ValueSpecialWidget62) _cls=%s\n", _cls);
    TRACECF(debug, "Starting.  ValueSpecialWidgetX62" );

    createInitialWidgets();
    delete layout();   // delete layout created by superclass constructor
    QHBoxLayout * layout = new QHBoxLayout();
    layoutWidget(layout);

    loadComboBox();

    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );

    TRACECF(debug, "Done. ValueSpecialWidgetX62");
}


#ifdef REF
void ValueNcWidget::setFeatureValue(const FeatureValue &fv) {
   bool debug = false;
   TRACEMCF(debug, "TRACECMF. ValueNcWidget. featureCode=0x%02x, capVcp=%p, ddcrc=%d",
                 fv.featureCode(), fv.capVcp(), fv.ddcrc());


    TRACEMCF(debugWidget, "Starting. feature 0x%02x, new sl=x%02x, Before ValueBaseWidget::setFeatureValue()",
              fv.featureCode(), fv.val().sl);
    ValueBaseWidget::setFeatureValue(fv);

    _guiChange = false;

    GlobalState& globalState = GlobalState::instance();
    NcValuesSource sourceMode = globalState._otherOptionsState->_ncValuesSource;
    _curNcValuesSource = sourceMode;
    loadComboBox(sourceMode);

    // _extraInfo->setText("default extra text");
    _extraInfo->setText("");

    _guiChange=true;

    TRACEMCF(debug, "Done");
}


void ValueSimpleContWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = debugValueWidgetSignals;
    // debug = true;
    TRACEMCF(debug, "Starting. feature code: 0x%02x, before ValueBaseWidget::setFeatureValue()", fv.featureCode());
    ValueBaseWidget::setFeatureValue(fv);

#ifdef FOR_SUBCLASS
    int _maxval = _mh << 8 | _ml;
#endif
    int curval = _sh << 8 | _sl;

    TRACEMCF(debug,
              "feature=0x%02x, curval=%d, maxval=%d", _featureCode, curval, _maxval);

    // maxval = 99999;   // for testing big numbers
    // curval = 99999;
    _guiChange = false;

    int range = _maxval - _minval;
    _curSlider->setTickInterval(range/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(_maxval);
    if (_minval > 0)
       _curSlider->setMinimum(_minval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(_maxval);
    if (_minval > 0)
       _curSpinBox->setMinimum(_minval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

#ifdef FOR_SUBCLASS
    QString s = QString::number(_maxval);
    _maxValue->setText(s);
#endif

    _guiChange = true;
    TRACEMCF(debug, "Done");
}


#endif



void ValueSpecialWidgetX62::setFeatureValue(const FeatureValue &fv) {
   bool debug = true;

   TRACEMCF(debug, "ValueSpecialWidgetX62. featureCode=0x%02x, capVcp=%p, sh=0x%02x, sl=0x%02x, ddcrc=%d",
                 fv.featureCode(), fv.capVcp(), fv.ddcrc());

    // TRACEMCF(debugWidget, "Starting. feature 0x%02x, new sl=x%02x, Before ValueBaseWidget::setFeatureValue()",
    //           fv.featureCode(), fv.val().sl);
    ValueBaseWidget::setFeatureValue(fv);

    uint16_t initialValue = _sh << 8 | _sl;

    _guiChange = false;   // changes not initiated by GUI, do not signal model change

    int _minval = 1;
    int _maxval = 254;
    assert(initialValue >= _minval);
    assert(initialValue <= _maxval);

    int range = _maxval - _minval;
    _curSlider->setTickInterval(range/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(_maxval);
    if (_minval > 0)
       _curSlider->setMinimum(_minval);

    _curSpinBox->setMaximum(_maxval);
    if (_minval > 0)
       _curSpinBox->setMinimum(_minval);
    //_curSpinBox->setRange(0,maxval);

    int spinsliderValue = initialValue;
    if (initialValue == 0 || initialValue == 255)
       spinsliderValue = 128;

    if (initialValue == 0) {
       TRACEC("initailValue == 0");
       _curSpinBox->setValue(128);
       _cb->setCurrentIndex(0);
       // disable slider and spinbox
       _curSpinBox->setEnabled(false);
       _curSlider->setEnabled(false);
    }

    else if (initialValue == 255) {
       TRACEC("initialValue == 255");
       _cb->setCurrentIndex(2);
       _curSpinBox->setValue(128);
       // disable slider and spinbox
       _curSpinBox->setEnabled(false);
       _curSlider->setEnabled(false);
    }

    else {
       TRACEC("1 <= initialValue <= 255");
       _cb->setCurrentIndex(1);
       _curSpinBox->setValue(initialValue);
       // enable slider and spinbox
       _curSpinBox->setEnabled(true);
       _curSlider->setEnabled(true);
    }

    _guiChange=true;

    TRACEMCF(debug, "Done");
}


#ifdef REF

void ValueNcWidget::loadComboBox(NcValuesSource mode) {
   bool debugFunc = false;
   debugFunc = debugFunc || debugNcValues;
   TRACEMF(debugNcValues, "feature 0x%02x, newSource=%d-%s", _featureCode, mode, ncValuesSourceName(mode) );

   // In case we're called to reload the combobox values, delete existing values
   for (int ndx = _cb->count()-1; ndx >= 0; ndx--) {
      _cb->removeItem(ndx);
   }

   Local_Feature_Value_Table * table = getComboBoxEntries(mode);
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
      TRACEMF(debugNcValues, "VCP features 0x%02x, _sl=0x%02x, cur_ndx = %d",
              _featureCode, _sl, cur_ndx);
       _cb->setCurrentIndex(cur_ndx);
       // TRACEMF(debugFunc, "currentIndex after set: %d", _cb->currentIndex());
   }
   else {
       TRACEMF(debugNcValues, "VCP feature 0x%02x: Unable to find value 0x%02x", _featureCode, _sl);
       // Add generated entry for observed value
       QString s = QString::asprintf("x%02x - Unrecognized value", _sl);
       _cb->addItem(s, QVariant(_sl));
       cur_ndx = _cb->count()-1;
       _cb->setCurrentIndex(cur_ndx);
   }
}
#endif


void ValueSpecialWidgetX62::loadComboBox() {

   _cb->addItem(QString("0: Mute"), QVariant(0) );
   _cb->addItem(QString("1-254: Level"), QVariant(1) );
   _cb->addItem(QString("255 - Default Level"), QVariant(255) );

   _cb->setCurrentIndex(2);
}


void ValueSpecialWidgetX62::setCurrentShSl(uint16_t newval) {
   bool debugFunc = true;
   debugFunc = debugFunc || debugWidget;
   TRACEMF(debugFunc, "Starting. feature 0x%02x, newval=x%04x", _featureCode, newval);

   _guiChange = false;

    ValueBaseWidget::setCurrentShSl(newval);

    newval = newval & 0xff;

    int oldCbIndex = _cb->currentIndex();
    int oldSlider  = _curSlider->value();


    if (newval == 0) {
     //  if (oldCbIndex != 0) {
          TRACEC("newval = 0, oldCbIndex != 0");
          // _guiChange = true;
          _cb->setCurrentIndex(0);
          _curSpinBox->setEnabled(false);
          _curSlider->setEnabled(false);
      // }
    }
    else if (newval == 255) {
   //    if (oldCbIndex != 2) {
          TRACEC("newval = 255, oldCbIndex != 2");
          // _guiChange = true;
          _cb->setCurrentIndex(2);
          _curSpinBox->setEnabled(false);
          _curSlider->setEnabled(false);
   //    }
    }
    else {   // newval = 1.254
    //   if (oldCbIndex != 1) {
          TRACEC("newval = %d, oldCbIndex != 1", newval);
          // _guiChange = true;
          _cb->setCurrentIndex(1);  // ??
          _curSlider->setValue(newval);
          _curSpinBox->setEnabled(true);
          _curSlider->setEnabled(true);
#ifdef OUT
       }

       else {
          TRACEC("newval = %d, oldCbIndex == 1", newval);
          if (_curSlider->value() != newval) {
             // _guiChange = true;
             _curSlider->setValue(newval);
          }
       }
#endif
    }

    _guiChange = true;
}


uint16_t ValueSpecialWidgetX62::getCurrentShSl() {
    // get sl from combobox
    int ndx = _cb->currentIndex();
    QVariant qv = _cb->itemData(ndx);
    uint i = qv.toUInt();

    if (_cb->currentIndex() == 0)
       _sl = 0;
    else if (_cb->currentIndex() == 2)
       _sl = 255;
    else
       _sl = _curSlider->value();

    _sh = 0;
    // _sl = i & 0xff;

    uint16_t result = (_sh << 8) | _sl;
    return result;
}


void ValueSpecialWidgetX62::combobox_activated(int index) {
   bool debug = true;
   debug = debug || debugWidget;
   TRACEMCF(debug, "feature 0x%02x, index=%d", _featureCode, index);
   int ndx = _cb->currentIndex();
   assert(ndx == index);

   // QVariant qv = _cb->itemData(ndx);
   // uint i = qv.toUInt();
   // uint8_t new_sh = 0;
   // uint8_t new_sl = i & 0xff;

   uint8_t new_sh = 0;

   assert(_sh == 0);

   uint8_t new_sl = _curSpinBox->value();
   if (index == 0)
      new_sl = 0;
   else if (index == 2)
      new_sl = 255;

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

// void ValueSpecialWidgetX62::forSpinSliderChanged(uint16_t newval) {
// }

