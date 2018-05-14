/* feature_value_tableitem_editor.h */

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

#ifndef FEATURE_VALUE_TABLEITEM_EDITOR_H
#define FEATURE_VALUE_TABLEITEM_EDITOR_H

#include <QtWidgets/QWidget>

#include "nongui/feature_value.h"
// #include "simple_feature_value.h"

class FeatureValueTableItemEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FeatureValueTableItemEditor(QWidget *parent = nullptr);

    QSize sizeHint() const override;

    // why are these functions defined in the header?
    void setFeatureValue(const FeatureValue &fv) {
        _featureValue = fv;
    }

    FeatureValue featureValue() {
        return _featureValue;
    }

// signals are protected in QT4, public in QT5
signals:
    void editingFinished();

protected:
#ifdef ARE_THESE_NEEDED
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif

public slots:

private:
    FeatureValue  _featureValue;   // should probably be a pointer
};

#endif // FEATURE_VALUE_TABLEITEM_EDITOR_H
