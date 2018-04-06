#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T08:57:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ddcui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    nongui/feature_base_model.cpp \
    nongui/feature_change_observer.cpp \
    nongui/feature_value.cpp \
    nongui/vcprequest.cpp \
    nongui/vcpthread.cpp

SOURCES += \
    feature_value_widgets/value_abstract_widget.cpp \
    feature_value_widgets/value_base_widget.cpp \
    feature_value_widgets/value_cont_widget.cpp \
    feature_value_widgets/value_nc_widget.cpp \
    feature_value_widgets/value_stacked_widget.cpp \
    feature_value_widgets/value_std_widget.cpp 

SOURCES += \
    table_model_view/feature_table_model.cpp \
    table_model_view/feature_value_tableitem_abstract_editor.cpp \
    table_model_view/feature_value_tableitem_cb_editor.cpp \
    table_model_view/feature_value_tableitem_cont_editor.cpp \
    table_model_view/feature_value_tableitem_delegate.cpp

SOURCES += \
    QtWaitingSpinner/waitingspinnerwidget.cpp \
    feature_item_model.cpp \
    feature_list_widget.cpp \
    feature_selection_dialog.cpp \ 
    feature_widget.cpp \
    feature_widget_basic.cpp \
    main.cpp \
    mainwindow.cpp \
    monitor.cpp \
    vcplineitem.cpp

# SOURCES += \
#     c_feature_widget.cpp

HEADERS += \
    nongui/feature_base_model.h \
    nongui/feature_change_observer.h \
    nongui/feature_value.h \
    nongui/vcprequest.h \
    nongui/vcpthread.h

HEADERS += \
    feature_value_widgets/value_abstract_widget.h \
    feature_value_widgets/value_base_widget.h \
    feature_value_widgets/value_cont_widget.h \
    feature_value_widgets/value_nc_widget.h \
    feature_value_widgets/value_stacked_widget.h \
    feature_value_widgets/value_std_widget.h 

HEADERS += \
    table_model_view/feature_table_model.h \
    table_model_view/feature_value_tableitem_abstract_editor.h \
    table_model_view/feature_value_tableitem_cb_editor.h \
    table_model_view/feature_value_tableitem_cont_editor.h \
    table_model_view/feature_value_tableitem_delegate.h

HEADERS += \
    QtWaitingSpinner/waitingspinnerwidget.h \
    base/ddcui_globals.h \
    feature_item_model.h \
    feature_list_widget.h \
    feature_selection_dialog.h \
    feature_widget.h \
    feature_widget_basic.h \
    mainwindow.h \
    misc.h \
    monitor.h \
    ui_mainwindow2.h \
    vcplineitem.h

# HEADERS += \
#     c_feature_widget.h

#   mainwindow.ui \

FORMS += \
    featureselectiondialog.ui

# FORMS += \
#     c_feature_widget.ui

FORMS += \
    featureitem.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ddcutil
