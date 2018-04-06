/* feature_value_tableitem_delegate.h */

#ifndef FEATURE_VALUE_TABLEITEM_DELEGATE_H
#define FEATURE_VALUE_TABLEITEM_DELEGATE_H

#include <QtCore/QObject>
#include <QtWidgets/QStyledItemDelegate>
// #include "c_vcp.h"
#include "feature_value_widgets/value_base_widget.h"

class FeatureValueTableItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    FeatureValueTableItemDelegate();
    ~FeatureValueTableItemDelegate();

    // ?? paint(), sizeHint() - in StarDelegate

    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const override;

    QSize sizeHint(
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const override;

    virtual QWidget* createEditor(
            QWidget *                   parent,
            const QStyleOptionViewItem& option,
            const QModelIndex&          index
            ) const  override;

    virtual void setEditorData(
            QWidget* editor,
            const QModelIndex& index
            ) const;    // ? override

    virtual void setModelData(
            QWidget* editor,
            QAbstractItemModel* model,
            const QModelIndex& index
            ) const;  //  ? override

    virtual void updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &index
            ) const;

    // public/protected/private ignored by metaobject system
public slots:    // StarDelegate   - was private slots
    void commitAndCloseEditor();

private:
    ValueBaseWidget* _valueWidget;
};

#endif // FEATURE_VALUE_TABLEITEM_DELEGATE_H
