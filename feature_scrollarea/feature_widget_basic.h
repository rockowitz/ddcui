/* feature_widge_basict.h - Custom widget for displaying/editing a VCP feature */

#ifndef FEATURE_WIDGET_BASIC_H
#define FEATURE_WIDGET_BASIC_H

#include "base/ddcui_globals.h"
#include "nongui/feature_value.h"
#include "feature_value_widgets/value_abstract_widget.h"
#include "feature_value_widgets/value_stacked_widget.h"

// Like FeatureWidget, but derives only from QWidget, not QListWidgetItem

class QLabel;
class QHBoxLayout;


class FeatureWidgetBasic :
        public QWidget         // QFrame
        , public SimpleFeatureValueObserver
{
    Q_OBJECT

public:
    explicit FeatureWidgetBasic(QWidget *parent = nullptr);

    // FeatureWidgetBasic(FeatureValue *fv, QWidget *parent=nullptr);

    void setFeatureValue(FeatureValue &fv);

    void setCurrentValue(uint16_t newval);

    void dbgrpt() const;

    uint8_t   _feature_code;
    QString*  _feature_name;
    DDCA_Version_Feature_Flags   _feature_flags;      // uint16_t
    DDCA_MCCS_Version_Spec _vspec;
    uint8_t   _mh;
    uint8_t   _ml;
    uint8_t   _sh;
    uint8_t   _sl;

    const char * _cls;    // className

    QSize sizeHint() const override;


    void simpleFeatureValueChanged(SimpleFeatureValue fv) override;   // SimpleFeatureValueObserver

signals:

public slots:
void onInternalValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl);

signals:
void valueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl);

protected:

    // void paintEvent(QPaintEvent *event) override;

private:
    QHBoxLayout *_layout;

    QLabel*   _featureCodeField;
    QLabel*   _featureNameField;
    QLabel*   _featureRwField;
    QLabel*   _featureTypeField;
    ValueStackedWidget * _valueWidget;

#ifdef ALT
    QStackedWidget * _featureValueStackedWidget; 
    QWidget*  _featureValueStandardWidget;
    QWidget*  _featureValueContWidget;
    QWidget*  _featureValueNcWidget;
#endif

};

#endif // FEATURE_WIDGET_BASIC_H
