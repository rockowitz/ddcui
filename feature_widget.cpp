/* feature_widget.cpp */

#include "assert.h"

#include <QtWidgets>
// #include <QWidget>
// #include <QVBoxLayout>
// #include <QHBoxLayout>


#include "feature_widget.h"
#include "value_cont_widget.h"
#include "value_nc_widget.h"
#include "value_std_widget.h"


FeatureWidget::FeatureWidget(QListWidget *parent) :
 //   QWidget(parent)
  //   QWidgetItem(nullptr)
      QListWidgetItem(parent, UserType+1)
{
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


    _layout = new QHBoxLayout();
    _layout->addWidget(_featureCodeField);
    _layout->addWidget(_featureNameField);
    _layout->addWidget(_featureRwField);
    _layout->addWidget(_featureTypeField);
    // layout->addWidget(_featureValueStackedWidget);
}


void FeatureWidget::setFeatureValue(FeatureValue &fv) {
    _feature_code  = fv._feature_code;
    _feature_flags = fv._feature_flags;
    _vspec         = fv._vspec;

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

    _valueWidget->setFeatureValue(fv);
    _layout->addWidget(_valueWidget);

}

void FeatureWidget::setCurrentValue(uint16_t newval) {
    _valueWidget->setCurrentValue(newval);
}
