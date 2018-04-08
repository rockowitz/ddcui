#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QScrollArea>
#include <stdio.h>

#include "features_scrollarea_view.h"
#include "features_scroll_area_contents.h"
#include "monitor.h"


FeaturesScrollAreaView::FeaturesScrollAreaView(
        Monitor *          monitor,
        FeatureBaseModel * model,
        QStackedWidget * centralStackedWidget,
        QObject *parent) :
    QObject(parent)
{
    _monitor = monitor;
    _centralStackedWidget = centralStackedWidget;
    _baseModel = model;

}


static QSizePolicy pageSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(1);
    policy.setHeightForWidth(false);
    return policy;
}



void FeaturesScrollAreaView::onEndInitialLoad(void) {
   printf("(FeatuesScrollAreaView::%s) Starting\n", __func__);  fflush(stdout);

    // QStackedWidget *   _centralStackedWidget;
    // FeatureBaseModel * _baseModel;

    QScrollArea * scrollArea = new QScrollArea();
    FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
    // no effect:
    // scrollAreaContents->setSizePolicy(pageSizePolicy());
    // scrollAreaContents->setMinimumSize(QSize(700,0));

    scrollAreaContents->setObjectName("scrollAreaContents local to onEndInitialLoad");
    QVBoxLayout * vLayout  = new QVBoxLayout();
    vLayout->setObjectName("vLayout in onEndInitLoad");

    int ct = 0;
    for (int feature_code = 0; feature_code < 256; feature_code++) {
         FeatureValue * fv =  _baseModel->modelVcpValueFind(feature_code);
         if (fv) {
             FeatureWidgetBasic * w = new FeatureWidgetBasic();
             w->setFeatureValue(*fv);
             vLayout->addWidget(w);
             ct++;
         }
    }

    // printf("Calling scrollAreaContents->setLayout()\n"); fflush(stdout);
    scrollAreaContents->setLayout(vLayout);
    // printf("Calling scrollArea->setWidget()\n"); fflush(stdout);
    scrollArea->setWidget(scrollAreaContents);

    // printf("Calling _centralStackedWidget->addWidget()\n"); fflush(stdout);
    int pageno = _centralStackedWidget->addWidget(scrollArea);
    // _centralStackedWidget->hide();    // no effect
    // _centralStackedWidget->setCurrentIndex(pageno);
    _centralStackedWidget->setCurrentWidget(scrollArea);
    _centralStackedWidget->show();
    printf("(FeatuesScrollAreaView::%s) Done.  feature count: %d\n", __func__, ct);  fflush(stdout);
}
