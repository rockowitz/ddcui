/* feature_value_tableitem_cont_editor.h */

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

#ifndef FEATURE_VALUE_TABLEITEM_CONT_EDITOR_H
#define FEATURE_VALUE_TABLEITEM_CONT_EDITOR_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

#include "nongui/feature_value.h"

#include "alt/table_model_view/feature_value_tableitem_abstract_editor.h"

class FeatureValueTableItemContEditor : public FeatureValueTableItemAbstractEditor
{
    Q_OBJECT

public:
    explicit FeatureValueTableItemContEditor(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setFeatureValue(const FeatureValue &fv);
    FeatureValue featureValue();

    void setCurValue(ushort curVal);

// signals are protected in QT4, public in QT5
signals:
    void editingFinished();

protected:
        void paintEvent(QPaintEvent *event) override;
#ifdef ARE_THESE_NEEDED

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif

public slots:

private:
    // FeatureValue  _fv;   // should probably be a pointer

    QSlider*    _curSlider;
    QSpinBox*   _curSpinBox;
    QLabel*     _maxTitle;
    QLabel*     _maxValue;

};

#endif // FEATURE_VALUE_TABLEITEM_CONT_EDITOR_H
