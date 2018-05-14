/* feature_widget_extended.cpp */

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

#include "alt/list_widget/feature_widget_extended.h"

#include "assert.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>

#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_stacked_widget.h"


static bool debug = true;

FeatureWidgetExtended::FeatureWidgetExtended(QListWidget *parent) :
 //   QWidget(parent)
  //   QWidgetItem(nullptr)
      QListWidgetItem(parent, FeatureWidgetType)
{
    _cls = metaObject()->className();
    setText("Dummy Item Text");

    // layout
    // setObjectName(QString::fromUtf8("FeatureWidget"));
    // resize(400,20);

    QFont font;
    font.setPointSize(9);
    QWidget::setFont(font);

    QFont monoValueFont;
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    QFont nonMonoValueFont;

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);

    /* Feature Code */
    int codeLabelStartX = 10;
    int codeLabelLen = 10;
    _featureCodeField = new QLabel();
    _featureCodeField->setObjectName(QString::fromUtf8("featureCode"));
    _featureCodeField->setGeometry( QRect(codeLabelStartX,0, codeLabelLen,10));
    _featureCodeField->setSizePolicy(fixedSizePolicy);
    _featureCodeField->setFont(monoValueFont);
    _featureCodeField->setText(QString::fromUtf8("0x00"));    // dummy

    /* Feature Name */
    int nameLabelStartX = codeLabelStartX + codeLabelLen + 10;
    _featureNameField = new QLabel();
    _featureNameField->setObjectName(QString::fromUtf8("featureName"));
    _featureNameField->setGeometry((QRect(nameLabelStartX,0,59,19)));
    _featureNameField->setSizePolicy(fixedSizePolicy);
    _featureNameField->setFont(nonMonoValueFont);
    _featureNameField->setText(QString::fromUtf8("Dummy feature name"));    // dummy

    /* RW/RO/WO */
    _featureRwField = new QLabel();
    _featureRwField->setObjectName(QString::fromUtf8("featureRW"));
    _featureRwField->setGeometry((QRect(50,0,59,19)));
    _featureRwField->setSizePolicy(fixedSizePolicy);
    _featureRwField->setFont(nonMonoValueFont);
    _featureRwField->setText(QString::fromUtf8("RW"));    // dummy

    /* MCCS Type */
    _featureTypeField = new QLabel();
    _featureTypeField->setObjectName(QString::fromUtf8("featureType"));
    _featureTypeField->setGeometry((QRect(60,0,69,19)));
    _featureTypeField->setSizePolicy(fixedSizePolicy);
    _featureTypeField->setFont(nonMonoValueFont);
    _featureTypeField->setText(QString::fromUtf8("NC"));    // dummy

#ifdef ALT
    _featureValueStackedWidget = new QStackedWidget(this);
    _featureValueStackedWidget->setObjectName(QString::fromUtf8("_featureValueStackedWidget"));
    _featureValueStackedWidget->setGeometry(QRect(209, 6, 181, 20));

    _featureValueStandardWidget = new QWidget();
    _featureValueStandardWidget->setObjectName(QString::fromUtf8("featureValueStandardWidgdet"));
    _featureValueStackedWidget->addWidget(_featureValueStandardWidget);

    _featureValueStackedWidget->setCurrentIndex(0);
#endif

    _valueWidget = new ValueStackedWidget();

    _layout = new QHBoxLayout();
    _layout->addWidget(_featureCodeField);
    _layout->addWidget(_featureNameField);
    _layout->addWidget(_featureRwField);
    _layout->addWidget(_featureTypeField);
    _layout->addWidget(_valueWidget);
    // layout->addWidget(_featureValueStackedWidget);

    setLayout(_layout);
}


void FeatureWidgetExtended::setFeatureValue(FeatureValue &fv) {
    if (debug) {
        printf("(FeatureWidget::%s)", __func__); fflush(stdout);
        fv.dbgrpt();
    }

    _feature_code  = fv._feature_code;
    _feature_flags = fv.flags();     // fv._feature_flags;
    // _vspec         = fv.vspec();     // fv._vspec;

    _featureCodeField->setText(QString::asprintf("x%02x", _feature_code) );

    // char * fname = ddca_get_feature_name(_feature_code);
    char * fname = fv.finfo().feature_name;
    _featureNameField->setText(QString::fromUtf8(fname));

    QString s_rw;
    if (_feature_flags & DDCA_RW)
        s_rw = QString("RW");
    else if (_feature_flags & DDCA_RO)
        s_rw = QString("RO");
    else {
        assert(_feature_flags & DDCA_WO);
        s_rw = QString("WO");
    }
    _featureRwField->setText(s_rw);

    if (_feature_flags & DDCA_CONT)
        _featureTypeField->setText(QString("C"));
    else if (_feature_flags & DDCA_NC)
        _featureTypeField->setText(QString("NC"));
    else {
        assert(_feature_flags & DDCA_TABLE);
        _featureTypeField->setText(QString("T"));
    }

#ifdef OLD
    if (_feature_flags & DDCA_STD_CONT) {
        _featureTypeField->setText(QString("C"));
        _valueWidget = new ValueContWidget();
    }
    else if (_feature_flags & DDCA_SIMPLE_NC) {
        _valueWidget = new ValueNcWidget();
    }
    else {
        _valueWidget = new ValueStdWidget();
    }
#endif

    _valueWidget->setFeatureValue(fv);
    _layout->addWidget(_valueWidget);
}


void FeatureWidgetExtended::setCurrentValue(uint16_t newval) {
    _valueWidget->setCurrentValue(newval);
}


#ifdef NO
void FeatureWidgetExtended::paintEvent(QPaintEvent *event) {
     printf("%s::%s)\n", _cls, __func__); fflush(stdout);
     // QWidget::paintEvent(event);
     _featureCodeField->update();  // causes separate X window - NO NOT THIS
}
#endif


void FeatureWidgetExtended::dbgrpt() const {
    std::string on1 = objectName().toStdString();
    const char * objname = on1.c_str();
    printf("%-20s code: 0x%02x, flags: 0x%04x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x, sl 0x%02x\n",
           objname, _feature_code, _feature_flags, _mh, _ml, _sh, _sl);
}
