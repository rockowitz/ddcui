/* feature_widget.cpp */

#include "feature_scrollarea/feature_widget.h"

#include <assert.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include "base/debug_utils.h"

#include "nongui/feature_base_model.h"

#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_stacked_widget.h"


static bool dimensionReportShown = false;

static void setupFeatureWidgetField(QLabel * w) {
   // printf("(%s)\n", __func__);  fflush(stdout);
   int fieldFrameStyle;
   fieldFrameStyle = QFrame::Sunken | QFrame::Panel;
   // fieldFrameStyle = QFrame::Plain | QFrame::Box;

   w->setFrameStyle(fieldFrameStyle);
   // w->setFrameStyle( QFrame::Sunken | QFrame::Panel );
   // apparently contents margins is the size of the Panel/box
    w->setContentsMargins(1,1,1,1);  // This is what kills the panel, when set to 0
    w->setLineWidth(1);
    // w->setFrameShape(QFrame::NoFrame);
    // w->setMargin(20,20,20,20);
    w->setMargin(0);

    // w->setStyleSheet("margins: 25px;");
}

void FeatureWidget::setupFeatureWidget() {
   // QFrame methods:
   // setFrameStyle(QFrame::Box);    // something to make it visible

   QFont font;
   font.setPointSize(8);
   QWidget::setFont(font);

   QFont monoValueFont;
   monoValueFont.setFamily(QString::fromUtf8("Monospace"));

   QFont nonMonoValueFont;

   QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::MinimumExpanding);
   // fixedSizePolicy.setHorizontalStretch(0);    // needed?
   // fixedSizePolicy.setVerticalStretch(0);
   // fixedSizePolicy.setHeightForWidth(false);
   // verify apparent defaults, since I can't find them documented
   assert(fixedSizePolicy.horizontalStretch() == 0);
   assert(fixedSizePolicy.verticalStretch() == 0);
   assert(fixedSizePolicy.hasHeightForWidth() == false);

   QSizePolicy adjustableSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
   fixedSizePolicy.setHorizontalStretch(1);    // needed?
   fixedSizePolicy.setVerticalStretch(0);
   fixedSizePolicy.setHeightForWidth(false);

   // int fieldFrameStyle = QFrame::Sunken | QFrame::Panel;

   /* Feature Code */
   _featureCodeField = new QLabel();
   _featureCodeField->setObjectName(QString::fromUtf8("featureCode"));
   _featureCodeField->setMinimumWidth(30);
   _featureCodeField->setSizePolicy(fixedSizePolicy);
   _featureCodeField->setFont(monoValueFont);
   _featureCodeField->setText(QString::fromUtf8("0x00"));    // dummy
   setupFeatureWidgetField(_featureCodeField);

   /* Feature Name */
   _featureNameField = new QLabel();
   _featureNameField->setObjectName(QString::fromUtf8("featureName"));
   // _featureNameField->setMinimumWidth(200);
   // _featureNameField->setMaximumSize(200,20);
   _featureNameField->setFixedWidth(200);
   _featureNameField->setSizePolicy(fixedSizePolicy);
   _featureNameField->setFont(nonMonoValueFont);
   _featureNameField->setText(QString::fromUtf8("Dummy feature name"));    // dummy
   setupFeatureWidgetField(_featureNameField);

   /* RW/RO/WO */
   _featureRwField = new QLabel();
   _featureRwField->setObjectName(QString::fromUtf8("featureRW"));
   _featureRwField->setSizePolicy(fixedSizePolicy);
   _featureRwField->setFixedWidth(25);
   _featureRwField->setFont(monoValueFont);
   _featureRwField->setText(QString::fromUtf8("XX"));    // dummy
  //  _featureCodeField->setFrameStyle(fieldFrameStyle);
   // _featureRwField->setFrameStyle(QFrame::Sunken | QFrame::Panel);
   setupFeatureWidgetField(_featureRwField);

   /* MCCS Type */
   _featureTypeField = new QLabel();
   _featureTypeField->setObjectName(QString::fromUtf8("featureType"));
   _featureTypeField->setSizePolicy(fixedSizePolicy);
   _featureTypeField->setFixedWidth(25);
   _featureTypeField->setFont(monoValueFont);
   _featureTypeField->setText(QString::fromUtf8("YY"));    // dummy
  //  _featureCodeField->setFrameStyle(fieldFrameStyle);
   // _featureTypeField->setFrameStyle(QFrame::Sunken | QFrame::Panel);
   setupFeatureWidgetField(_featureTypeField);

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
   // _valueWidget->setSizePolicy(fixedSizePolicy);
   // _valueWidget->setFixedWidth(400);

   _layout = new QHBoxLayout();
   _layout->addWidget(_featureCodeField);
   _layout->addWidget(_featureNameField);
   _layout->addWidget(_featureRwField);
   _layout->addWidget(_featureTypeField);
   _layout->addWidget(_valueWidget);
   // layout->addWidget(_featureValueStackedWidget);

   // eliminating addStretch() eliminates gap between Type and Value fields, but allows
   // feature name field to expand
   // _layout->addStretch(2);

   // _layout->insertStretch(-1, 2);
   _layout->setSpacing(0);

   // _layout->setMargin(0);    // no effect
   _layout->setContentsMargins(0,0,0,0);
   // removes space between all fields except field type <-> value
   // _layout->setSpacing(0);  // adjusts space between each horizontal item, no effect on above/below
   // _layout->setSpacing(50);

   // eliminating vlayout restores horizontal spacing between all fields.  why?
