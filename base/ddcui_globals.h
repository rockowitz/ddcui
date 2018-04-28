#ifndef DDCUI_GLOBALS_H
#define DDCUI_GLOBALS_H

#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc

const int FeatureWidgetType = QListWidgetItem::UserType+1;

const bool debugSignals            = true;
const bool debugValueWidgetSignals = false;
const bool debugFeatureLists       = false;
const bool debugFeatureSelection   = false;
const bool debugLayout             = false;

const bool useApplyCancel    = false;
const bool enableAltFeatures = false;


// #define DDCMSG0(format, ...) ddcmsg(TRACE_GROUP, __func__, __LINE__, __FILE__, format, ##__VA_ARGS__)

#define PRINTFCM(__FMT__, ...) \
   do { \
     printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
     fflush(stdout); \
   } while(0)

#define PRINTFCMF(__FLAG__, __FMT__, ...) \
   if (__FLAG__) { \
      printf("(%s::%s) " __FMT__ "\n", _cls, __func__, ##__VA_ARGS__); \
      fflush(stdout); \
   }

#endif // DDCUI_GLOBALS_H
