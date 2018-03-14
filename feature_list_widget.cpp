/* feature_list_widget.cpp */

#include "feature_list_widget.h"
#include "feature_base_model.h"
#include "feature_value.h"
#include "feature_widget.h"
#include <assert.h>

FeatureListWidget::FeatureListWidget(QWidget * parent):
    QListWidget(parent)
{

}


void FeatureListWidget::setModel(FeatureBaseModel * baseModel) {
    printf("(FeatureListWidget::%s)\n", __func__); fflush(stdout);
    _baseModel = baseModel;
}

void FeatureListWidget::addFeature(FeatureValue * fv) {
    printf("(FeatureListWidget::%s)\n", __func__); fflush(stdout);
    FeatureWidget * itemWidget = new FeatureWidget();
    itemWidget->setFeatureValue(*fv);
    // todo: insert at proper location
    addItem(itemWidget);
}


int FeatureListWidget::findFeatureItem(uint8_t feature_code) {
    int result = -1;

    for (int ndx = 0; ndx < count(); ndx++) {
        FeatureWidget * curItem = (FeatureWidget*) item(ndx);  // ???
        if (curItem->_feature_code == feature_code) {
            result = ndx;
            break;
        }
    }

    return result;
}


FeatureWidget * FeatureListWidget::getFeatureItem(uint8_t feature_code) {
    FeatureWidget * result = NULL;

    for (int ndx = 0; ndx < count(); ndx++) {
        FeatureWidget * curItem = (FeatureWidget *) item(ndx);
        if (curItem->_feature_code == feature_code) {
            result = curItem;
            break;
        }
    }

    return result;
}


void FeatureListWidget::updateFeature(FeatureValue * fv) {
    FeatureWidget * itemWidget = getFeatureItem(fv->_feature_code);
    assert(itemWidget);

    uint16_t newval = (fv->_sh << 8) | fv->_sl;
    itemWidget->setCurrentValue(newval);
    // signal paint event?
}



// void modelMccsVersionSet(DDCA_MCCS_Version_Spec    vspec);
// DDCA_MCCS_Version_Spec mccsVersionSpec();

void  FeatureListWidget::startInitialLoad(void) {

}

void   FeatureListWidget::endInitialLoad(void) {

}

void  FeatureListWidget::featureAdded(FeatureValue& fv) {
    printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, fv._feature_code); fflush(stdout);
    FeatureValue * fv2 = _baseModel->modelVcpValueFind(fv._feature_code);
    assert(fv2);
    addFeature(&fv);

}

void   FeatureListWidget::featureUpdated(char feature_code) {
    printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, feature_code); fflush(stdout);
    FeatureValue * fv = _baseModel->modelVcpValueFind(feature_code);
    assert(fv);
    updateFeature(fv);

}

#ifdef OLD
void    FeatureListWidget::featureChangedObserver(uint8_t feature_code) {
     printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, feature_code); fflush(stdout);
     FeatureValue * fv = _baseModel->modelVcpValueFind(feature_code);
     assert(fv);
     addFeature(fv);

}
#endif

void    FeatureListWidget::featureChanged(uint8_t feature_code)  {
     printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, feature_code); fflush(stdout);
     FeatureValue * fv = _baseModel->modelVcpValueFind(feature_code);
     assert(fv);
     addFeature(fv);

}


