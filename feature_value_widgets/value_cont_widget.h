/* value_cont_widget.h - Widget for displaying/editing a Continuous VCP feature value */

#ifndef VALUE_CONT_WIDGET_H
#define VALUE_CONT_WIDGET_H

#include "config.h"

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

#ifdef APPLY_CANCEL
    QPushButton * _applyButton;
    QPushButton * _cancelButton;
#endif


protected:
#ifdef UNUSED
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
#endif

    // void leaveEvent(QEvent * event);

private:
    uint16_t _newval;

    QTimer* _spinBoxTimer;
    bool    _guiChange = false;

private slots:
    void onSliderReleased();
    // void onSliderValueChanged(int value);  // unused
    // void onSpinBoxEditingFinished();     // unused
    void onSpinBoxTimedOut();
    void onSpinBoxValueChanged(int value);

#ifdef APPLY_CANCEL
    void onApplyButtonClicked(bool checked);
    void onCancelButtonClicked(bool checked);
#endif

};

#endif // VALUE_CONT_WIDGET_H
