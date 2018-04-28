/* value_nc_widget.cpp */

#include "feature_value_widgets/value_reset_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

#include <ddcutil_c_api.h>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"


static bool dimensionReportShown = false;

ValueResetWidget::ValueResetWidget(QWidget *parent):
        ValueBaseWidget(parent)
{
   _cls = strdup(metaObject()->className());
    // printf("(ValueResetWidget::ValueResetWidget) Starting.\n" ); fflush(stdout);

   QFont nonMonoFont;
   nonMonoFont.setPointSize(9);

   _resetButton = new QPushButton("Restore");
   _resetButton->setMaximumSize(60,20);

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _resetButton->setSizePolicy(*sizePolicy);
    _resetButton->setFont(nonMonoFont);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    _resetButton->setStyleSheet("background-color:white;");

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_resetButton);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (!dimensionReportShown && debugLayout) {
        // printf("-------------------------------------------->\n"); fflush(stdout);
        reportWidgetDimensions(this, _cls, __func__);
        dimensionReportShown = true;
    }
    if (debugLayout)
       this->setStyleSheet("background-color:cyan;");

    // QObject::connect(_resetButton,  &QAbstractButton::released),
    //                  this,          &ValueResetWidget::on_resetButton_pressed );

    QObject::connect(_resetButton,  SIGNAL(released()),
                     this,          SLOT(  on_resetButton_pressed()) );
}


void ValueResetWidget::on_resetButton_pressed() {
   if (debugValueWidgetSignals) {
       printf("(%s::%s)\n", _cls, __func__); fflush(stdout);
   }
   emit featureValueChanged(_featureCode, 0, 1);
}

