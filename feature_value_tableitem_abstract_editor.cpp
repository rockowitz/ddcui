#include "feature_value_tableitem_abstract_editor.h"

FeatureValueTableItemAbstractEditor::FeatureValueTableItemAbstractEditor(QWidget *parent)
    : QWidget(parent)
{
  setAutoFillBackground(true);
}

QSize FeatureValueTableItemAbstractEditor::sizeHint() const {
    printf("============> (FeatureValueTableItemContEditor::sizeHint)\n" ); fflush(stdout);
    return QSize(100,50);
}
