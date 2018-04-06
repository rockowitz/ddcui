/* feature_table_model.h */

#ifndef FEATURETABLEMODEL_H
#define FEATURETABLEMODEL_H

#include <QtCore/QAbstractTableModel>

#include "nongui/feature_base_model.h"


class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FeatureTableModel(FeatureBaseModel* baseModel, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

#ifdef UNNEEDED
    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
#endif

    const char * _cls;    // className

public slots:
    void          startInitialLoad(void);
    void          endInitialLoad(void);

private:
    FeatureBaseModel * _baseModel;
};

#endif // FEATURETABLEMODEL_H
