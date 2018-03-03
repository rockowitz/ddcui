/* vcplineitem.h */

#ifndef VCPLINEITEM_H
#define VCPLINEITEM_H

#include <QtWidgets/QDialog>
#include <QtCore/QVariant>

#include <QtWidgets/QWidget>
// #include <QtWidgets/QApplication>
// #include <QtWidgets/QButtonGroup>
// #include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QStackedWidget>

// #include <QtGui/QAction>   // Qt4

#include "ddcutil_c_api.h"

// QT_BEGIN_NAMESPACE

class VcpLineItem : public QWidget, public QListWidgetItem

     //   , public QDialog
{
    Q_OBJECT

public:
    uint8_t _feature_code;
    QLabel *_featureCodeLabel;
    QLabel *_featureNameLabel;
    QLabel *_featureRWLabel;
    QLabel *_featureTypeLabel;

    QStackedWidget *_featureDataStackedWidget;
    QLabel  *       _featureDataTextLabel;
    QWidget *        featureDataCDefault;
    QWidget *        featureDataCDefault_2;

    DDCA_Version_Feature_Info * _feature_info;

    VcpLineItem(QListWidget* parent = 0);
    ~VcpLineItem(void);

    void setFeatureCode(uint8_t feature_code);
    void setFeatureName(QString name);
    void setFeatureRW(QString rw);
    void setFeatureMccsType(QString featureType);
    void setFeatureInfo(DDCA_Version_Feature_Info * finfo);
    void setFormattedValue(char * formattedValue);

private: 
      void initInstance(void);
      // void _deleteUI(void);
};

// QT_END_NAMESPACE

#endif // VCPLINEITEM_H
