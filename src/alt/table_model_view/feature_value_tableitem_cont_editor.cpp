/* feature_value_tableitem_cont_editor.cpp */

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

#include "alt/table_model_view/feature_value_tableitem_cont_editor.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QLayout>


FeatureValueTableItemContEditor::FeatureValueTableItemContEditor(QWidget *parent) :
        FeatureValueTableItemAbstractEditor(parent)
{
  // std::cout << "(FeatureValueTableItemContEditor::FeatureValueTableItemContEditor)" << std::endl;
  printf("============> (FeatureValueTableItemContEditor::FeatureValueTableItemContEditor)\n" ); fflush(stdout);


  _curSlider = new QSlider(Qt::Horizontal);
  _curSlider->setFocusPolicy(Qt::StrongFocus);
  _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
  _curSlider->setSingleStep(1);

  _curSpinBox = new QSpinBox();
  _curSpinBox->setSingleStep(1);

  _maxTitle = new QLabel("Max:");
  _maxValue = new QLabel();

  connect( _curSlider, SIGNAL(valueChanged(int)),
           _curSpinBox, SLOT(setValue(int)));
  connect(_curSpinBox, SIGNAL(valueChanged(int)),
          _curSlider,   SLOT(setValue(int)));

  QHBoxLayout * layout = new QHBoxLayout();
  layout->addWidget(_curSlider);
  layout->addWidget(_curSpinBox);
  layout->addWidget(_maxTitle);
  layout->addWidget(_maxValue);
  setLayout(layout);
}


void FeatureValueTableItemContEditor::setFeatureValue(const FeatureValue &fv) {
    printf("============> (FeatureValueTableItemContEditor::setFeatureValue)\n" ); fflush(stdout);
    _fv = fv;

    int maxval = _fv.val().mh << 8 | _fv.val().ml;
    int curval = _fv.val().sh << 8 | _fv.val().sl;

    _curSlider->setTickInterval(maxval/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(maxval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(maxval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

    QString s = QString::number(maxval);
    _maxValue->setText(s);

}

FeatureValue FeatureValueTableItemContEditor::featureValue() {
    printf("============> (FeatureValueTableItemContEditor::featureValue)\n" ); fflush(stdout);
    int curval = _curSlider->value();
    _fv.setCurrentValue( curval >> 8 & 0xff, curval & 0xff);
    // _fv._value.sh = curval >> 8;
    // _fv._value.sl = curval & 0xff;
    return _fv;
}


void FeatureValueTableItemContEditor::setCurValue(ushort curval) {
    // _fv._value.sh = curval >> 8;
    // _fv._value.sl = curval & 0xff;
    _fv.setCurrentValue(curval);
    _curSpinBox->setValue(curval);
    _curSlider->setValue(curval);
}


void FeatureValueTableItemContEditor::paintEvent(QPaintEvent *event)  {
    // printf("============> (FeatureValueTableItemContEditor::paintEvent)\n" ); fflush(stdout);
    // QRect rect = event->rect();       // unused
    // QRegion region = event->region(); // unused
}


QSize FeatureValueTableItemContEditor::sizeHint() const {
    printf("============> (FeatureValueTableItemContEditor::sizeHint)\n" ); fflush(stdout);
    return QSize(100,50);
}
