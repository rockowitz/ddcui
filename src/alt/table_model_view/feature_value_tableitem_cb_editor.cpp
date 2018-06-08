/* feature_value_tableitem_cb_editor.cpp */

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

#include "alt/table_model_view/feature_value_tableitem_cb_editor.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>

#include <ddcutil_c_api.h>



FeatureValueTableItemCbEditor::FeatureValueTableItemCbEditor(QWidget *parent) :
    FeatureValueTableItemAbstractEditor(parent)
{
  // printf("============> (FeatureValueTableItemCbEditor::FeatureValueTableItemCbEditor)\n" ); fflush(stdout);

  _cb = new QComboBox();

  QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  // _cb->setHorizontalStretch(0);
  _cb->setSizePolicy(*sizePolicy);
  QHBoxLayout * layout = new QHBoxLayout();
  layout->addWidget(_cb);
  setLayout(layout);
  delete sizePolicy;   // safe?
}


int FeatureValueTableItemCbEditor::findItem(uint8_t sl_value) {
    int result = _cb->findData(QVariant(sl_value));
    return result;
}


void FeatureValueTableItemCbEditor::setFeatureValue(const FeatureValue &fv) {
    printf("============> (FeatureValueTableItemCbEditor::setFeatureValue)\n" ); fflush(stdout);
    _fv = fv;

    // DDCA_Version_Feature_Info info;
    // DDCA_Status rc = 0;
    // rc = ddca_get_feature_info_by_vcp_version(_fv._feature_code, mccs_id, &info);
    DDCA_Feature_Value_Table value_table;

    value_table = fv.finfo().sl_values;

     // - set values in combo box
     printf("(%s) Setting combo box values. value_table=%p\n", __func__, value_table);
     DDCA_Feature_Value_Entry * cur = value_table;

     if (cur) {
         while (cur->value_name) {
             // printf("(%s) value code: 0x%02x, value_name: %s\n",
             //        __func__, cur->value_code, cur->value_name);  fflush(stdout);
             QString s;
             s.sprintf("%s (0x%02x)", cur->value_name, cur->value_code);
             _cb->addItem(s, QVariant(cur->value_code));
             cur++;     // ??
         }
     }

     // - set current value in combo box
     int cur_ndx = findItem(_fv.val().sl);
     if (cur_ndx >= 0) {
         _cb->setCurrentIndex(cur_ndx);
     }
     else {
         printf("(FeatureValueTableItemCbEditor::%s) Unable to find value 0x%02x\n", __func__, _fv.val().sl);
     }
}


FeatureValue FeatureValueTableItemCbEditor::featureValue() {
    printf("============> (FeatureValueTableItemCbEditor::featureValue)\n" ); fflush(stdout);
    // get sl from combobox
    int ndx = _cb->currentIndex();
    QVariant qv = _cb->itemData(ndx);
    uint i = qv.toUInt();

     // _fv._value.sh = 0;
     // _fv._value.sl = i & 0xff;
     _fv.setCurrentValue(0, i&0xff);
    return _fv;
}


void FeatureValueTableItemCbEditor::setCurValue(ushort curval) {
    //  _fv._value.sh = curval >> 8;
    // _fv._value.sl = curval & 0xff;
    _fv.setCurrentValue(curval);
    // _curSpinBox->setValue(curval);
    // _curSlider->setValue(curval);
}


void FeatureValueTableItemCbEditor::paintEvent(QPaintEvent *event)  {
    // printf("============> (FeatureValueTableItemCbEditor::paintEvent)\n" ); fflush(stdout);
    // QRect rect = event->rect();
    // QRegion region = event->region();
}


// why needed, not finding function in parent
QSize FeatureValueTableItemCbEditor::sizeHint() const {
    printf("============> (FeatureValueTableItemCbEditor::sizeHint)\n" ); fflush(stdout);
    return QSize(100,50);
}
