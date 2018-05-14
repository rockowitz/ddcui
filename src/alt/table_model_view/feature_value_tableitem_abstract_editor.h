/* feature_value_tableitem_abstract_editor.h */

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

#ifndef FEATURE_VALUE_TABLEITEM_ABSTRACT_EDITOR_H
#define FEATURE_VALUE_TABLEITEM_ABSTRACT_EDITOR_H

#include <QtWidgets/QWidget>

#include "nongui/feature_value.h"


class FeatureValueTableItemAbstractEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FeatureValueTableItemAbstractEditor(QWidget *parent = nullptr);

    virtual QSize sizeHint() const override;
    virtual void setFeatureValue(const FeatureValue &fv) = 0;
    virtual FeatureValue featureValue() = 0;
    virtual void setCurValue(ushort curVal) = 0;

    // signals are protected in QT4, public in QT5
signals:
    void editingFinished();

protected:
    virtual void paintEvent(QPaintEvent *event) =0;

#ifdef ARE_THESE_NEEDED
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif

public slots:

protected:
    FeatureValue  _fv;   // should probably be a pointer
};

#endif // FEATURE_VALUE_TABLEITEM_ABSTRACT_EDITOR_H
