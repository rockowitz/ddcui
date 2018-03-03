/* vcplineitem.h */

//#include <assert.h>
#include <iostream>

#include "ddcutil_c_api.h"

#include <QtWidgets/QHBoxLayout>

#include "vcplineitem.h"

using namespace std;


void VcpLineItem::initInstance(void)
{
        if (this->objectName().isEmpty())
            this->setObjectName(QString::fromUtf8("vcpLineItemInstance"));

        this->resize(400, 20);

        QFont font;
        font.setPointSize(8);
        // thisLineItem->setFont(font);

        QFont font1;
        font1.setFamily(QString::fromUtf8("Monospace"));
        // featureCode->setFont(font1);

 
        QFont fixedFont;
        fixedFont.setPointSize(8);
        fixedFont.setFamily(QString::fromUtf8("Monospace"));



        QSizePolicy fixedSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        fixedSizePolicy.setHorizontalStretch(0);
        fixedSizePolicy.setVerticalStretch(0);
        // fixedSizePolicy.setHeightForWidth(featureCode->sizePolicy().hasHeightForWidth());



        _featureCodeLabel = new QLabel(this);
        _featureCodeLabel->setObjectName(QString::fromUtf8("featureCode"));
        _featureCodeLabel->setGeometry(QRect(10, 0, 10, 18));
        _featureCodeLabel->setSizePolicy(fixedSizePolicy);
        _featureCodeLabel->setFont(fixedFont);

        _featureNameLabel = new QLabel(this);
        _featureNameLabel->setObjectName(QString::fromUtf8("featureName"));
        _featureNameLabel->setGeometry(QRect(40, 0, 59, 18));
        _featureNameLabel->setSizePolicy(fixedSizePolicy);
        _featureNameLabel->setFont(fixedFont);

        _featureRWLabel = new QLabel(this);
        _featureRWLabel->setObjectName(QString::fromUtf8("featureRW"));
        _featureRWLabel->setGeometry(QRect(110, 0, 10, 18));
        _featureRWLabel->setSizePolicy(fixedSizePolicy);
        _featureRWLabel->setFont(fixedFont);

        _featureTypeLabel = new QLabel(this);
        _featureTypeLabel->setObjectName(QString::fromUtf8("featureType"));
        _featureTypeLabel->setGeometry(QRect(140, 0, 8, 18));
        _featureTypeLabel->setSizePolicy(fixedSizePolicy);
        _featureTypeLabel->setFont(fixedFont);


        _featureDataStackedWidget = new QStackedWidget(this);
        _featureDataStackedWidget->setObjectName(QString::fromUtf8("featureDataStackedWidget"));
        _featureDataStackedWidget->setGeometry(QRect(209, 6, 181, 20));

        _featureDataTextLabel = new QLabel();
        _featureDataTextLabel->setObjectName(QString::fromUtf8("featureDataText"));
        _featureDataStackedWidget->addWidget(_featureDataTextLabel);


        featureDataCDefault = new QWidget();
        featureDataCDefault->setObjectName(QString::fromUtf8("featureDataCDefault"));
        _featureDataStackedWidget->addWidget(featureDataCDefault);
        featureDataCDefault_2 = new QWidget();
        featureDataCDefault_2->setObjectName(QString::fromUtf8("featureDataCDefault_2"));
        _featureDataStackedWidget->addWidget(featureDataCDefault_2);

        // retranslateUi(thisLineItem);

        _featureDataStackedWidget->setCurrentIndex(0);

        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(_featureCodeLabel);
        layout->addWidget(_featureNameLabel);
        layout->addWidget(_featureRWLabel);
        layout->addWidget(_featureTypeLabel);
        layout->addWidget(_featureDataStackedWidget);
        this->setLayout(layout);


        QMetaObject::connectSlotsByName(this);
    } // setupUi




VcpLineItem::VcpLineItem(QListWidget *parent) :
    QListWidgetItem(parent, UserType+1)
  // QDialog(parent)
  //  QWidget(parent)

   
{
    initInstance();
    this->show();    // temp location
}

#ifdef NO
void VcpLineItem::_deleteUI(void) {
  cout << "bye\n";
}
#endif


VcpLineItem::~VcpLineItem(void) {
 //   _deleteUI();
//     // delete ui;
}




void VcpLineItem::setFormattedValue(char * formattedValue) {
   _featureDataTextLabel->setText(QString::fromUtf8(formattedValue));
}




void VcpLineItem::setFeatureCode(uint8_t feature_code) {
   this->_feature_code = feature_code;
   char buf[8];
   snprintf(buf, 8, "%02x", this->_feature_code);
   this->_featureCodeLabel->setText(buf);
}

// void VcpLineItem::setFeatureName(char * name) {
//    this->_featureNameLabel->setText(QString::fromUtf8(name));
//}

void VcpLineItem::setFeatureName(QString name)  {
    this->_featureNameLabel->setText(name);
}

void VcpLineItem::setFeatureRW(QString rw) {
   _featureRWLabel->setText(rw);
}

void VcpLineItem::setFeatureMccsType(QString featureType) {
   _featureTypeLabel->setText(featureType);
}


void VcpLineItem::setFeatureInfo(DDCA_Version_Feature_Info * finfo) {
   _feature_info = finfo;
   this->setFeatureCode(finfo->feature_code);
   this->setFeatureName(finfo->feature_name);
   QString rw = "";
   if (finfo->feature_flags & DDCA_RW)
      rw = "RW";
   else if (finfo->feature_flags & DDCA_RO)
      rw = "RO";
   else {
      // assert(finfo->feature_flags & DDCA_WO);
      rw = "WO";
   }
   
   this->setFeatureRW(rw);

   QString mccsFeatureType = "";
   if (finfo->feature_flags & DDCA_CONT) 
      mccsFeatureType = "C";
   else if (finfo->feature_flags  & DDCA_NC)
      mccsFeatureType = "NC";
   else {
      // assert (finfo->feature_flags & DDCA_TABLE);
      mccsFeatureType = "T";
   }

   this->setFeatureMccsType(mccsFeatureType); 
}



#ifdef NO
    void retranslateUi(QWidget *thisLineItem)
    {
        thisLineItem->setWindowTitle(QApplication::translate("thisLineItem", "Form", 0, QApplication::UnicodeUTF8));
        featureCode->setText(QApplication::translate("thisLineItem", "TextLabel", 0, QApplication::UnicodeUTF8));
        featureName->setText(QApplication::translate("thisLineItem", "TextLabel", 0, QApplication::UnicodeUTF8));
        featureRW->setText(QApplication::translate("thisLineItem", "TextLabel", 0, QApplication::UnicodeUTF8));
        featureType->setText(QApplication::translate("thisLineItem", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi
#endif

