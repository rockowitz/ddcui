/* feature_widget_basic.cpp */

#include "assert.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include "feature_value_widgets/value_cont_widget.h"
#include "feature_value_widgets/value_nc_widget.h"
#include "feature_value_widgets/value_std_widget.h"
#include "feature_value_widgets/value_stacked_widget.h"

#include "feature_widget_basic.h"


FeatureWidgetBasic::FeatureWidgetBasic(QWidget *parent) :
   QFrame(parent)
{
    _cls = metaObject()->className();


    // QFrame methods:
    setFrameStyle(QFrame::Box);    // something to amke it visible

    // layout
    // setObjectName(QString::fromUtf8("FeatureWidgetBasic"));
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
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);



    /* Feature Code */
    int codeLabelStartX = 10;
    int codeLabelLen = 10;
    _featureCodeField = new QLabel();
    _featureCodeField->setObjectName(QString::fromUtf8("featureCode"));
 // _featureCodeField->setGeometry( QRect(codeLabelStartX,0, codeLabelLen,10));
    _featureCodeField->setMinimumWidth(20);
    _featureCodeField->setSizePolicy(fixedSizePolicy);
    _featureCodeField->setFont(monoValueFont);
    _featureCodeField->setText(QString::fromUtf8("0x00"));    // dummy

    /* Feature Name */
    int nameLabelStartX = codeLabelStartX + codeLabelLen + 10;
    _featureNameField = new QLabel();
    _featureNameField->setObjectName(QString::fromUtf8("featureName"));
  //  _featureNameField->setGeometry((QRect(nameLabelStartX,0,59,19)));
    _featureNameField->setMinimumWidth(150);
    _featureNameField->setSizePolicy(fixedSizePolicy);
    _featureNameField->setFont(nonMonoValueFont);
    _featureNameField->setText(QString::fromUtf8("Dummy feature name"));    // dummy

    /* RW/RO/WO */
    _featureRwField = new QLabel();
    _featureRwField->setObjectName(QString::fromUtf8("featureRW"));
   // _featureRwField->setGeometry((QRect(50,0,59,19)));
    _featureRwField->setSizePolicy(fixedSizePolicy);
    _featureRwField->setMinimumWidth(20);
    _featureRwField->setFont(nonMonoValueFont);
    _featureRwField->setText(QString::fromUtf8("RW"));    // dummy

    /* MCCS Type */
    _featureTypeField = new QLabel();
    _featureTypeField->setObjectName(QString::fromUtf8("featureType"));
 //   _featureTypeField->setGeometry((QRect(60,0,69,19)));
    _featureTypeField->setSizePolicy(fixedSizePolicy);
    _featureTypeField->setMinimumWidth(20);
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
    _valueWidget->setSizePolicy(fixedSizePolicy);
    _valueWidget->setMinimumWidth(200);

    _layout = new QHBoxLayout();
    _layout->addWidget(_featureCodeField);
    _layout->addWidget(_featureNameField);
    _layout->addWidget(_featureRwField);
    _layout->addWidget(_featureTypeField);
    _layout->addWidget(_valueWidget);
    // layout->addWidget(_featureValueStackedWidget);
    _layout->addStretch(1);

    setLayout(_layout);
}


void FeatureWidgetBasic::setFeatureValue(FeatureValue &fv) {
    printf("(FeatureWidgetBasic::%s)", __func__); fflush(stdout);
    fv.report();
    _feature_code  = fv._feature_code;
    _feature_flags = fv._feature_flags;
    _vspec         = fv._vspec;

    // need to save value here?
    _mh = fv._mh;
    _ml = fv._ml;
    _sh = fv._sh;
    _sl = fv._sl;




    _featureCodeField->setText(QString::asprintf("x%02x", _feature_code) );

    char * fname = ddca_get_feature_name(_feature_code);
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


void FeatureWidgetBasic::setCurrentValue(uint16_t newval) {
    _valueWidget->setCurrentValue(newval);
}


QSize FeatureWidgetBasic::sizeHint() const {
    int w = 500;
    int h = 20;
    printf("(%s::%s) Returning (%d,%d)\n", _cls, __func__, w, h);  fflush(stdout);
    return QSize(w,h);    // ???
}


#ifdef NO
void FeatureWidgetBasic::paintEvent(QPaintEvent *event) {
     printf("%s::%s)\n", _cls, __func__); fflush(stdout);
     QFrame::paintEvent(event);
     _featureCodeField->update();  // causes separate X window - NO NOT THIS
}
#endif



void FeatureWidgetBasic::dbgrpt() const {
    std::string on1 = objectName().toStdString();
    const char * objname = on1.c_str();
    printf("%-20s code: 0x%02x, flags: 0x%04x, mh: 0x%02x, ml: 0x%02x, sh: 0x%02x, sl 0x%02x\n",
           objname, _feature_code, _feature_flags, _mh, _ml, _sh, _sl);
}
