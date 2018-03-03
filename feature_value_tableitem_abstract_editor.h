/* feature_value_tableitem_abstract_editor.h */

#ifndef FEATURE_VALUE_TABLEITEM_ABSTRACT_EDITOR_H
#define FEATURE_VALUE_TABLEITEM_ABSTRACT_EDITOR_H

#include <QtWidgets/QWidget>

#include "feature_value.h"


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
