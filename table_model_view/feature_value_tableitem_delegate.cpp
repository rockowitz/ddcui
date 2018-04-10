/* feature_value_tableitem_delegate.cpp */

#include "../table_model_view/feature_value_tableitem_delegate.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtWidgets/QApplication>

#include "base/ddcui_globals.h"

#include "nongui/feature_value.h"

#include "feature_value_widgets/value_stacked_widget.h"
#include "feature_value_widgets/value_std_widget.h"

#include "feature_value_tableitem_abstract_editor.h"
#include "feature_value_tableitem_cb_editor.h"
#include "feature_value_tableitem_cont_editor.h"


FeatureValueTableItemDelegate::FeatureValueTableItemDelegate()
{
  std::cout << "(FeatureValueTableItemDelegate::FeatureValueTableItemDelegate" << std::endl;
  printf("-------------------------> (FeatureValueTableItemDelegate::FeatureValueTableItemDelegate)\n"); fflush(stdout);
  _valueWidget = new ValueStdWidget();   // TEMP     // HOW TO SET PARENT?
  // _valueWidget = new ValueStackedWidget(this);  // eventual
}

FeatureValueTableItemDelegate::~FeatureValueTableItemDelegate()
{
}

void FeatureValueTableItemDelegate::paint(
        QPainter                   *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex          &index
       ) const
{
    // printf("=====> (FeatureValueTableItemDelegate::paint)\n");
    // Todo: also test if its a type of feature value we know how to edit
    bool canconvert = index.data(FeatureValueRole).canConvert<FeatureValue>();
    // int col = index.column(); // unused
    // printf("===> (FeatureValueTableItemDelegate::paint) canconvert=%d, col=%d\n", canconvert, col); fflush(stdout);
    if ( canconvert && index.column() == 4) {
        // printf("=====> (FeatureValueTableItemDelegate::paint) FeatureValue case\n"); fflush(stdout);
        FeatureValue fv = qvariant_cast<FeatureValue>(index.data(FeatureValueRole));
        // fv.report();
        if (fv._feature_flags & (DDCA_CONT | DDCA_SIMPLE_NC)) {
            // printf("    continuous feature\n");  fflush(stdout);
            // int maxval = fv._mh << 8 | fv._ml;  // unused
            // int curval = fv._sh << 8 | fv._sl;  // unused
            QStyleOptionViewItem myOption = option;
            myOption.displayAlignment = Qt::AlignLeft;
#ifdef PROGRESS_BAR
            QString text = QString("--%1--")
                    .arg(fv._feature_code);
            // drawDisplay(painter, myOption, myOption.rect, text);
            // drawFocus(  painter, myOption, myOption.rect);

            QStyleOptionProgressBar progressBarOption;
            progressBarOption.rect = option.rect;
            progressBarOption.minimum = 0;
            progressBarOption.maximum = maxval;
            progressBarOption.progress = curval;
            progressBarOption.text = QString::number(curval) + QString("/") + QString::number(maxval);
            progressBarOption.textVisible = true;

            QApplication::style()->drawControl(
                                        QStyle::CE_ProgressBar,
                                        &progressBarOption,
                                        painter);
#endif


          // QApplication::style()->drawControl(
          //                             QStyle::CE_ProgressBar,
         //                              myOption,
         //                              painter);

           QStyledItemDelegate::paint(painter, option, index);   // ???

           // Wrong - there's one delegate for entire column,
           // need separate _valueWidgets for each row
           // _valueWidget->setFeatureValue(fv);
           // _valueWidget->update();
        }
        else {
            // printf("      Feature Value, not Continuous\n"); fflush(stdout);
            QStyledItemDelegate::paint(painter, option, index);
        }


#ifdef BAD
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());

        fv.paint(painter, option.rect, option.palette,
                         FeatureValue::ReadOnly);
#endif
    } else {
        printf("=====> (FeatureValueTableItemDelegate::paint) Normal case - invoking QStyledItemDelegate::paint\n");
        QStyledItemDelegate::paint(painter, option, index);

    }

}

