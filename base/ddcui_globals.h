#ifndef DDCUI_GLOBALS_H
#define DDCUI_GLOBALS_H

#include <QtCore/QtCore>
#include <QtWidgets/QListWidgetItem>

const int FeatureValueRole  = Qt::UserRole+1;  // DisplayRole, EditorRole, .. etc

const int FeatureWidgetType = QListWidgetItem::UserType+1;

const bool debugLayout = false;
const bool useApplyCancel = false;
const bool enableAltFeatures = false;

#endif // DDCUI_GLOBALS_H
