/* feature_value_tableitem_abstract_editor.cpp */

#include "alt/table_model_view/feature_value_tableitem_abstract_editor.h"

FeatureValueTableItemAbstractEditor::FeatureValueTableItemAbstractEditor(QWidget *parent)
    : QWidget(parent)
{
  setAutoFillBackground(true);
}

QSize FeatureValueTableItemAbstractEditor::sizeHint() const {
    printf("============> (FeatureValueTableItemContEditor::sizeHint)\n" ); fflush(stdout);
    return QSize(100,50);
}