// Called when the user starts editing an item
QWidget *FeatureValueTableItemDelegate::createEditor(
            QWidget *parent,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const
{
    // printf("=====> (FeatureValueTableItemDelegate::createEditor)\n");
    bool canconvert = index.data(FeatureValueRole).canConvert<FeatureValue>();
    int col = index.column();
    printf("(FeatureValueTableItemDelegate::createEditor) canconvert=%d, col=%d\n", canconvert, col);
    fflush(stdout);
    // to do, check for kind of FeatureValue
    if ( canconvert && col == 4) {
        FeatureValue fv = qvariant_cast<FeatureValue>(index.data(FeatureValueRole));
        fv.report();

        QWidget* edwidget = nullptr;
        if (fv._feature_flags & DDCA_CONT) {
           FeatureValueTableItemContEditor *editor = new FeatureValueTableItemContEditor(parent);
           editor->setFeatureValue(fv);
           edwidget = editor;
           connect(editor, SIGNAL(editingFinished()),
                   this,   SLOT(commitAndCloseEditor()));

           // QT5 style - errors re editingFinished() protected within this context
           // connect(editor, &FeatureValueTableItemEditor::editingFinished,
           //         this,   &FeatureValueTableItemDelegate::commitAndCloseEditor);

        }
        else if (fv._feature_flags & DDCA_SIMPLE_NC) {
            FeatureValueTableItemCbEditor * editor = new FeatureValueTableItemCbEditor(parent);
            editor->setFeatureValue(fv);
            edwidget = editor;
            //  old style
            connect(editor, SIGNAL(editingFinished()),
                    this,   SLOT(commitAndCloseEditor()));
        }
        if (!edwidget) {
            edwidget =  QStyledItemDelegate::createEditor(parent, option, index);
        }

        printf("=====> (FeatureValueTableItemDelegate::createEditor) retuning custom editor %p\n", edwidget); fflush(stdout);
        return edwidget;
    } else {
        printf("(=====> FeatureValueTableItemDelegate::createEditor) retuning parent editor \n"); fflush(stdout);
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void FeatureValueTableItemDelegate::commitAndCloseEditor()
{
    FeatureValueTableItemAbstractEditor *editor = qobject_cast<FeatureValueTableItemAbstractEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

// Called when editor is created to initialize it with data from the model
void FeatureValueTableItemDelegate::setEditorData(
        QWidget           *editor,
        const QModelIndex &index
       ) const
{
    if (index.data().canConvert<FeatureValue>()) {
        FeatureValue fv = qvariant_cast<FeatureValue>(index.data());
        FeatureValueTableItemAbstractEditor *fv_editor = qobject_cast<FeatureValueTableItemAbstractEditor *>(editor);

        fv_editor->setFeatureValue(fv);
        printf("(FeatureValueTableItemDelegate::setEditorData) Calling update\n"); fflush(stdout);
        fv_editor->update();
        //fv_editor->setStarRating(starRating);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

// Called when editing is finished, to commit data from the editor to the model
// TOD:  Should not update model directly, should fire off request to set VCP value
void FeatureValueTableItemDelegate::setModelData(
        QWidget *            editor,
        QAbstractItemModel * model,
        const QModelIndex    &index
       ) const
{
    FeatureValueTableItemAbstractEditor *fv_editor = qobject_cast<FeatureValueTableItemAbstractEditor *>(editor);
    FeatureValue fv = fv_editor->featureValue();
    printf("(FeatureValueTableItemDelegate::setModelData) Calling setModelData() directly\n"); fflush(stdout);
    model->setData(index,  QVariant::fromValue(fv_editor->featureValue()));



    printf("(FeatureValueTableItemDelegate::setModelData) index=(%d,%d)\n",
           index.row(), index.column()); fflush(stdout);
        fv.report(); fflush(stdout);
    // how to determine if used editor?
    if (index.data().canConvert<FeatureValue>()) {
        printf("(FeatureValueTableItemDelegate::setModelData) FeatureValue case\n"); fflush(stdout);
        // TO CHANGE
       //  model->setData(index, QVariant::fromValue(fv_editor->featureValue()));
        // we don't have a reference to the monitor, but the model does
        // pass the request to the model
        model->setData(index,  QVariant::fromValue(fv_editor->featureValue()));
    } else {
        printf("(FeatureValueTableItemDelegate::setModelData) Normal case\n"); fflush(stdout);
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}


// Returns an item's preferred size
QSize FeatureValueTableItemDelegate::sizeHint(
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const
{
    printf("=====> (FeatureValueTableItemDelegate::sizeHint)\n"); fflush(stdout);
    QSize qsz;
    if (index.data().canConvert<FeatureValue>()) {
        // FeatureValue fv = qvariant_cast<FeatureValue>(index.data()); // unused
        // return featureValue.sizeHint(); // ??? why is size hint in model?
                qsz = QSize(20,100);
      printf("=====> (FeatureValueTableItemDelegate::sizeHint) returning bogus value %d,%d\n", qsz.width(), qsz.height());

    } else {

        qsz = QStyledItemDelegate::sizeHint(option, index);
        printf("=====> (FeatureValueTableItemDelegate::sizeHint) returning parent value %d,%d\n", qsz.width(), qsz.height());
    }

    return qsz;
}


void FeatureValueTableItemDelegate::updateEditorGeometry(
        QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
