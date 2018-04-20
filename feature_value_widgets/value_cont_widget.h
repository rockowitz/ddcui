/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

// #include <QtWidgets/QWidget>
// #include <QtWidgets/QSlider>
// #include <QtWidgets/QSpinBox>
// #include <QtWidgets/QLabel>

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_base_widget.h"

class QSlider;
class QSpinBox;
class QLabel;
class QPushButton;


class ValueContWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueContWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    QSlider*    _curSlider;
    QSpinBox*   _curSpinBox;
    QLabel*     _maxTitle;
    QLabel*     _maxValue;

    QPushButton * _applyButton;
    QPushButton * _cancelButton;


protected:
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);

    // void leaveEvent(QEvent * event);

private:
    uint16_t _newval;

    QTimer* _spinBoxTimer;
    bool    _guiChange = false;

private slots:

    void onSliderValueChanged(int value);

    void onSliderReleased();

    void onSpinBoxValueChanged(int value);

    void onSpinBoxTimedOut();

    void onSpinBoxEditingFinished();

    void onApplyButtonClicked(bool checked);

    void onCancelButtonClicked(bool checked);

};

#endif // VALUE_CONT_WIDGET_H
