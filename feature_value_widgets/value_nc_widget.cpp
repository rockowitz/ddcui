/* value_nc_widget.cpp */

#include "feature_value_widgets/value_nc_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>

#include "base/ddcui_globals.h"

#include <ddcutil_c_api.h>



ValueNcWidget::ValueNcWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
   _cls                    = strdup(metaObject()->className());
    // printf("(ValueNcWidget::ValueNcWidget) Starting.\n" ); fflush(stdout);

   QFont nonMonoFont;
   nonMonoFont.setPointSize(9);

    _cb = new QComboBox();

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _cb->setSizePolicy(*sizePolicy);
    _cb->setFont(nonMonoFont);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    _cb->setStyleSheet("background-color:white;");

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_cb);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    int m_left, m_right, m_top, m_bottom;
    getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
    // printf("(ValueNcWidget::ValueNcWidget) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
    //        m_left, m_right, m_top, m_bottom);
    if (debugLayout)
       this->setStyleSheet("background-color:cyan;");

    QObject::connect(_cb,  SIGNAL(activated(int)),
                     this, SLOT(combobox_activated(int)) );

}


void ValueNcWidget::setFeatureValue(const FeatureValue &fv) {
    ValueBaseWidget::setFeatureValue(fv);

    // printf("(ValueNcWidget::setFeatureValue) Starting\n" ); fflush(stdout);

    DDCA_Status rc = 0;
    DDCA_Feature_Value_Table value_table;
    // - get list of values for combo box
    rc = ddca_get_simple_sl_value_table_by_vspec(_feature_code, _vspec, &value_table);
    if (rc != 0) {
        printf("ddca_get_simple_sl_value_table() returned %d\n", rc);
    }
    else {
        // - set values in combo box
        // printf("(%s) Setting combo box values. value_table=%p\n", __func__, value_table);
        DDCA_Feature_Value_Entry * cur = value_table;

        if (cur) {
            while (cur->value_name) {
                // printf("(%s) value code: 0x%02x, value_name: %s\n",
                //        __func__, cur->value_code, cur->value_name);  fflush(stdout);
                QString s;
                s.sprintf("%s (0x%02x)", cur->value_name, cur->value_code);
                _cb->addItem(s, QVariant(cur->value_code));
                cur++;
            }
        }

        // - set current value in combo box
        int cur_ndx = findItem(_sl);
        if (cur_ndx >= 0) {
            _cb->setCurrentIndex(cur_ndx);
        }
        else {
            printf("(FeatureValueTableItemCbEditor::%s) Unable to find value 0x%02x\n", __func__, _sl);
        }
    }
}


void ValueNcWidget::setCurrentValue(uint16_t newval) {
    ValueBaseWidget::setCurrentValue(newval);

    // - set current value in combo box
    int cur_ndx = findItem(_sl);
    if (cur_ndx >= 0) {
        _cb->setCurrentIndex(cur_ndx);
    }
    else {
        printf("(FeatureValueTableItemCbEditor::%s) Unable to find value 0x%02x\n", __func__, _sl);
    }
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
   printf("(%s::%s) index=%d\n", _cls, __func__, index); fflush(stdout);
   int ndx = _cb->currentIndex();
   assert(ndx == index);

   QVariant qv = _cb->itemData(ndx);
   uint i = qv.toUInt();
   uint8_t new_sh = 0;
   uint8_t new_sl = i & 0xff;

   if (new_sh != _sh || new_sl != _sl) {
      printf("(%s::%s) Value changed.  New sl: %u\n", _cls, __func__, new_sl); fflush(stdout);
      emit featureValueChanged(_feature_code, 0, new_sl);
      _sh = 0;
      _sl = new_sl;
   }
   else {
      printf("(%s::%s) Value not changed.\n", _cls, __func__); fflush(stdout);
   }

}


