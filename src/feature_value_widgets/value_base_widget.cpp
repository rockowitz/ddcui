/* value_base_widget.cpp */

/* <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#include "feature_value_widgets/value_base_widget.h"
#include "feature_value_widgets/value_abstract_widget.h"

#include <QtCore/QMargins>

ValueBaseWidget::ValueBaseWidget(QWidget *parent)
     : ValueAbstractWidget(parent)
{
   // n. will be reset in subclass constructor
   _cls = strdup(metaObject()->className());

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
}


void ValueBaseWidget::setFeatureValue(const FeatureValue &fv) {
    // printf("(ValueBaseWidget::%s) featureCode=0x%02x\n", __func__, fv.featureCode()); fflush(stdout);

    _featureCode    = fv.featureCode();
    _dref           = fv.dref();
    _finfo          = fv.finfo();
    _capVcp         = fv._cap_vcp;
    _mh             = fv._value.mh;
    _ml             = fv._value.ml;
    _sh             = fv._value.sh;
    _sl             = fv._value.sl;
}


#ifdef UNNEEDED
// hack to give ValueNcWidget access to parsed capabilities
void ValueBaseWidget::setBaseModel(FeatureBaseModel * model) {
   printf("(%s::%s) feature_code=0x%02x, model=%p\n",
         _cls, __func__, _featureCode, model); fflush(stdout);
   _baseModel = model;
}
#endif


void ValueBaseWidget::setCurrentValue(uint16_t newval) {
    _sh = newval >> 8;
    _sl = newval & 0xff;
}


uint16_t ValueBaseWidget::getCurrentValue() {
    uint16_t result = (_sh << 8) | _sl;
    return result;
}

#ifdef NO
QSize ValueBaseWidget::sizeHint() const {
    int w = 300;
    int h = 20;
    printf("============> (ValueBaseWidget::sizeHint) returning (%d,%d)\n", w,h ); fflush(stdout);
    return QSize(w,h);
}
#endif
