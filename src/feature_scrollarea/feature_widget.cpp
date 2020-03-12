// feature_widget.h - Custom widget for displaying/editing a VCP feature

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_scrollarea/feature_widget.h"

#include <assert.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QResizeEvent>

#include "base/core.h"
#include "base/widget_debug.h"
#include "nongui/feature_base_model.h"

#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_stacked_widget.h"


static bool showBasicDims    = false  || debugFeatureDimensions;
static bool showFullDims     = false;
static bool showResizeEvents = false;

// static QFont monoValueFont;


// used for all but the value field
static QLabel * createFeatureWidgetField(
      const char *  objectName,
      const int     fixedWidth,
      const char *  dummyValue)
{
   QLabel * field = new QLabel();
   field->setObjectName(QString::fromUtf8(objectName));
   field->setFixedWidth(fixedWidth);
   // field->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
   field->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
   // field->setFont(monoValueFont);
   field->setFont(FeatureValueTextFont);
   field->setText(QString::fromUtf8(dummyValue));

   field->setFrameStyle( QFrame::Sunken | QFrame::Panel );
   // alt: QFrame::Plain | QFrame::Box;

   field->setContentsMargins(1,1,1,1);  // This is what kills the panel, when set to 0
   field->setLineWidth(1);

   return field;
}


void FeatureWidget::setupFeatureWidget()
{
   bool debug = false;
   TRACECF(debug, "Starting");
   // setFrameStyle(QFrame::Box);    // something to make it visible for development

#ifdef OLD
   QFont font;
   font.setPointSize(8);
   QWidget::setFont(font);
#endif

   // QFont monoValueFont;
   // monoValueFont.setFamily(QString::fromUtf8("Monospace"));

   // QFont nonMonoValueFont;

#ifdef OLD
   QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
   // fixedSizePolicy.setHorizontalStretch(0);    // needed?
   // fixedSizePolicy.setVerticalStretch(0);
   // fixedSizePolicy.setHeightForWidth(false);
   // verify apparent defaults, since I can't find them documented
   assert(fixedSizePolicy.horizontalStretch() == 0);
   assert(fixedSizePolicy.verticalStretch() == 0);
   assert(fixedSizePolicy.hasHeightForWidth() == false);

   QSizePolicy adjustableSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
   adjustableSizePolicy.setHorizontalStretch(1);    // needed?
   adjustableSizePolicy.setVerticalStretch(0);
   adjustableSizePolicy.setHeightForWidth(false);
#endif

   _featureCodeField = createFeatureWidgetField("featureCode",  30, "x00");
   _featureNameField = createFeatureWidgetField("featureName", 200, "dummy feature name");
   // _featureNameField->setFont(nonMonoValueFont);
   _featureNameField->setFont(FeatureValueTextFont);
   _featureRwField   = createFeatureWidgetField("featureRW",    25, "XX");    // RW/RO/WO
   _featureTypeField = createFeatureWidgetField("featureType",  25, "YY");    // MCCS type

#ifdef ALT
   _featureValueStackedWidget = new QStackedWidget(this);
   _featureValueStackedWidget->setObjectName(QString::fromUtf8("_featureValueStackedWidget"));
   _featureValueStackedWidget->setGeometry(QRect(209, 6, 181, 20));

   _featureValueStandardWidget = new QWidget();
   _featureValueStandardWidget->setObjectName(QString::fromUtf8("featureValueStandardWidgdet"));
   _featureValueStackedWidget->addWidget(_featureValueStandardWidget);

   _featureValueStackedWidget->setCurrentIndex(0);
#endif

   TRACECF(debug, "creating ValueStackedWidget, feature code dummy");
   _valueWidget = new ValueStackedWidget();
   _valueWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

   _layout = new QHBoxLayout();
   _layout->addWidget(_featureCodeField);
   _layout->addWidget(_featureNameField);
   _layout->addWidget(_featureRwField);
   _layout->addWidget(_featureTypeField);
  //  _layout->addWidget(_valueWidget);
   // _layout->setStretchFactor(_valueWidget, 10);
   _layout->addWidget(_valueWidget, /*stretch*/ 10, Qt::AlignVCenter);
   // layout->addWidget(_featureValueStackedWidget);

   // eliminating addStretch() eliminates gap between Type and Value fields, but allows
   // feature name field to expand
   // _layout->addStretch(2);

   // _layout->insertStretch(-1, 2);
   _layout->setSpacing(0);   // spacing between widgets inside the (horizontal) layout
   _layout->setContentsMargins(0,0,0,0);

   // eliminating vlayout restores horizontal spacing between all fields.  why?
#ifdef UNNEEDED
   QVBoxLayout * vlayout = new QVBoxLayout();
   vlayout->addLayout(_layout);
   vlayout->setSpacing(0);
   vlayout->setContentsMargins(0,0,0,0);
   setLayout(vlayout);
#endif
   setLayout(_layout);

   setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
   setMaximumHeight(FeatureRowHeight);      // with this alone. rows ware narrow, even
   // setMinimumHeight(FeatureRowHeight);   // with this alone, rows too large, uneven

   static bool dimensionReportShown = false;
   if (debugLayout || showFullDims) {
       this->setStyleSheet("background-color:orange;");

       if (!dimensionReportShown) {
          TRACEMC("FeatureWidget dimensions:");
          reportWidgetDimensions(this, _cls, __func__);
          dimensionReportShown = true;
       }
   }

   static bool basicDimsShown = false;
   if (showBasicDims && !basicDimsShown) {
      REPORT_BASIC_WIDGET_DIMENSIONS(this);
      REPORT_BASIC_WIDGET_DIMENSIONS(_featureCodeField);
      REPORT_BASIC_WIDGET_DIMENSIONS(_valueWidget);
      basicDimsShown = true;
   }

   TRACECF(debug, "Done");
}


