/* feature_value_tableitem_cb_editor.h */

#ifndef FEATURE_VALUE_TABLEITEM_CB_EDITOR_H
#define FEATURE_VALUE_TABLEITEM_CB_EDITOR_H

#include "alt/table_model_view/feature_value_tableitem_abstract_editor.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>

#include "nongui/feature_value.h"


class FeatureValueTableItemCbEditor : public FeatureValueTableItemAbstractEditor
{
    Q_OBJECT

public:
    explicit FeatureValueTableItemCbEditor(QWidget *parent = nullptr);

    QSize        sizeHint() const override;
    void         setFeatureValue(const FeatureValue &fv);
    FeatureValue featureValue();
    void         setCurValue(ushort curVal);

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
    QComboBox * _cb;

    int findItem(uint8_t sl_value);
};

#endif // FEATURE_VALUE_TABLEITEM_CB_EDITOR
