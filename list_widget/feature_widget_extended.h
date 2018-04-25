/* feature_widget_extended.h - Custom widget for displaying/editing a VCP feature */

#ifndef FEATURE_WIDGET_EXTENDED_H
#define FEATURE_WIDGET_EXTENDED_H

#include "base/ddcui_globals.h"
#include "nongui/feature_value.h"

#include "feature_value_widgets/value_abstract_widget.h"

class QLabel;
class QHBoxLayout;

class FeatureWidgetExtended :
        public QWidget,
        public QListWidgetItem     // n. does not inherit from QWidget
{
    Q_OBJECT

public:
    explicit FeatureWidgetExtended(QListWidget *parent = nullptr);

    FeatureWidgetExtended(FeatureValue *fv, QWidget *parent=nullptr);

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

signals:

public slots:

protected:
//    void paintEvent(QPaintEvent *event);

private:
    QHBoxLayout *_layout;

    QLabel*   _featureCodeField;
    QLabel*   _featureNameField;
    QLabel*   _featureRwField;
    QLabel*   _featureTypeField;
    ValueAbstractWidget * _valueWidget;

#ifdef ALT
    QStackedWidget * _featureValueStackedWidget; 
    QWidget*  _featureValueStandardWidget;
    QWidget*  _featureValueContWidget;
    QWidget*  _featureValueNcWidget;
#endif

};

#endif // FEATURE_WIDGET_EXTENDED_H
