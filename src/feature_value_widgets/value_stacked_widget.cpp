/** @file value_stacked_widget.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_stacked_widget.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

#include "base/core.h"
#include "base/widget_debug.h"

#include "feature_value_widgets/value_bytes_widget.h"
#include "feature_value_widgets/value_cnc_widget_x14.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_new_cont_widget.h"
#include "feature_value_widgets/value_std_widget.h"


static bool dimensionReportShown = false;

int ValueStackedWidget::nextId = 0;

ValueStackedWidget::ValueStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , _featureCode(0)              // appease Coverity
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    _id = ++nextId;
    TRACECF(debug, "Starting. id=%d", _id);

    // _typedParent = dynamic_cast<FeatureWidget *>(parent);

    // this->setObjectName(QString::fromUtf8("value_stacked_widget"));   // ambiguous
    // setGeometry(QRect(209,6, 181, 20));

    _newContWidget    = new ValueNewContWidget(this);
    _simpleContWidget = new ValueSimpleContWidget(this);
    _ncWidget         = new ValueNcWidget(this);
    _stdWidget        = new ValueStdWidget(this);
    _resetWidget      = new ValueResetWidget(this);
    _2ButtonWidget    = new Value2ButtonWidget(this);
    _cncWidgetX14     = new ValueCncWidgetX14(this);
    _bytesWidget      = new ValueBytesWidget(this);
    _ncplusWidget     = new ValueNcplusWidget(this);
    _specialWidgetX62 = new ValueSpecialWidgetX62(this);
    TRACECF(debug," _ncWidget->_id=%d, _cncWidgetX14->_id=%d, _ncplusWidget._id=%d",
                  _ncWidget->_id,      _cncWidgetX14->_id,   _ncplusWidget->_id);

    _subwidget[_subwidgetCt++] =         _stdWidget;
    _subwidget[_subwidgetCt++] =     _newContWidget;
    _subwidget[_subwidgetCt++] =  _simpleContWidget;
    _subwidget[_subwidgetCt++] =           _ncWidget;
    _subwidget[_subwidgetCt++] =     _resetWidget;
    _subwidget[_subwidgetCt++] =    _2ButtonWidget;
    _subwidget[_subwidgetCt++] =     _cncWidgetX14;
    _subwidget[_subwidgetCt++] =       _bytesWidget;
    _subwidget[_subwidgetCt++] =     _ncplusWidget;
    _subwidget[_subwidgetCt++] =   _specialWidgetX62;
    assert(_subwidgetCt == ARRAY_SIZE(_subwidget));


    // relying on _pageno_xxx order corresponds to addWidget() order
    _pageno_cont        = 0;
    _pageno_nc          = 1;
    _pageno_std         = 2;
    _pageno_reset       = 3;
    _pageno_2button     = 4;
    _pageno_x14         = 5;
    _pageno_bytes       = 6;
    _pageno_ncplus      = 7;
    _pageno_simple_cont = 8;
    _pageno_x62         = 9;

    addWidget(_newContWidget);
    addWidget(_ncWidget);
    addWidget(_stdWidget);
    addWidget(_resetWidget);
    addWidget(_2ButtonWidget);
    addWidget(_cncWidgetX14);
    addWidget(_bytesWidget);
    addWidget(_ncplusWidget);
    addWidget(_simpleContWidget);
    addWidget(_specialWidgetX62);

    if (debugLayout) {
        if (!dimensionReportShown) {
            // printf("-------------------------------------------->\n");
            reportFrameDimensions(this, _cls, __func__, "ValueStackedWidget dimensions");
            dimensionReportShown = true;
        }
        this->setStyleSheet("background-color:red;");
    }

    _pageno_selected = _pageno_std;    // default
    // setCurrentIndex(_pageno_selected);
    setCurrentWidget(_stdWidget);
    _cur_stacked_widget = _stdWidget;

    // ValueStackedWidget * curWidget = this;  // still treated as ValueBaseWidget* in SIGNAL/SLOT versions

    QWidget::connect(_newContWidget,    &ValueNewContWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_simpleContWidget, &ValueSimpleContWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_specialWidgetX62, &ValueSpecialWidgetX62::featureValueChanged,
                      this,             &ValueStackedWidget::forContainedWidgetChanged);
    // needed?
    QWidget::connect(_specialWidgetX62, &ValueSimpleContWidget::featureValueChanged,
                      this,             &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_ncWidget,         &ValueNcWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_ncplusWidget,     &ValueNcplusWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    // why disabled?
    // QWidget::connect(_cncWidgetX14,  &ValueNcWidget::featureValueChanged,
    //                  this,           &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_bytesWidget,      &ValueBaseWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);


    QWidget::connect(_resetWidget,     &ValueResetWidget::featureValueChanged,
                     this,             &ValueStackedWidget::forContainedWidgetChanged);

#ifdef WORKS
    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(     uint8_t, uint8_t, uint8_t)),
                     curWidget,   SLOT(forContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));


    QWidget::connect(_ncWidget, SIGNAL(featureValueChanged(     uint8_t, uint8_t, uint8_t)),
                      curWidget,  SLOT(forContainedWidgetChanged(uint8_t, uint8_t, uint8_t)));
#endif

   QWidget::connect(
      GlobalState::instance()._otherOptionsState, &OtherOptionsState::ncValuesSourceChanged,
      this,                                       &ValueStackedWidget::setNcValuesSource );

   // Initialize based on current UserInterfaceOptionsState value
   bool initialControlKeyRequired =   GlobalState::instance()._uiOptionsState->_controlKeyRequired;
   TRACECF(debug, "calling setInstanceControlKeyRequired(%s)", SBOOL(initialControlKeyRequired));
   setInstanceControlKeyRequired(initialControlKeyRequired);

#ifdef NOT_NEEDED
   QWidget::connect(
      GlobalState::instance()._mainWindow, &MainWindow::signalControlKeyRequired,
      this,                                &ValueStackedWidget::setInstanceControlKeyRequired );
#endif

   QWidget::connect(
      GlobalState::instance()._uiOptionsState, &UserInterfaceOptionsState::controlKeyRequired_changed,
      this,                                    &ValueStackedWidget::setInstanceControlKeyRequired );

   QWidget::connect(
      GlobalState::instance()._mainWindow, &MainWindow::signalControlKeyPressed,
      this,                                &ValueStackedWidget::setInstanceControlKeyPressed );

    TRACECF(debug, "Done.");
}


ValueStackedWidget::~ValueStackedWidget() {
   free((void*) _cls);
}


void ValueStackedWidget::enableSubwidgets() {
   bool debug = false;
   bool enabled = !_instanceControlKeyRequired || _instanceControlKeyPressed;
   TRACEMCF(debug, "_id=%d, _pageno_selected=%d, calling setEnabled(%s) for subwidgets",
                   _id, _pageno_selected, SBOOL(enabled));
   for (int ndx = 0; ndx < _subwidgetCt; ndx++) {
      _subwidget[ndx]->setEnabled(enabled);
   }
   // if called during initial setup, _cur_stacked_widget apparently wrong
   // _cur_stacked_widget->setEnabled(enabled);

   TRACEMCF(debug, "Done.");
}


// slot
void ValueStackedWidget::setInstanceControlKeyRequired(bool onoff) {
   bool debug = false;
   debug = debug | debugValueWidgetSignals;
   TRACEMCF(debug, "_id=%d, _pageno_selected=%d, onoff=%s", _id, _pageno_selected, SBOOL(onoff));

   _instanceControlKeyRequired = onoff;
   enableSubwidgets();

   TRACEMCF(debug, "Done.");
}

void ValueStackedWidget::setInstanceControlKeyPressed(bool onoff) {
   bool debug = false;
   debug = debug | debugValueWidgetSignals;
   TRACEMCF(debug, "_id=%d, _pageno_selected=%d, onoff=%s", _id, _pageno_selected, SBOOL(onoff));

   _instanceControlKeyPressed = onoff;
   enableSubwidgets();

   TRACEMCF(debug, "Done.");
}


bool vspec_eq(DDCA_MCCS_Version_Spec vspec1, DDCA_MCCS_Version_Spec vspec2) {
   bool result = false;
   if (vspec1.major == vspec2.major && vspec1.minor == vspec2.minor)
      result = true;
   return result;
}


void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    // debug = debug || (fv.featureCode() == 0xdf);
    // debug = debug || (fv.featureCode() == 0x14);
    debug = debug || debugValueWidgetSignals;
    TRACEMCF(debug, "Starting. this._id=%d, feature code: 0x%02x", _id, fv.featureCode());
    // if (debug)
    //    fv.dbgrpt();

    // ValueBaseWidget::setFeatureValue(fv);
    _featureCode = fv.featureCode();   // needed since not calling ValueBaseWidget::setFeatureValue()
    DDCA_MCCS_Version_Spec vspec = fv.vspec();   // unused

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
       _pageno_selected = _pageno_reset;
       // setCurrentIndex(_pageno_reset);
       setCurrentWidget(_resetWidget);
       _cur_stacked_widget = _resetWidget;
    }

    else if (_featureCode == 0x0c) {
       // fv.flags marks it as DDCA_COMPLEX_CONT, but just treat it a normal continuous feature
       // printf("(ValueStackedWidget::%s) x0c\n", __func__); fflush(stdout);
        _pageno_selected = _pageno_cont;
        _cur_stacked_widget = _newContWidget;
        // setCurrentIndex(_pageno_selected);
        setCurrentWidget(_cur_stacked_widget);
     }

    else if (_featureCode == 0x14) {
       TRACEMCF(debug, "_feature_code == 0x14");
       _pageno_selected = _pageno_ncplus;
       _cur_stacked_widget = _ncplusWidget;
       setCurrentWidget(_cur_stacked_widget);
    }

    else if (_featureCode == 0x62    // Audio volume
             &&
             (vspec_eq(vspec, DDCA_VSPEC_V30) || vspec_eq(vspec, DDCA_VSPEC_V22) )
            )
      {
         // TRACEC( "setting _specialWidgetX62");
         _pageno_selected = _pageno_x62;
         _cur_stacked_widget = _specialWidgetX62;
         // setCurrentIndex(_pageno_selected);
         setCurrentWidget(_cur_stacked_widget);
      }

    else if (  (vspec_eq(vspec, DDCA_VSPEC_V30) ||
                vspec_eq(vspec, DDCA_VSPEC_V22) )
               &&
               ( _featureCode == 0x8f ||     // Audio Treble
                 _featureCode == 0x91 ||     // Audio Bass
                 _featureCode == 0x93 )      // Audio Balance
           )
    {
       _pageno_selected = _pageno_simple_cont;
       _cur_stacked_widget = _simpleContWidget;
       // setCurrentIndex(_pageno_simple_cont);
       setCurrentWidget(_cur_stacked_widget);
       if (_featureCode == 0x93)
          _simpleContWidget->setRange(0x01, 0xfe);
       else
          _simpleContWidget->setRange(0x01, 0xff);
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
       setCurrentWidget(_cur_stacked_widget);
    }

    else if ( _featureCode == 0xca) {
       _pageno_selected = _pageno_ncplus;;
       _cur_stacked_widget = _ncplusWidget;
       // setCurrentIndex(_pageno_ncplus);
       setCurrentWidget(_cur_stacked_widget);
    }

    else if ( _featureCode >= 0xe0) {
       // DDCA_Cap_Vcp * capvcp = fv.capVcp();
       // TRACEMC("feature 0x%02x, capvcp = %p", _featureCode, capvcp);
       // TRACEMC("feature 0x%02x, value count %d", _featureCode, fv.capVcp()->value_ct);

#ifdef TOO_MANY_EDGE_CASES
       if (capvcp && capvcp->value_ct > 0) {   // segfaults if use fv.capvcp() instead of capvcp
          TRACEMC("capabilities string for feature 0x%02x has vcp value list, treat it as simple NC", _featureCode);
          _pageno_selected = _pageno_nc;
          _cur_stacked_widget = _ncWidget;
          setCurrentWidget(_cur_stacked_widget);
       }
       else {
#endif
          _pageno_selected = _pageno_bytes;
          _cur_stacked_widget = _bytesWidget;
          // setCurrentIndex(_pageno_bytes);
          setCurrentWidget(_cur_stacked_widget);
#ifdef TOO_MANY_EDIGE_CASES
       }
#endif
    }

#ifdef OUT
    // *** temp for comparison ***
    else if (_featureCode == 0x12 ||
             _featureCode == 0x16 )
    {
       _pageno_selected = _pageno_old_cont;
       _cur_stacked_widget = _contWidget;
       // setCurrentIndex(_pageno_selected);
       setCurrentWidget(_cur_stacked_widget);
   }
#endif


    else if (fv.flags() & DDCA_STD_CONT) {
         // printf("(ValueStackedWidget::%s) DDCA_STD_CONT\n", __func__); fflush(stdout);
        _pageno_selected = _pageno_cont;
        _cur_stacked_widget = _newContWidget;
        // setCurrentIndex(_pageno_selected);
        setCurrentWidget(_cur_stacked_widget);
    }
    else if ( (fv.flags() & DDCA_SIMPLE_NC) &&
              (fv.flags() & DDCA_WRITABLE)
            )
    {
        TRACEMCF(debug, "DDCA_SIMPLE_NC");
        _pageno_selected = _pageno_nc;
        _cur_stacked_widget = _ncWidget;
        // setCurrentIndex(_pageno_selected);
        setCurrentWidget(_cur_stacked_widget);
    }
    else {
       TRACEMCF(debug, "default case, _stdWidget");

        _pageno_selected = _pageno_std;
        // setCurrentIndex(_pageno_selected);
        _cur_stacked_widget = _stdWidget;
        setCurrentWidget(_cur_stacked_widget);
    }

#ifdef NO
    if (_pageno_selected == _pageno_nc || _pageno_selected == _pageno_ncplus) {
       QWidget::connect(GlobalState::instance()._otherOptionsState, &OtherOptionsState::ncValuesSourceChanged,
               _ncWidget,                        &ValueNcWidget::reloadComboBox );
    }
    else if (_pageno_selected == _pageno_ncplus) {
       QWidget::connect(GlobalState::instance()._otherOptionsState, &OtherOptionsState::ncValuesSourceChanged,
               _ncplusWidget,                        &ValueNcWidget::reloadComboBox );
    }
#endif

    TRACECF(debug, "Calling _cur_stacked_widget->setFeatureValue(), _pageno_selected=%d", _pageno_selected );
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

#ifdef NC_FEATURE_VALUES2
bool ValueStackedWidget::hasSlTable() {
   bool result = (_pageno_selected == _pageno_nc || _pageno_selected == _pageno_ncplus);
   return result;
}
#endif


void ValueStackedWidget::setNcValuesSource(NcValuesSource newValuesSource, bool newUseLatestNcValueNames) {
   bool debug = false;
   debug = debug || debugNcValues;
   TRACECF(debug, "newValuesSource=%d, newUseLatestNcValueNames=%s, _pageno_selected=%d, _pageno_nc=%d, _pageno_ncplus=%d",
                      newValuesSource, SBOOL(newUseLatestNcValueNames), _pageno_selected, _pageno_nc, _pageno_ncplus);

   // if (_pageno_selected == _pageno_nc) {
   if (_cur_stacked_widget == _ncWidget) {
      _ncWidget->reloadComboBox(newValuesSource, newUseLatestNcValueNames);
   }
   // else if (_pageno_selected == _pageno_ncplus) {
   else if (_cur_stacked_widget == _ncplusWidget) {
      _ncplusWidget->reloadComboBox(newValuesSource, newUseLatestNcValueNames);
   }

   TRACECF(debug, "Done");
}

