/* feature_list_widget.h - List widget containing FeatureListWidgets */

/* <copyright>
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

#ifndef FEATURE_LIST_WIDGET_H
#define FEATURE_LIST_WIDGET_H

#include "alt/list_widget/feature_widget_extended.h"

#include <QtWidgets/QWidget>

#include <ddcutil_c_api.h>

#include "nongui/feature_base_model.h"
#ifdef FEATURE_CHANGE
#include "nongui/feature_change_observer.h"
#endif


class FeatureListWidget : public QListWidget
#ifdef FEATURE_LIST_WIDGET
                        , public FeatureChangeObserver    // no longer needed
#endif
{
    Q_OBJECT

public:
    FeatureListWidget(QWidget *parent = Q_NULLPTR);

    void setModel(FeatureBaseModel * baseModel);

    const char * _cls;    // className

    void dbgrpt() const;

public slots:

#ifdef NO
    void  modelVcpValueSet(
                       uint8_t                              feature_code,
                       DDCA_MCCS_Version_Spec               vspec,
                       DDCA_Simplified_Version_Feature_Info feature_flags,
                       DDCA_Non_Table_Value *               feature_value);

    void  modelVcpValueUpdate(
                       uint8_t                              feature_code,
                       uint8_t                              sh,
                       uint8_t                              sl);
#endif
    // void modelMccsVersionSet(DDCA_MCCS_Version_Spec    vspec);
    // DDCA_MCCS_Version_Spec mccsVersionSpec();

    void            startInitialLoad(void);
    void            endInitialLoad(void);

    void            featureAdded(FeatureValue fv);
    void            featureUpdated(char feature_code);
    void            featureUpdated3(const char * caller, uint8_t feature_code, uint8_t sh, uint8_t sl);

    void            featureChanged(char * caller, char feature_code, char sh, char sl); //  override;    // virtual

protected:
    void            paintEvent(QPaintEvent *event);

private:
    int             findFeatureItem(uint8_t feature_code);
    FeatureWidgetExtended * getFeatureItem(uint8_t feature_code) ;
    void            addFeature(FeatureValue * fv);
    void            updateFeature(FeatureValue * fv);

    FeatureBaseModel * _baseModel;
};

#endif // FEATURE_LIST_WIDGET_H
