/* value_nc_widget.h - Widget for displaying/editing a simple Non-Continuous VCP feature value */

#ifndef VALUE_NC_WIDGET_H
#define VALUE_NC_WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>

#include "base/other_options_state.h"
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
    DDCA_Feature_Value_Entry * getComboBoxEntries(NcValuesSource mode);
    void loadComboBox(NcValuesSource mode);
    void reloadComboBox(NcValuesSource newSource);

private:
    QComboBox *    _cb;
    QPushButton *  _applyButton;
    QPushButton *  _cancelButton;
    NcValuesSource _curNcValuesSource = NcValuesSourceUnset;
    bool           _guiChange = false;

private slots:
    void combobox_activated(int index);
};

#endif // VALUE_NC_WIDGET_H
