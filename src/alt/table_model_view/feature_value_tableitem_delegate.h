/* feature_value_tableitem_delegate.h */

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

#ifndef FEATURE_VALUE_TABLEITEM_DELEGATE_H
#define FEATURE_VALUE_TABLEITEM_DELEGATE_H

#include <QtCore/QObject>
#include <QtWidgets/QStyledItemDelegate>
// #include "c_vcp.h"
#include "feature_value_widgets/value_base_widget.h"

class FeatureValueTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    FeatureValueTableItemDelegate();
    ~FeatureValueTableItemDelegate();

    // ?? paint(), sizeHint() - in StarDelegate

    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const override;

    QSize sizeHint(
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const override;

    virtual QWidget* createEditor(
            QWidget *                   parent,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
            ) const  override;

    virtual void setEditorData(
            QWidget* editor,
            const QModelIndex& index
            ) const;    // ? override

    virtual void setModelData(
            QWidget* editor,
            QAbstractItemModel* model,
            const QModelIndex& index
            ) const;  //  ? override

    virtual void updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const;

    // public/protected/private ignored by metaobject system
public slots:    // StarDelegate   - was private slots
    void commitAndCloseEditor();

private:
    ValueBaseWidget* _valueWidget;
};

#endif // FEATURE_VALUE_TABLEITEM_DELEGATE_H
