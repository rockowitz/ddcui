#ifndef FEATUREITEMMODEL_H
#define FEATUREITEMMODEL_H

#include <QAbstractListModel>
#include "feature_base_model.h"
#include "feature_value.h"

class FeatureItemModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FeatureItemModel(FeatureBaseModel * baseModel, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void          startInitialLoad(void);
    void          endInitialLoad(void);


private:
    FeatureBaseModel * _baseModel;

};

#endif // FEATUREITEMMODEL_H
