/** feature_widget_header.cpp
  *
  * Row at top of central widget showing column titles
  */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtWidgets/QFrame>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QHBoxLayout>

#include "base/core.h"
#include "base/widget_debug.h"

#include "feature_widget_header.h"


static bool showDimensionReport = false;
static bool showResizeEvents = false;


static QLabel* stdLabelSetup(QLabel * l) {
   // l->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
   l->setFrameStyle(QFrame::Box | QFrame::Plain);
   l->setLineWidth(0);  // eliminates bar between Code|Name Type|Value - why?

   // l->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
   // l->setHorizontalPolicy(QSizePolicy::Fixed);
   // l->setVerticalPolicy(QSizePolicy::Fixed);

   l->setFont(FeatureValueHeaderFont);
   l->setContentsMargins(0,0,0,0);
   l->setIndent(0);

   return l;
}


void FeatureWidgetHeader::layoutWidget() {
   QLabel * _codeLabel  = stdLabelSetup( new QLabel("Code")  );
   QLabel * _nameLabel  = stdLabelSetup( new QLabel("Name")  );
   QLabel * _rwLabel    = stdLabelSetup( new QLabel("RW")    );
   QLabel * _typeLabel  = stdLabelSetup( new QLabel("Type")  );
   QLabel * _valueLabel = stdLabelSetup( new QLabel("Value") );

   // width
   _codeLabel->setMinimumWidth(30);
   _nameLabel->setFixedWidth(200);   // was 200-2
   _rwLabel->setFixedWidth(25-1);
   _typeLabel->setFixedWidth(25+1+4);
   _valueLabel->setMinimumWidth(200-4+2);

   _nameLabel->setContentsMargins(2,0,0,0);  // so there's space before "Name"
   _valueLabel->setContentsMargins(4,0,0,0); // so there's space before "Value"
   _valueLabel->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );

   QHBoxLayout * layout = new QHBoxLayout();
   layout->addWidget(_codeLabel);
   layout->addWidget(_nameLabel);
   layout->addWidget(_rwLabel);
   layout->addWidget(_typeLabel);
   layout->addWidget(_valueLabel);
   layout->setSpacing(0);

   // From QLayout doc;
   // margins around the layout
   // By default, QLayout uses the values provided by the style.
   // On most platforms, the margin is 11 pixels in all directions.
   layout->setContentsMargins(0,0,0,0);

   setLayout(layout);

   setFrameStyle(QFrame::NoFrame | QFrame::Plain);
   // setContentsMargins(1,1,1,1);  // 0,0,0,0 doesn't move "Code" left
   setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
   setFixedHeight( FeatureHeaderHeight );
   setLineWidth(1);

   if (debugLayout || showDimensionReport) {
      setStyleSheet("background-color:beige;");

      static bool dimensionsReported = false;

      if (showDimensionReport && !dimensionsReported) {
         printf("\nFeatureWidgetHeader dimensions:\n");
         REPORT_BASIC_WIDGET_DIMENSIONS(this);

         reportLabelDimensions(_codeLabel,  _cls, __func__, "_codeLabel dimensions");
         reportLabelDimensions(_nameLabel,  _cls, __func__, "_nameLabel dimensions");
         reportLabelDimensions(_rwLabel,    _cls, __func__, "_rwLabel dimensions");
         reportLabelDimensions(_typeLabel,  _cls, __func__, "_typeLabel dimensions");
         reportLabelDimensions(_valueLabel, _cls, __func__, "_valueLabel dimensions");

         reportFrameDimensions(this, _cls, __func__, "FeatureWidgetHeader dimensions");

         // dimensionsReported = true;
      }
   }
}


FeatureWidgetHeader::FeatureWidgetHeader(QWidget * parent)
: QFrame(parent)
{
   _cls = metaObject()->className();
   layoutWidget();
}


FeatureWidgetHeader::~FeatureWidgetHeader() {
   // TODO Auto-generated destructor stub
}


void FeatureWidgetHeader::resizeEvent(QResizeEvent * evt)
{
   if (showResizeEvents) {
      QSize oldSz = evt->oldSize();
      QSize newSz = evt->size();

      TRACEC("old size = %d, %d   new size = %d, %d", oldSz.width(), oldSz.height(),
            newSz.width(), newSz.height());
   }
   evt->ignore();
}



