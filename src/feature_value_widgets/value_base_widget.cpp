/* value_base_widget.cpp */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_base_widget.h"

#include <QtCore/QMargins>

#include "base/ddcui_globals.h"
#include "base/global_state.h"

ValueBaseWidget::ValueBaseWidget(QWidget *parent)
     : QFrame(parent)             // ValueAbstractWidget(parent)
{
   // n. will be reset in subclass constructor
   _cls = strdup(metaObject()->className());
   // TRACE("Create empty widget");

   // setFrameStyle(QFrame::Box);
   // int m_left, m_right, m_top, m_bottom;
   // getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
   // printf("(ValueBaseWidget::ValueBaseWidget) Before reset, margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //        m_left, m_top, m_right, m_bottom);
   // returns 1,1,1,1
   //                 l  t  r   b
   // setContentsMargins(0, 0, 0, 0);
   // getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
   // printf("(ValueBaseWidget::ValueBaseWidget) After reset, margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //        m_left, m_top, m_right, m_bottom);


   //QMargins margins = contentsMargins();
   //printf("(ValueBaseWidget::ValueBaseWidget) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //       margins.m_left, margins.m_top, margins.m_right, margins.m_bottom);

   // try setting frame style at this level,
   setFrameStyle(QFrame::Sunken | QFrame::Panel);

   // dummy values to make Coverity happy, will be set by setFeatureValue()
   _featureCode = 0;
   _dref = NULL;
   _finfo = NULL;
//   _finfo->feature_code = 0;
//   _finfo->feature_flags = 0;
//   _finfo->sl_values = NULL;
//   _finfo->feature_name = NULL;
//   _finfo->feature_desc = NULL;
   _capVcp = NULL;
   _mh = _ml = _sh = _sl = 0;
}


void ValueBaseWidget::setFeatureValue(const FeatureValue &fv) {
   bool debug = false;
   TRACEF(debug, "featureCode=0x%02x, capVcp=%p", fv.featureCode(), fv.capVcp());

    _featureCode    = fv.featureCode();
    _dref           = fv.dref();
    _finfo          = fv.finfo();
    _capVcp         = fv.capVcp();
    _vspec          = fv.vspec();
    _mh             = fv.val().mh;
    _ml             = fv.val().ml;
    _sh             = fv.val().sh;
    _sl             = fv.val().sl;

    // when ValueBaseWidget is initialized
    GlobalState& globalState = GlobalState::instance();
    _controlKeyRequired = globalState._uiOptionsState->controlKeyRequired;

    // listen for changes after initialization
    QObject::connect(globalState._uiOptionsState, &UserInterfaceOptionsState::controlKeyRequired_changed,
                    this, &ValueBaseWidget::when_controlKeyRequired_changed);
}


#ifdef UNNEEDED
// hack to give ValueNcWidget access to parsed capabilities
void ValueBaseWidget::setBaseModel(FeatureBaseModel * model) {
   printf("(%s::%s) feature_code=0x%02x, model=%p\n",
         _cls, __func__, _featureCode, model); fflush(stdout);
   _baseModel = model;
}
#endif


void ValueBaseWidget::setCurrentShSl(uint16_t newval) {
    _sh = newval >> 8;
    _sl = newval & 0xff;
}


uint16_t ValueBaseWidget::getCurrentShSl() {
    uint16_t result = (_sh << 8) | _sl;
    return result;
}

void ValueBaseWidget::setControlKeyRequired(bool onoff) {
   // TRACE("onoff = %s", sbool(onoff) );
   _controlKeyRequired = onoff;
}

bool ValueBaseWidget::getControlKeyRequired() {
   return   _controlKeyRequired ;
}

void ValueBaseWidget::when_controlKeyRequired_changed(bool onoff) {
   // TRACE("Executiong. onoff=%s", sbool(onoff));
   // TRACE("calling setControlKeyRequired()");
   setControlKeyRequired(onoff);
}




#ifdef NO
QSize ValueBaseWidget::sizeHint() const {
    int w = 300;
    int h = 20;
    printf("============> (ValueBaseWidget::sizeHint) returning (%d,%d)\n", w,h ); fflush(stdout);
    return QSize(w,h);
}
#endif
