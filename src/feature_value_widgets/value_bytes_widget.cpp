// value_bytes_widget.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later


#include "value_bytes_widget.h"


#include <string.h>

#include <QtWidgets/QWidget>
// #include <QtWidgets/QSlider>
// #include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

#include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QVBoxLayout>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"


static bool titleDimensionReportShown = false;
static bool dimensionReportShown = false;










QLabel *
newTitle(QString title) {
   QLabel* lab = new QLabel(title);

   QFont nonMonoValueFont;
   nonMonoValueFont.setPointSize(8);

   QSize   titleSize = QSize(25,18);
   QFont   titleFont = nonMonoValueFont;
   int     titleFrameStyle = QFrame::Plain | QFrame::NoFrame;
   QString titleDebugStyle("background-color:cyan;");

   lab->setFixedSize(titleSize);
   lab->setFont(nonMonoValueFont);
   lab->setFrameStyle(titleFrameStyle);

   if (!titleDimensionReportShown) {
   }

   if (debugLayout)
      lab->setStyleSheet(titleDebugStyle);

   return lab;
}


ValueBytesWidget::ValueBytesWidget(QWidget *parent)
    : ValueBaseWidget(parent)
    , _newval(0)
{
    _cls = strdup(metaObject()->className());
    // PRINTFCM("Starting." );

    nonMonoFont9.setPointSize(8);

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);

    QFont monoValueFont;
    monoValueFont.setPointSize(9);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    _mhTitle = newTitle("mh:");
    _mlTitle = newTitle("ml:");
    _shTitle = newTitle("sh:");
    _slTitle = newTitle("sl:");

    QFont valueFont(monoValueFont);
    int   valueFrameStyle = QFrame::Plain | QFrame::NoFrame;
    QSize valueSize(30,20);

    _mhValue = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    _mlValue = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    _shValue = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);
    _slValue = new NumberEntryWidget( 1, valueFont, valueFrameStyle, valueSize);

// #ifdef APPLY_CANCEL
  //  if (useApplyCancel) {
        QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
       _applyButton  = new QPushButton("Apply");
       _cancelButton = new QPushButton("Cancel");
       _applyButton->setMaximumSize(55,20);
       _applyButton->setSizePolicy(*sizePolicy);
       _applyButton->setFont(nonMonoFont9);
       _cancelButton->setMaximumSize(55,20);
       _cancelButton->setSizePolicy(*sizePolicy);
       _cancelButton->setFont(nonMonoFont9);

       _applyButton->setEnabled(false);
       _cancelButton->setEnabled(false);

       connect(_applyButton,  &QPushButton::clicked,
               this,          &ValueBytesWidget::onApplyButtonClicked);
       connect(_cancelButton, &QPushButton::clicked,
               this,          &ValueBytesWidget::onCancelButtonClicked);
   //
// #endif

   QLabel* spacer = new QLabel();
   spacer->setFixedSize(10,18);


    QHBoxLayout * layout = new QHBoxLayout();
    // layout->addWidget(spacer);
    layout->addWidget(_mhTitle);
    layout->addWidget(_mhValue);
    // not the culprit
  // layout->addWidget(spacer);  adds space here, but also whole valuestackedwidget shifts right

    layout->setStretch(1,0);

    layout->addWidget(_mlTitle);
    layout->addWidget(_mlValue);
    layout->setStretch(1,0);
    layout->addSpacing(5);

    layout->addWidget(_shTitle);
    layout->addWidget(_shValue);
    layout->setStretch(1,0);

    layout->addWidget(_slTitle);
    layout->addWidget(_slValue);
    layout->setStretch(4,0);


       layout->addSpacing(5);
       layout->addWidget(_applyButton);
       layout->addWidget(_cancelButton);

    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (debugLayout) {
       this->setStyleSheet("background-color:yellow;");

       if (!dimensionReportShown) {

           PRINTFCM("_mhTitle dimensions");
           reportWidgetDimensions(_mhTitle, _cls, __func__);

           PRINTFCM("_mhValue dimensions");
           reportWidgetDimensions(_mhValue, _cls, __func__);

           PRINTFCM("_mlTitle dimensions");
           reportWidgetDimensions(_mlTitle, _cls, __func__);

           PRINTFCM("_mlValue dimensions");
           reportWidgetDimensions(_mlValue, _cls, __func__);

           PRINTFCM("_applyButton dimensions");
           reportWidgetDimensions(_applyButton, _cls, __func__);

           PRINTFCM("ValueBytesWidget dimensions");
           reportWidgetDimensions(this, _cls, __func__);
           dimensionReportShown = true;
       }
    }
}

void ValueBytesWidget::setFeatureValue(const FeatureValue &fv) {
    PRINTFCMF(debugValueWidgetSignals, "Starting. feature code: 0x%02x", fv.featureCode());
    ValueBaseWidget::setFeatureValue(fv);

    _guiChange = false;

    // WRONG - NEED TO INSERT AS HEX VALUE
    QString mhHex = QString::number( _mh, 16 );
    _mhValue->setText(mhHex);
    QString mlHex = QString::number( _ml, 16 );
    _mlValue->setText(mlHex);
    QString shHex = QString::number( _mh, 16 );
    _shValue->setText(shHex);
    QString slHex = QString::number( _sl, 16 );
    _slValue->setText(slHex);


    // cf:
    // QString s = QString::number(maxval);
    // _maxValue->setText(s);

// #ifdef APPLY_CANCEL
//     if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
//     }
// #endif

    _guiChange = true;
}


void ValueBytesWidget::setCurrentValue(uint16_t newval) {
    ValueBaseWidget::setCurrentValue(newval);
    _guiChange = false;

     int curval = _sh << 8 | _sl;
     PRINTFCMF(debugValueWidgetSignals, "feature=0x%02x, curval=0%04x", _featureCode , curval);


// #ifdef APPLY_CANCEL
//    if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
//    }
// #endif

    _guiChange = true;
}


uint16_t ValueBytesWidget::getCurrentValue() {
    int curval = _sh << 8 | _sl;
    uint16_t result = (_sh << 8) | _sl;
    return curval;
}

// ifdef APPLY_CANCEL
void  ValueBytesWidget::onApplyButtonClicked(bool checked) {
   PRINTFCMF(debugValueWidgetSignals, "Executing");

   uint16_t oldval = _sh << 8 | _sl;
   if (_newval != oldval) {
      emit featureValueChanged(_featureCode, _newval >> 8, _newval & 0xff);
   }

   // what to do while we're waiting for the update to apply or fail?
   _mhValue->setText( QString::number( _mh, 16 ) );
   // etc

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}


void  ValueBytesWidget::onCancelButtonClicked(bool checked) {
    PRINTFCMF(debugValueWidgetSignals, "Executing");

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);

   _mhValue->setText( QString::number( _mh, 16 ) );
   // etc

}

