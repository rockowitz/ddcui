/* feature_widget.h - Custom widget for displaying/editing a VCP feature */

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURE_WIDGET_H
#define FEATURE_WIDGET_H

#include "../base/core.h"
#include "nongui/feature_value.h"

#include "feature_value_widgets/value_stacked_widget.h"

class QLabel;
class QHBoxLayout;
class FeatureBaseModel;
class QResizeEvent;


class FeatureWidget :
        public QWidget
        // , public SimpleFeatureValueObserver
{
    Q_OBJECT

public:
    explicit FeatureWidget(QWidget *parent = nullptr);
    FeatureWidget(FeatureValue& fv, QWidget *parent=nullptr);

    // QSize sizeHint() const override;

    void setFeatureValue(FeatureValue &fv);
    void setCurrentValue(uint16_t newval);
    bool hasSlTable();
    void setNcValuesSource(NcValuesSource newsrc);
    void dbgrpt() const;
    // void simpleFeatureValueChanged(SimpleFeatureValue fv) override;   // SimpleFeatureValueObserver

public:            // member variables
    uint8_t             _feature_code;
    QString*            _feature_name;

protected:
    void resizeEvent(QResizeEvent * event) override;

public slots:
    void onInternalValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl);

signals:
    void valueChanged(uint8_t featureCode, bool writeOnlyFeature, uint8_t sh, uint8_t sl);

private:
    void setupFeatureWidget();   // called by constructor
    void setupConnections();     // called by constructor

private:
    const char *        _cls;    // className

    // uint8_t          _feature_code;
   //  QString*         _feature_name;
    DDCA_Version_Feature_Flags  _feature_flags;      // uint16_t

    QHBoxLayout*        _layout;
    QLabel*             _featureCodeField;
    QLabel*             _featureNameField;
    QLabel*             _featureRwField;
    QLabel*             _featureTypeField;
    ValueStackedWidget* _valueWidget;

#ifdef ALT
    QStackedWidget * _featureValueStackedWidget; 
    QWidget*  _featureValueStandardWidget;
    QWidget*  _featureValueContWidget;
    QWidget*  _featureValueNcWidget;
#endif

};

#endif // FEATURE_WIDGET_H
