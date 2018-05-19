/* feature_widget.h - Custom widget for displaying/editing a VCP feature */

/*
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef FEATURE_WIDGET_H
#define FEATURE_WIDGET_H

#include "base/ddcui_globals.h"
#include "nongui/feature_value.h"

// #include "feature_value_widgets/value_abstract_widget.h"
#include "feature_value_widgets/value_base_widget.h"
#include "feature_value_widgets/value_stacked_widget.h"

// Like FeatureWidgetExtended, but derives only from QWidget, not QListWidgetItem

class QLabel;
class QHBoxLayout;

class FeatureBaseModel;


class FeatureWidget :
        public QWidget
        // , public SimpleFeatureValueObserver
{
    Q_OBJECT

public:
    explicit FeatureWidget(QWidget *parent = nullptr);
    FeatureWidget(FeatureValue& fv, QWidget *parent=nullptr);
    void setFeatureValue(FeatureValue &fv);

    void setCurrentValue(uint16_t newval);

    void dbgrpt() const;

    QSize sizeHint() const override;

    // void simpleFeatureValueChanged(SimpleFeatureValue fv) override;   // SimpleFeatureValueObserver
    bool isSimpleNc();
    void setNcValuesSource(NcValuesSource newsrc);

public slots:
    void onInternalValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl);

signals:
    void valueChanged(uint8_t featureCode, bool writeOnlyFeature, uint8_t sh, uint8_t sl);

protected:

    // void paintEvent(QPaintEvent *event) override;

private:
    void setupFeatureWidget();   // called by constructor

    const char *        _cls;    // className

    uint8_t             _feature_code;
    QString*            _feature_name;
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
