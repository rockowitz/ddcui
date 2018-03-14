#ifndef FEATURE_WIDGET_H
#define FEATURE_WIDGET_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QLabel>
#include <QStackedWidget>
#include <QtWidgets>

#include "feature_value.h"
#include "value_abstract_widget.h"

class FeatureWidget :
        public QWidget,
        public QListWidgetItem     // apparently does not inherit from QWidget
{
    Q_OBJECT
public:
    explicit FeatureWidget(QListWidget *parent = nullptr);

    FeatureWidget(FeatureValue *fv, QWidget *parent=nullptr);

    void setFeatureValue(FeatureValue &fv);

    void setCurrentValue(uint16_t newval);

    uint8_t   _feature_code;
    QString*  _feature_name;
    uint8_t   _feature_flags;      // to do, use correct type
    DDCA_MCCS_Version_Spec _vspec;
    uint8_t   _mh;
    uint8_t   _ml;
    uint8_t   _sh;
    uint8_t   _sl;


signals:

public slots:

private:
    QHBoxLayout *_layout;



    QLabel*   _featureCodeField;
    QLabel*   _featureNameField;
    QLabel*   _featureRwField;
    QLabel*   _featureTypeField;
    ValueAbstractWidget * _valueWidget;

#ifdef ALT
    QStackedWidget * _featureValueStackedWidget; 
    QWidget*  _featureValueStandardWidget;
    QWidget*  _featureValueContWidget;
    QWidget*  _featureValueNcWidget;
#endif


};

#endif // FEATURE_WIDGET_H
