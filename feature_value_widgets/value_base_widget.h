/* value_base_widget.h - Common features of all Value_..._Widgets */

#ifndef VALUE_BASE_WIDGET_H
#define VALUE_BASE_WIDGET_H

// #include <QtCore/QSize>
#include <QtWidgets/QFrame>

#include <ddcutil_c_api.h>

#include "nongui/feature_value.h"
#include "feature_value_widgets/value_abstract_widget.h"

class FeatureBaseModel;

class ValueBaseWidget : public ValueAbstractWidget
{
    Q_OBJECT

public:
    explicit ValueBaseWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    // QSize sizeHint() const override;   //   needed?

    uint8_t                _featureCode;
    DDCA_Display_Ref       _dref;
    DDCA_Feature_Metadata  _finfo;
    DDCA_Cap_Vcp *         _capVcp;
    uint8_t                _mh;
    uint8_t                _ml;
    uint8_t                _sh;
    uint8_t                _sl;

protected:
    char * _cls;

signals:
// compiler warning: signals may not be declared virtual
void featureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);
// virtual void featureValueChanged(uint16_t val);

public slots:
};

#endif // VALUE_BASE_WIDGET_H
