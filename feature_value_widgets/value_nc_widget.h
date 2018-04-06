/* value_nc_widget.h - Widget for displaying/editing a simple Non-Continuous VCP feature value */

#ifndef VALUE_NC_WIDGET_H
#define VALUE_NC_WIDGET_H

#include <QWidget>
#include <QtWidgets/QComboBox>

#include "feature_value_widgets/value_base_widget.h"


class ValueNcWidget : public ValueBaseWidget
{
    Q_OBJECT

public:
    ValueNcWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    int findItem(uint8_t sl_value);

    QComboBox *_cb;
};

#endif // VALUE_NC_WIDGET_H
