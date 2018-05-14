/* feature_item_model.h */

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

#ifndef FEATUREITEMMODEL_H
#define FEATUREITEMMODEL_H

#include <QtCore/QAbstractListModel>

#include "nongui/feature_base_model.h"
#include "nongui/feature_value.h"


class FeatureItemModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FeatureItemModel(FeatureBaseModel * baseModel, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const char * _cls;    // className

public slots:
    void          startInitialLoad(void);
    void          endInitialLoad(void);

private:
    FeatureBaseModel * _baseModel;

};

#endif // FEATUREITEMMODEL_H
