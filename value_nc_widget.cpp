/* value_nc_widget.cpp */

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>

#include <ddcutil_c_api.h>
#include "value_nc_widget.h"


ValueNcWidget::ValueNcWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
    printf("============> (ValueNcWidget::ValueNcWidget)\n" ); fflush(stdout);

    _cb = new QComboBox();

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _cb->setSizePolicy(*sizePolicy);
    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_cb);
    setLayout(layout);
}


void ValueNcWidget::setFeatureValue(const FeatureValue &fv) {
    ValueBaseWidget::setFeatureValue(fv);

    printf("============> (ValueNcWidget::setFeatureValue)\n" ); fflush(stdout);

    DDCA_Status rc = 0;
    DDCA_Feature_Value_Table value_table;
    // - get list of values for combo box
    rc = ddca_get_simple_sl_value_table_by_vspec(_feature_code, _vspec, &value_table);
    if (rc != 0) {
        printf("ddca_get_simple_sl_value_table() returned %d\n", rc);
    }
    else {
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





