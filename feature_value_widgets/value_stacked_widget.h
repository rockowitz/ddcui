#ifndef VALUE_STACKED_WIDGET_H
#define VALUE_STACKED_WIDGET_H

#include "nongui/simple_feature_value.h"
#include "nongui/simple_feature_value_subject.h"

#include "value_abstract_widget.h"
#include "value_base_widget.h"
#include "value_std_widget.h"
#include "value_cont_widget.h"
#include "value_nc_widget.h"
#include "value_reset_widget.h"
#include "value_2button_widget.h"

class QWidget;
class QPaintEvent;
class QStackedWidget;

class ValueStackedWidget : public ValueBaseWidget, public SimpleFeatureValueSubject
{
   Q_OBJECT

public:
    explicit ValueStackedWidget(QWidget *parent = nullptr);

    void     setFeatureValue(const FeatureValue &fv) override;
    void     setCurrentValue(uint16_t newval) override;
    uint16_t getCurrentValue() override;

    // QSize sizeHint() const override;
    // void paintEvent(QPaintEvent *event) override;

    bool isSimpleNc();
    void setNcValuesSource(NcValuesSource newsrc);

signals:
    void stackedFeatureValueChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

private:
    QStackedWidget * _stacked;
    ValueBaseWidget * _cur_stacked_widget;

    int  _pageno_std;
    int  _pageno_cont;
    int  _pageno_nc;
    int  _pageno_reset;
    int  _pageno_2button;
    int  _pageno_selected;

    ValueContWidget    * _contWidget;
    ValueNcWidget      * _ncWidget;
    ValueResetWidget   * _resetWidget;
    Value2ButtonWidget * _2ButtonWidget;
    ValueStdWidget     * _stdWidget;

private slots:
    void onContainedWidgetChanged(uint8_t feature_code, uint8_t sh, uint8_t sl);

};

#endif // VALUE_STACKED_WIDGET_H
