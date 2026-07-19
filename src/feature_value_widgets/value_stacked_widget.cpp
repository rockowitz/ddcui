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
#include "base/global_state.h"
#include "base/widget_debug.h"
#include "c_util/debug_util.h"
#include "base/ddcui_rtti.h"

#include "feature_value_widgets/value_2button_widget.h"
#include "feature_value_widgets/value_bytes_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_ncplus_widget.h"
#include "feature_value_widgets/value_new_cont_widget.h"
#include "feature_value_widgets/value_reset_widget.h"
#include "feature_value_widgets/value_simple_cont_widget.h"
#include "feature_value_widgets/value_special_widget_x62.h"
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

    // The value widget appropriate to the feature is created by setFeatureValue()

    if (debugLayout) {
        if (!dimensionReportShown) {
            // printf("-------------------------------------------->\n");
            reportFrameDimensions(this, _cls, __func__, "ValueStackedWidget dimensions");
            dimensionReportShown = true;
        }
        this->setStyleSheet("background-color:red;");
    }

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

   // the value widget is a child of this widget; Qt deletes it

   free((void*) _cls);
}


void ValueStackedWidget::enableCurrentWidget() {
   bool debug = false;
   bool enabled = !_instanceControlKeyRequired || _instanceControlKeyPressed;
   TRACEMCF_STARTING(debug, "_id=%d, calling setEnabled(%s)", _id, SBOOL(enabled));
   // NULL until setFeatureValue() has created the widget; setFeatureValue()
   // applies the accumulated state at creation
   if (_cur_stacked_widget)
      _cur_stacked_widget->setEnabled(enabled);
   TRACEMCF_DONE(debug, "");
}


// slot
void ValueStackedWidget::setInstanceControlKeyRequired(bool onoff) {
   bool debug = false;
   debug = debug | debugValueWidgetSignals;
   TRACEMCF_STARTING(debug, "_id=%d, onoff=%s", _id, SBOOL(onoff));

   _instanceControlKeyRequired = onoff;
   enableCurrentWidget();

   TRACEMCF_DONE(debug, "");
}

void ValueStackedWidget::setInstanceControlKeyPressed(bool onoff) {
   bool debug = false;
   debug = debug | debugValueWidgetSignals;
   TRACEMCF_STARTING(debug, "_id=%d, onoff=%s", _id, SBOOL(onoff));

   _instanceControlKeyPressed = onoff;
   enableCurrentWidget();

   TRACEMCF_DONE(debug, "");
}


static bool vspec_eq(DDCA_MCCS_Version_Spec vspec1, DDCA_MCCS_Version_Spec vspec2) {
   bool result = false;
   if (vspec1.major == vspec2.major && vspec1.minor == vspec2.minor)
      result = true;
   return result;
}


/** Creates the value widget appropriate to the feature and makes it current.
 *
 *  Called once per instance, from the FeatureWidget constructor.
 */
