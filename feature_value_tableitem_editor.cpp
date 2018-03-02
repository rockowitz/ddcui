#include "feature_value_tableitem_editor.h"
#include <iostream>
#include <stdio.h>
#include <string.h>



FeatureValueTableItemEditor::FeatureValueTableItemEditor(QWidget *parent) : QWidget(parent)
{
  // std::cout << "(FeatureValueTableItemEditor::FeatureValueTableItemEditor)" << std::endl;
  printf("============> (FeatureValueTableItemEditor::FeatureValueTableItemEditor)\n" ); fflush(stdout);
}

QSize FeatureValueTableItemEditor::sizeHint() const {
    printf("============> (FeatureValueTableItemEditor::sizeHint)\n" ); fflush(stdout);
    return QSize(100,50);
}
