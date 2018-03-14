#ifndef VALUE_BASE_WIDGET_H
#define VALUE_BASE_WIDGET_H

#include <QWidget>

#include <ddcutil_c_api.h>

#include "value_abstract_widget.h"
#include "feature_value.h"

class ValueBaseWidget : public ValueAbstractWidget
{
    Q_OBJECT

public:
    explicit ValueBaseWidget(QWidget *parent = nullptr);

    virtual void setFeatureValue(const FeatureValue &fv);

    virtual void setCurrentValue(uint16_t newval);

    virtual uint16_t getCurrentValue();

    uint8_t                _feature_code;
    DDCA_MCCS_Version_Spec _vspec;
    DDCA_Non_Table_Value   _value;    // use this or individual bytes? DDCA_Non_Table_Value needed for get_formatted_value call
    uint8_t                _mh;
    uint8_t                _ml;
    uint8_t                _sh;
    uint8_t                _sl;
    DDCA_Feature_Flags     _feature_flags;  // should this be here or looked up?

    QSize sizeHint() const override;   //   needed?

signals:

public slots:
};

#endif // VALUE_BASE_WIDGET_H