#ifdef UNNEEDED
   QVBoxLayout * vlayout = new QVBoxLayout();
   vlayout->addLayout(_layout);
   vlayout->setSpacing(0);
   vlayout->setContentsMargins(0,0,0,0);
   setLayout(vlayout);
#endif
   setLayout(_layout);

   if (debugLayout) {
       this->setStyleSheet("background-color:orange;");

       if (!dimensionReportShown) {
          PRINTFCM("FeatureWidget dimenstions:");
          reportWidgetDimensions(this, _cls, __func__);
          dimensionReportShown = true;
       }
   }

   QObject::connect(_valueWidget, &ValueStackedWidget::stackedFeatureValueChanged,
                    this,         &FeatureWidget::onInternalValueChanged);

#ifdef ALT
   QObject::connect( _valueWidget, SIGNAL( stackedFeatureValueChanged(uint8_t, uint8_t, uint8_t)),
                     this,         SLOT(onInternalValueChanged(uint8_t, uint8_t, uint8_t)));
#endif
   // signals/slots not working, try hardcoding
   // _valueWidget->addSimpleFeatureValueObserver(this);
}


FeatureWidget::FeatureWidget(QWidget *parent) :
   QWidget(parent),
   _cls(metaObject()->className())
{
    // _cls = metaObject()->className();
    setupFeatureWidget();
}

FeatureWidget::FeatureWidget(FeatureValue& fv, QWidget *parent) :
   QWidget(parent),
   _cls(metaObject()->className())
{
    // _cls = metaObject()->className();
    setupFeatureWidget();
    setFeatureValue(fv);
}


// Used only to set feature value immediately after constructor called
void FeatureWidget::setFeatureValue(FeatureValue &fv) {
    // printf("(FeatureWidgetBasic::%s)", __func__);
    // fv.report(); ff;isj)stdpit);
    // printf("(%s::%s) _feature_flags = 0x%08x, fv._feature_flags = 0x%08x, fv._finfo

    _feature_code  = fv._feature_code;
    _feature_flags = fv.flags();

    setObjectName(QString::asprintf("FeatureWidget-0x%02x", _feature_code));

    _featureCodeField->setText(QString::asprintf("x%02x", _feature_code) );
    _featureNameField->setText(QString::fromUtf8( fv.finfo().feature_name));

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

    _valueWidget->setFeatureValue(fv);
    _layout->addWidget(_valueWidget);
}


void FeatureWidget::setCurrentValue(uint16_t newval) {
    _valueWidget->setCurrentValue(newval);
}


QSize FeatureWidget::sizeHint() const {
    int w = 700;
    int h = 10;
    // printf("(%s::%s) Returning (%d,%d)\n", _cls, __func__, w, h);  fflush(stdout);
    return QSize(w,h);    // ???
}


#ifdef NO
void FeatureWidget::paintEvent(QPaintEvent *event) {
     printf("%s::%s)\n", _cls, __func__); fflush(stdout);
     QFrame::paintEvent(event);
     _featureCodeField->update();  // causes separate X window - NO NOT THIS
}
#endif


void FeatureWidget::dbgrpt() const {
    std::string on1 = objectName().toStdString();
    const char * objname = on1.c_str();
    // printf("%-20s code: 0x%02x, flags: 0x%04x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x, sl 0x%02x\n",
    //        objname, _feature_code, _feature_flags, _mh, _ml, _sh, _sl);
    PRINTFCM("%-20s feature code: 0x%02x, flags: 0x%04x", objname, _feature_code, _feature_flags);
}


void FeatureWidget::onInternalValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl) {
   PRINTFCMF(debugSignals, "feature_code = 0x%02x, sh=0x%02x, sl=0x%02x\n", featureCode, sh, sl);
   assert(featureCode == _feature_code);

   bool writeOnlyFeature = _feature_flags & DDCA_WO;

   emit valueChanged(featureCode, writeOnlyFeature, sh, sl);
}


#ifdef UNUSED
// SimpleFeatureValueObserver
void FeatureWidget::simpleFeatureValueChanged(SimpleFeatureValue fv) {
   printf("(%s::%s) feature_code = 0x%02x, sh=0x%02x, sl=0x%02x\n", _cls, __func__,
          fv.featureCode, fv.hiByte, fv.loByte); fflush(stdout);
}
#endif


bool FeatureWidget::isSimpleNc() {
   bool result = _valueWidget->isSimpleNc();
   // PRINTFCM("Returning: %s \n", sbool(result));
   return result;
}

void FeatureWidget::setNcValuesSource(NcValuesSource newsrc) {
   PRINTFCM("newsrc = %d-%s \n", newsrc, ncValuesSourceName(newsrc));
   _valueWidget->setNcValuesSource(newsrc);
}
