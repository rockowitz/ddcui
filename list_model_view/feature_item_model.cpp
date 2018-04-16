/* feature_item_model.cpp */

#include "assert.h"
#include <iostream>

#include <QtCore/QByteArray>
#include <QtCore/QVector>

#include "ddcutil_c_api.h"

#include "nongui/feature_value.h"

#include "feature_item_model.h"

using namespace std;

FeatureItemModel::FeatureItemModel(FeatureBaseModel * baseModel, QObject *parent)
    : QAbstractListModel(parent)
{
        this->_baseModel = baseModel;
}


QVariant FeatureItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    QString s = QString("(FeatureItemModel::headerData) section=%1, orientation=%2, role=%3")
            .arg(section)
            .arg(orientation)
            .arg(role);
    return QVariant(s);      // what to do?
}

int FeatureItemModel::rowCount(const QModelIndex &parent) const
{
    int result = 0;
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (!parent.isValid()) {

       // FIXME: Implement me!
       result = _baseModel->modelVcpValueCount();
    }
    printf("(FeatureItemModel::rowCount) Returning %d\n", result);
    return result;
}

QVariant FeatureItemModel::data(const QModelIndex &index, int role) const
{
    printf("(FeatureItemModel::data) Starting. role=%d\n", role);
    if (!index.isValid())
        return QVariant();

    // how to convert index to feature code/index in monitor?

    // FeatureValue * fv = _monitor->currentVcpValueFind(feature_code);

    // will this work?
    int rowNdx = index.row();
    FeatureValue * fv = _baseModel->modelVcpValueAt(rowNdx);

    QVariant result = QVariant();

    switch(role) {
    case(Qt::DisplayRole):
    {
        char * s_name = ddca_feature_name_by_vspec(fv->_feature_code, fv->_vspec);


        char * s_formatted = NULL;
        DDCA_Status rc = ddca_format_non_table_vcp_value(
                             fv->_feature_code,
                             fv->_vspec,
                             &fv->_mmid,
                             &fv->_value,
                             &s_formatted);
        if (rc != 0)
            s_formatted = (char*) "invalid formatted value";   // explicit cast to avoid compiler warning

        char * s_rw = NULL;
        if (fv->_feature_flags & DDCA_RW)
            s_rw = (char*) "RW";
        else if (fv->_feature_flags & DDCA_RO)
            s_rw = (char*)  "RO";
        else {
            assert (fv->_feature_flags & DDCA_WO) ;
            s_rw = (char*) "WO";
        }

        char * s_mccs_type = NULL;
        if (fv->_feature_flags & DDCA_CONT)
            s_mccs_type =(char*)  "C";
        else if (fv->_feature_flags & DDCA_NC)
            s_mccs_type = (char*) "NC";
        else {
            assert ( fv->_feature_flags & DDCA_TABLE) ;
            s_mccs_type =(char*)  "T";
        }

        char buffer[100];
        snprintf(buffer, 100, "%02x  %-30s %-2s %-2s %-40s",
                 fv->_feature_code, s_name, s_mccs_type, s_rw, s_formatted  );
        printf("(FeatureItemModel::data) buffer=|%s|\n", buffer);

        result = QString(buffer);
        break;
    }
    case(Qt::UserRole):     // probably some constant UserRole+n
        // todo custom QVaraiant
        break;
    default:
        break;
    }

    // FIXME: Implement me!
    // return QVariant();
    return result;
}


void  FeatureItemModel::startInitialLoad(void) {
    // TODO replace with signal
    beginResetModel();
}

void  FeatureItemModel::endInitialLoad(void) {
    // TODO: replace with signal
    endResetModel();
}




#ifdef OLD

struct featureInfo {
    int                         monitorNdx;
    uint8_t                     feature_code;
    DDCA_Version_Feature_Info * feature_info;
    DDCA_Non_Table_Value *      feature_value;
    QString *                   formatted_value;
};


void FeatureItemModel::setMonitorInfo(
        int monitorNdx,
        DDCA_Version_Feature_Info * info)
{

}

void FeatureItemModel::setFeatureValue(
        int      monitorNdx,
        uint8_t  feature_code,
        DDCA_Non_Table_Value * ntv)
{

}

void FeatureItemModel::setFormattedFeatureValue(
        int     moitorNdx,
        uint8_t featureCode,
        char *  formatted)
{

}
#endif

