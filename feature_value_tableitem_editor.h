#ifndef FEATURE_VALUE_TABLEITEM_EDITOR_H
#define FEATURE_VALUE_TABLEITEM_EDITOR_H

#include <QWidget>
#include "feature_value.h"
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
