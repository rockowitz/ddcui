/** @file value_stacked_widget.cpp */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "base/ddcui_core.h"
#include "base/widget_debug.h"
#include "c_util/debug_util.h"
#include "base/ddcui_rtti.h"

#include "feature_value_widgets/value_bytes_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_new_cont_widget.h"
#include "feature_value_widgets/value_std_widget.h"

#include "feature_value_widgets/value_stacked_widget.h"

static bool dimensionReportShown = false;

int ValueStackedWidget::nextId = 0;

ValueStackedWidget::ValueStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , _featureCode(0)              // appease Coverity
{
    bool debug = false;
    _cls = strdup(metaObject()->className());
    _id = ++nextId;
    TRACECF_STARTING(debug, "id=%d", _id);

    // _typedParent = dynamic_cast<FeatureWidget *>(parent);

    // this->setObjectName(QString::fromUtf8("value_stacked_widget"));   // ambiguous
    // setGeometry(QRect(209,6, 181, 20));

    _newContWidget    = new ValueNewContWidget(this);
    _simpleContWidget = new ValueSimpleContWidget(this);
    _ncWidget         = new ValueNcWidget(this);
    _stdWidget        = new ValueStdWidget(this);
    _resetWidget      = new ValueResetWidget(this);
    _2ButtonWidget    = new Value2ButtonWidget(this);
    _bytesWidget      = new ValueBytesWidget(this);
    _ncplusWidget     = new ValueNcplusWidget(this);
    _specialWidgetX62 = new ValueSpecialWidgetX62(this);
    TRACECF_NOPREFIX(debug," _ncWidget->_id=%d, _ncplusWidget._id=%d",
                  _ncWidget->_id,      _ncplusWidget->_id);

    _subwidget[_subwidgetCt++] =         _stdWidget;
    _subwidget[_subwidgetCt++] =     _newContWidget;
    _subwidget[_subwidgetCt++] =  _simpleContWidget;
    _subwidget[_subwidgetCt++] =           _ncWidget;
    _subwidget[_subwidgetCt++] =     _resetWidget;
    _subwidget[_subwidgetCt++] =    _2ButtonWidget;
    _subwidget[_subwidgetCt++] =       _bytesWidget;
    _subwidget[_subwidgetCt++] =     _ncplusWidget;
    _subwidget[_subwidgetCt++] =   _specialWidgetX62;
    assert(_subwidgetCt == ARRAY_SIZE(_subwidget));


    addWidget(_newContWidget);
    addWidget(_ncWidget);
    addWidget(_stdWidget);
    addWidget(_resetWidget);
    addWidget(_2ButtonWidget);
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

    setCurrentWidget(_stdWidget);      // default
    _cur_stacked_widget = _stdWidget;

    // ValueStackedWidget * curWidget = this;  // still treated as ValueBaseWidget* in SIGNAL/SLOT versions

    QWidget::connect(_newContWidget,    &ValueNewContWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_simpleContWidget, &ValueSimpleContWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    // n. only one connection: featureValueChanged is declared once, in ValueBaseWidget;
    // connecting via both the ValueSpecialWidgetX62 and ValueSimpleContWidget names
    // created two connections to the same signal, so each x62 change was delivered
    // (and setvcp issued) twice
    QWidget::connect(_specialWidgetX62, &ValueSpecialWidgetX62::featureValueChanged,
                      this,             &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_ncWidget,         &ValueNcWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

    QWidget::connect(_ncplusWidget,     &ValueNcplusWidget::featureValueChanged,
                     this,              &ValueStackedWidget::forContainedWidgetChanged);

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
      GlobalState::instance()._ncValuesState, &NcValuesState::ncValuesSourceChanged,
      this,                                       &ValueStackedWidget::setNcValuesSource );

   // Initialize based on current UserInterfaceOptionsState value
   bool initialControlKeyRequired =   GlobalState::instance()._uiOptionsState->_controlKeyRequired;
   TRACECF_NOPREFIX(debug, "calling setInstanceControlKeyRequired(%s)", SBOOL(initialControlKeyRequired));
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

    TRACECF_DONE(debug, "");
}


ValueStackedWidget::~ValueStackedWidget() {
   bool debug = false;
   TRACECF_EVENT(debug, "Executing. _cls=%s", _cls);

   // the subwidgets are children of this widget; Qt deletes them

   free((void*) _cls);
}


void ValueStackedWidget::enableSubwidgets() {
   bool debug = false;
   bool enabled = !_instanceControlKeyRequired || _instanceControlKeyPressed;
   TRACEMCF_STARTING(debug, "_id=%d, calling setEnabled(%s) for subwidgets", _id, SBOOL(enabled));
   for (int ndx = 0; ndx < _subwidgetCt; ndx++) {
      _subwidget[ndx]->setEnabled(enabled);
   }
   // if called during initial setup, _cur_stacked_widget apparently wrong
   // _cur_stacked_widget->setEnabled(enabled);

   TRACEMCF_DONE(debug, "");
}


// slot
void ValueStackedWidget::setInstanceControlKeyRequired(bool onoff) {
   bool debug = false;
   debug = debug | debugValueWidgetSignals;
   TRACEMCF_STARTING(debug, "_id=%d, onoff=%s", _id, SBOOL(onoff));

   _instanceControlKeyRequired = onoff;
   enableSubwidgets();

   TRACEMCF_DONE(debug, "");
}

void ValueStackedWidget::setInstanceControlKeyPressed(bool onoff) {
   bool debug = false;
   debug = debug | debugValueWidgetSignals;
   TRACEMCF_STARTING(debug, "_id=%d, onoff=%s", _id, SBOOL(onoff));

   _instanceControlKeyPressed = onoff;
   enableSubwidgets();

   TRACEMCF_DONE(debug, "");
}


static bool vspec_eq(DDCA_MCCS_Version_Spec vspec1, DDCA_MCCS_Version_Spec vspec2) {
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
    TRACEMCF_STARTING(debug, "this._id=%d, feature code: 0x%02x", _id, fv.featureCode());
    // if (debug)
    //    fv.dbgrpt();

    // ValueBaseWidget::setFeatureValue(fv);
    _featureCode = fv.featureCode();   // needed since not calling ValueBaseWidget::setFeatureValue()
    // fv.vspec() asserts a non-null _finfo, which does not exist if the value read failed
    DDCA_MCCS_Version_Spec vspec = {0,0};
    if (fv.finfo())
       vspec = fv.finfo()->vcp_version;

    if (fv.ddcrc() != 0 || !fv.finfo()) {
       // value read failed, or no metadata: use the default standard widget, set in constructor
    }
    // alt, test for PRESET, then xb0 (settings) or normal
    else if ( _featureCode == 0x04 ||    // Restore factory defaults
         _featureCode == 0x05 ||    // Restore factory brightness/contrast defaults
         _featureCode == 0x06 ||    // Restore factory geometry defaults
         _featureCode == 0x08 ||    // Restore factory color defaults
         _featureCode == 0x0a )     // Restore factory TV defaults
    {
       setCurrentWidget(_resetWidget);
       _cur_stacked_widget = _resetWidget;
    }

    else if (_featureCode == 0x0c) {
       // fv.flags marks it as DDCA_COMPLEX_CONT, but just treat it a normal continuous feature
       // printf("(ValueStackedWidget::%s) x0c\n", __func__); fflush(stdout);
        _cur_stacked_widget = _newContWidget;
        setCurrentWidget(_cur_stacked_widget);
     }

    else if (_featureCode == 0x14) {
       TRACEMCF_NOPREFIX(debug, "_feature_code == 0x14");
       _cur_stacked_widget = _ncplusWidget;
       setCurrentWidget(_cur_stacked_widget);
    }

    else if (_featureCode == 0x62    // Audio volume
             &&
             (vspec_eq(vspec, DDCA_VSPEC_V30) || vspec_eq(vspec, DDCA_VSPEC_V22) )
            )
      {
         // TRACEC( "setting _specialWidgetX62");
         _cur_stacked_widget = _specialWidgetX62;
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
       _cur_stacked_widget = _simpleContWidget;
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
       _cur_stacked_widget = _2ButtonWidget;
       setCurrentWidget(_cur_stacked_widget);
    }

    else if ( _featureCode == 0xca) {
       _cur_stacked_widget = _ncplusWidget;
       setCurrentWidget(_cur_stacked_widget);
    }

    else if ( _featureCode >= 0xe0) {
       // DDCA_Cap_Vcp * capvcp = fv.capVcp();
       // TRACEMC("feature 0x%02x, capvcp = %p", _featureCode, capvcp);
       // TRACEMC("feature 0x%02x, value count %d", _featureCode, fv.capVcp()->value_ct);

#ifdef TOO_MANY_EDGE_CASES
       if (capvcp && capvcp->value_ct > 0) {   // segfaults if use fv.capvcp() instead of capvcp
          TRACEMC("capabilities string for feature 0x%02x has vcp value list, treat it as simple NC", _featureCode);
          _cur_stacked_widget = _ncWidget;
          setCurrentWidget(_cur_stacked_widget);
       }
       else {
#endif
          _cur_stacked_widget = _bytesWidget;
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
       _cur_stacked_widget = _contWidget;
       setCurrentWidget(_cur_stacked_widget);
   }
#endif


    else if (fv.flags() & DDCA_STD_CONT) {
         // printf("(ValueStackedWidget::%s) DDCA_STD_CONT\n", __func__); fflush(stdout);
        _cur_stacked_widget = _newContWidget;
        setCurrentWidget(_cur_stacked_widget);
    }
    else if ( (fv.flags() & DDCA_SIMPLE_NC) &&
              (fv.flags() & DDCA_WRITABLE)
            )
    {
        TRACEMCF_NOPREFIX(debug, "DDCA_SIMPLE_NC");
        _cur_stacked_widget = _ncWidget;
        setCurrentWidget(_cur_stacked_widget);
    }
    else {
       TRACEMCF_NOPREFIX(debug, "default case, _stdWidget");

        _cur_stacked_widget = _stdWidget;
        setCurrentWidget(_cur_stacked_widget);
    }

#ifdef NO
    if (_pageno_selected == _pageno_nc || _pageno_selected == _pageno_ncplus) {
       QWidget::connect(GlobalState::instance()._ncValuesState, &NcValuesState::ncValuesSourceChanged,
               _ncWidget,                        &ValueNcWidget::reloadComboBox );
    }
    else if (_pageno_selected == _pageno_ncplus) {
       QWidget::connect(GlobalState::instance()._ncValuesState, &NcValuesState::ncValuesSourceChanged,
               _ncplusWidget,                        &ValueNcWidget::reloadComboBox );
    }
#endif

    TRACECF_NOPREFIX(debug, "Calling _cur_stacked_widget->setFeatureValue()");
    _cur_stacked_widget->setFeatureValue(fv);
    TRACECF_DONE(debug, "");
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
   TRACECF_STARTING(debug, "feature_code=0x%02x, sh=0x%02x, sl=0x%02x", feature_code, sh, sl);
   assert(feature_code == _featureCode);

   TRACECF_NOPREFIX(debug,
           "-> Calling emit stackedFeatureValueChanged(), feature_code=0x%02x, sh=0x%02x, sl=0x%02x",
           feature_code, sh, sl);
   emit stackedFeatureValueChanged(feature_code, sh, sl);

   // printf("(%s::%s) Calling simpleFeatueValueNotify() \n", _cls, __func__);  fflush(stdout);
   // simpleFeatureValueNotify(SimpleFeatureValue(feature_code, sh, sl));
   TRACECF_DONE(debug, "");
}

#ifdef NC_FEATURE_VALUES2
bool ValueStackedWidget::hasSlTable() {
   bool result = (_cur_stacked_widget == _ncWidget || _cur_stacked_widget == _ncplusWidget);
   return result;
}
#endif


void ValueStackedWidget::setNcValuesSource(NcValuesSource newValuesSource, bool newUseLatestNcValueNames) {
   bool debug = false;
   debug = debug || debugNcValues;
   TRACECF_STARTING(debug, "newValuesSource=%d, newUseLatestNcValueNames=%s",
                      newValuesSource, SBOOL(newUseLatestNcValueNames));

   if (_cur_stacked_widget == _ncWidget) {
      _ncWidget->reloadComboBox(newValuesSource, newUseLatestNcValueNames);
   }
   else if (_cur_stacked_widget == _ncplusWidget) {
      _ncplusWidget->reloadComboBox(newValuesSource, newUseLatestNcValueNames);
   }

   TRACECF_DONE(debug, "");
}


void init_value_stacked_widget() {
   bool debug = false;
   DBGF(debug, "Starting");
   RTTI_ADD_METHOD(ValueStackedWidget::ValueStackedWidget);
   RTTI_ADD_METHOD(ValueStackedWidget::~ValueStackedWidget);
   RTTI_ADD_METHOD(ValueStackedWidget::enableSubwidgets);
   RTTI_ADD_METHOD(ValueStackedWidget::setInstanceControlKeyRequired);
   RTTI_ADD_METHOD(ValueStackedWidget::setInstanceControlKeyPressed);
   RTTI_ADD_METHOD(ValueStackedWidget::setFeatureValue);
   RTTI_ADD_METHOD(ValueStackedWidget::forContainedWidgetChanged);
   RTTI_ADD_METHOD(ValueStackedWidget::setNcValuesSource);
   DBGF(debug, "Done");
}

