/* feature_widget_header.cpp
 *
 * <copyright>
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

#include <QtWidgets/QFrame>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtGui/QSizePolicy>
#include <QtWidgets/QHBoxLayout>

#include "base/debug_utils.h"

#include "feature_widget_header.h"


static bool dimensionsReported = false;

static void stdLabelSetup(QLabel * l) {
   // l->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
   l->setFrameStyle(QFrame::Box | QFrame::Plain);
   // setMargin()
   // setindent()
   //set

   QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
   l->setSizePolicy(sizePolicy);
   // l->setHorizontalPolicy(QSizePolicy::Fixed);
   // l->setVerticalPolicy(QSizePolicy::Fixed);

   QFont font;
   font.setPointSize(8);
   l->setFont(font);
   l->setContentsMargins(0,0,0,0);

}



FeatureWidgetHeader::FeatureWidgetHeader(QWidget * parent)
: QFrame(parent)
{
   _cls = metaObject()->className();

   QLabel * _codeLabel  = new QLabel("Code");
   QLabel * _nameLabel  = new QLabel("Name");
   QLabel * _rwLabel    = new QLabel("RW");
   QLabel * _typeLabel  = new QLabel("Type");
   QLabel * _valueLabel = new QLabel("Value");

   stdLabelSetup(_codeLabel);
   stdLabelSetup(_nameLabel);
   stdLabelSetup(_rwLabel);
   stdLabelSetup(_typeLabel);
   stdLabelSetup(_valueLabel);

   _codeLabel->setMinimumWidth(30);
   _nameLabel->setMinimumWidth(200);
   _nameLabel->setFixedWidth(25);
   _typeLabel->setFixedWidth(25);
   _valueLabel->setMinimumWidth(200);

   QSizePolicy adjSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
   _valueLabel->setSizePolicy(adjSizePolicy);


   QHBoxLayout * layout = new QHBoxLayout();
   layout->addWidget(_codeLabel);
   layout->addWidget(_nameLabel);
   layout->addWidget(_rwLabel);
   layout->addWidget(_typeLabel);
   layout->addWidget(_valueLabel);

   setLayout(layout);

   setFrameStyle(QFrame::Box | QFrame::Plain);
   setContentsMargins(1,1,1,1);

   setLineWidth(1);

   if (debugLayout) {
      setStyleSheet("background-color:beige;");

      if (!dimensionsReported) {
         reportLabelDimensions(_codeLabel, _cls, __func__, "_codeLabel dimensions");
         reportLabelDimensions(_nameLabel, _cls, __func__, "_nameLabel dimensions");
         reportLabelDimensions(_rwLabel, _cls, __func__, "_rwLabel dimensions");
         reportLabelDimensions(_typeLabel, _cls, __func__, "_typeLabel dimensions");
         reportLabelDimensions(_valueLabel, _cls, __func__, "_valueLabel dimensions");

         reportFrameDimensions(this, _cls, __func__, "FeatureWidgetHeader dimensions");

         dimensionsReported = true;
      }
   }
}

FeatureWidgetHeader::~FeatureWidgetHeader() {
   // TODO Auto-generated destructor stub
}