void ValueStackedWidget::setFeatureValue(const FeatureValue &fv) {
    bool debug = false;
    // debug = debug || (fv.featureCode() == 0xdf);
    // debug = debug || (fv.featureCode() == 0x14);
    debug = debug || debugValueWidgetSignals;
    TRACEMCF_STARTING(debug, "this._id=%d, feature code: 0x%02x", _id, fv.featureCode());
    // if (debug)
    //    fv.dbgrpt();

    _featureCode = fv.featureCode();
    // fv.vspec() asserts a non-null _finfo, which does not exist if the value read failed
    DDCA_MCCS_Version_Spec vspec = {0,0};
    if (fv.finfo())
       vspec = fv.finfo()->vcp_version;

    // Not expected, but if called again discard the previously created widget
    if (_cur_stacked_widget) {
       removeWidget(_cur_stacked_widget);
       delete _cur_stacked_widget;
       _cur_stacked_widget = nullptr;
    }

    ValueBaseWidget * w = nullptr;

    // ValueStdWidget is display-only and never emits featureValueChanged.
    bool connectValueChanged = true;

    if (fv.ddcrc() != 0 || !fv.finfo()) {
       // value read failed, or no metadata: use the standard display-only widget
       w = new ValueStdWidget(this);
       connectValueChanged = false;
    }
    // alt, test for PRESET, then xb0 (settings) or normal
    else if ( _featureCode == 0x04 ||    // Restore factory defaults
         _featureCode == 0x05 ||    // Restore factory brightness/contrast defaults
         _featureCode == 0x06 ||    // Restore factory geometry defaults
         _featureCode == 0x08 ||    // Restore factory color defaults
         _featureCode == 0x0a )     // Restore factory TV defaults
    {
       w = new ValueResetWidget(this);
    }

    else if (_featureCode == 0x0c) {
       // fv.flags marks it as DDCA_COMPLEX_CONT, but just treat it a normal continuous feature
       w = new ValueNewContWidget(this);
    }

    else if (_featureCode == 0x14) {
       TRACEMCF_NOPREFIX(debug, "_feature_code == 0x14");
       w = new ValueNcplusWidget(this);
    }

    else if (_featureCode == 0x62    // Audio volume
             &&
             (vspec_eq(vspec, DDCA_VSPEC_V30) || vspec_eq(vspec, DDCA_VSPEC_V22) )
            )
    {
       w = new ValueSpecialWidgetX62(this);
    }

    else if (  (vspec_eq(vspec, DDCA_VSPEC_V30) ||
                vspec_eq(vspec, DDCA_VSPEC_V22) )
               &&
               ( _featureCode == 0x8f ||     // Audio Treble
                 _featureCode == 0x91 ||     // Audio Bass
                 _featureCode == 0x93 )      // Audio Balance
           )
    {
       ValueSimpleContWidget * scw = new ValueSimpleContWidget(this);
       if (_featureCode == 0x93)
          scw->setRange(0x01, 0xfe);
       else
          scw->setRange(0x01, 0xff);
       w = scw;
    }

    else if (_featureCode == 0xb0) {
       Value2ButtonWidget * bw = new Value2ButtonWidget(this);
       bw->setButtonDetail(
             QString("Store"),
             1,
             QString("Restore"),
             2);
       w = bw;
       // n. unlike the earlier eager-creation design, the widget's
       // featureValueChanged signal is connected, so the Store/Restore
       // buttons actually issue a setvcp
    }

    else if ( _featureCode == 0xca) {
       w = new ValueNcplusWidget(this);
    }

    else if ( _featureCode >= 0xe0) {
       // DDCA_Cap_Vcp * capvcp = fv.capVcp();
       // TRACEMC("feature 0x%02x, capvcp = %p", _featureCode, capvcp);
       // TRACEMC("feature 0x%02x, value count %d", _featureCode, fv.capVcp()->value_ct);

#ifdef TOO_MANY_EDGE_CASES
       if (capvcp && capvcp->value_ct > 0) {   // segfaults if use fv.capvcp() instead of capvcp
          TRACEMC("capabilities string for feature 0x%02x has vcp value list, treat it as simple NC", _featureCode);
          w = new ValueNcWidget(this);
       }
       else {
#endif
          w = new ValueBytesWidget(this);
#ifdef TOO_MANY_EDIGE_CASES
       }
#endif
    }

    else if (fv.flags() & DDCA_STD_CONT) {
        // printf("(ValueStackedWidget::%s) DDCA_STD_CONT\n", __func__); fflush(stdout);
        w = new ValueNewContWidget(this);
    }
    else if ( (fv.flags() & DDCA_SIMPLE_NC) &&
              (fv.flags() & DDCA_WRITABLE)
            )
    {
        TRACEMCF_NOPREFIX(debug, "DDCA_SIMPLE_NC");
        w = new ValueNcWidget(this);
    }
    else {
       TRACEMCF_NOPREFIX(debug, "default case, ValueStdWidget");
       w = new ValueStdWidget(this);
       connectValueChanged = false;
    }

    if (connectValueChanged) {
       // featureValueChanged is declared once, in ValueBaseWidget, so one
       // connection per widget suffices.  (A previous design connected the
       // x62 widget twice, once via each class name in its ancestry, so each
       // change was delivered, and setvcp issued, twice.)
       QWidget::connect(w,    &ValueBaseWidget::featureValueChanged,
                        this, &ValueStackedWidget::forContainedWidgetChanged);
    }

    addWidget(w);
    setCurrentWidget(w);
    _cur_stacked_widget = w;

    // apply the control-key state received before the widget existed
    enableCurrentWidget();

    TRACECF_NOPREFIX(debug, "Calling _cur_stacked_widget->setFeatureValue()");
    _cur_stacked_widget->setFeatureValue(fv);
    TRACECF_DONE(debug, "");
}


void ValueStackedWidget::setCurrentValue(uint16_t newval) {
    // NULL only if setFeatureValue() has not yet been called
    if (_cur_stacked_widget)
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
   // a ValueNcplusWidget is a ValueNcWidget, so one test covers both
   bool result = (dynamic_cast<ValueNcWidget*>(_cur_stacked_widget) != nullptr);
   return result;
}
#endif


void ValueStackedWidget::setNcValuesSource(NcValuesSource newValuesSource, bool newUseLatestNcValueNames) {
   bool debug = false;
   debug = debug || debugNcValues;
   TRACECF_STARTING(debug, "newValuesSource=%d, newUseLatestNcValueNames=%s",
                      newValuesSource, SBOOL(newUseLatestNcValueNames));

   // a ValueNcplusWidget is a ValueNcWidget, so one test covers both
   ValueNcWidget * ncw = dynamic_cast<ValueNcWidget*>(_cur_stacked_widget);
   if (ncw)
      ncw->reloadComboBox(newValuesSource, newUseLatestNcValueNames);

   TRACECF_DONE(debug, "");
}


void init_value_stacked_widget() {
   bool debug = false;
   DBGF(debug, "Starting");
   RTTI_ADD_METHOD(ValueStackedWidget::ValueStackedWidget);
   RTTI_ADD_METHOD(ValueStackedWidget::~ValueStackedWidget);
   RTTI_ADD_METHOD(ValueStackedWidget::enableCurrentWidget);
   RTTI_ADD_METHOD(ValueStackedWidget::setInstanceControlKeyRequired);
   RTTI_ADD_METHOD(ValueStackedWidget::setInstanceControlKeyPressed);
   RTTI_ADD_METHOD(ValueStackedWidget::setFeatureValue);
   RTTI_ADD_METHOD(ValueStackedWidget::forContainedWidgetChanged);
   RTTI_ADD_METHOD(ValueStackedWidget::setNcValuesSource);
   DBGF(debug, "Done");
}