void FeatureWidget::setupConnections()
{
   QObject::connect(_valueWidget, &ValueStackedWidget::stackedFeatureValueChanged,
                    this,         &FeatureWidget::onInternalValueChanged);

   // signals/slots not working, try hardcoding
   // _valueWidget->addSimpleFeatureValueObserver(this);
}


FeatureWidget::FeatureWidget(QWidget *parent)
   : QWidget(parent)
   , _cls(metaObject()->className())
{
    setupFeatureWidget();
    setupConnections();
}


FeatureWidget::FeatureWidget(FeatureValue& fv, QWidget *parent)
   : QWidget(parent)
   , _cls(metaObject()->className())
{
    setupFeatureWidget();
    setupConnections();
    setFeatureValue(fv);
}


// Used only to set feature value immediately after constructor called
void FeatureWidget::setFeatureValue(FeatureValue &fv)
{
   bool debug = false;
   TRACECF(debug, "feature code = 0x%02x, ddcrc=%s",
             fv.featureCode(), ddca_rc_name(fv.ddcrc()));

    _feature_code  = fv.featureCode();
    _feature_flags = fv.flags();

    setObjectName(QString::asprintf("FeatureWidget-0x%02x", _feature_code));

    _featureCodeField->setText(QString::asprintf("x%02x", _feature_code) );
    _featureNameField->setText(QString::fromUtf8( fv.finfo()->feature_name));

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

    DDCA_Status ddcrc =fv.ddcrc();

    TRACECF(debug, "feature=0x%02x, getvcp status=%s", _feature_code , ddca_rc_name(ddcrc) );
    // TRACEF(debug, "Before calling valueWidget->setFeatureValue()");
    _valueWidget->setFeatureValue(fv);

    TRACECF(debug, "After calling valueWidget->setFeatureValue()");
    _layout->addWidget(_valueWidget);
}


void FeatureWidget::setCurrentValue(uint16_t newval)
{
    _valueWidget->setCurrentValue(newval);
}

#ifdef UNNEEDED
QSize FeatureWidget::sizeHint() const
{
    int w = 700;
    int h = 10;
    // printf("(%s::%s) Returning (%d,%d)\n", _cls, __func__, w, h);  fflush(stdout);
    return QSize(w,h);    // ???
}
#endif


#ifdef NO
void FeatureWidget::paintEvent(QPaintEvent *event) {
     printf("%s::%s)\n", _cls, __func__); fflush(stdout);
     QFrame::paintEvent(event);
     _featureCodeField->update();  // causes separate X window - NO NOT THIS
}
#endif


void FeatureWidget::dbgrpt() const
{
    std::string on1 = objectName().toStdString();
    const char * objname = on1.c_str();
    // printf("%-20s code: 0x%02x, flags: 0x%04x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x, sl 0x%02x\n",
    //        objname, _feature_code, _feature_flags, _mh, _ml, _sh, _sl);
    TRACEC("%-20s feature code: 0x%02x, flags: 0x%04x", objname, _feature_code, _feature_flags);
}


void FeatureWidget::onInternalValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl)
{
   bool debug = false;
   debug = debug || debugSignals;
   TRACEMCF(debug, "feature_code = 0x%02x, sh=0x%02x, sl=0x%02x", featureCode, sh, sl);
   assert(featureCode == _feature_code);

   bool writeOnlyFeature = _feature_flags & DDCA_WO;
   TRACEMCF(debug, "-> Calling emit valueChanged, feature_code=0x%02x, writeOnlyFeature=%s, sh=0x%02x, sl=0x%02x",
                    featureCode, SBOOL(writeOnlyFeature), sh, sl);
   emit valueChanged(featureCode, writeOnlyFeature, sh, sl);
}


#ifdef UNUSED
// SimpleFeatureValueObserver
void FeatureWidget::simpleFeatureValueChanged(SimpleFeatureValue fv) {
   printf("(%s::%s) feature_code = 0x%02x, sh=0x%02x, sl=0x%02x\n", _cls, __func__,
          fv.featureCode, fv.hiByte, fv.loByte); fflush(stdout);
}
#endif


bool FeatureWidget::hasSlTable()
{
   bool result = _valueWidget->hasSlTable();
   // TRACE("Returning: %s", SBOOL(result));
   return result;
}


void FeatureWidget::setNcValuesSource(NcValuesSource newsrc)
{
   // TRACE("newsrc = %d-%s", newsrc, ncValuesSourceName(newsrc));
   _valueWidget->setNcValuesSource(newsrc);
   // TRACE("Done");
}

void FeatureWidget::resizeEvent(QResizeEvent * evt)
{
   bool show = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool resizeEventsShown = false;
   if (showResizeEvents && !resizeEventsShown) {
      show = true;
      // resizeEventsShown = true;
   }

#ifdef ALT

   int oldWidth = oldSz.width();
   int oldHeight = oldSz.height();
   int newWidth = newSz.width();
   int newHeight = newSz.width();
   if (oldHeight != newHeight || oldWidth != newWidth) {
      show = true;
   }
#endif

   if (show) {
      TRACEC("old size = %d, %d, new size = %d, %d", oldSz.width(), oldSz.height(),
      newSz.width(), newSz.height());
   }

   // evt->ignore();
   evt->accept();
}

