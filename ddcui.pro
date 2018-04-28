#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T08:57:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ddcui
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-misleading-indentation -Wno-unused-parameter

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
    imported/QtWaitingSpinner/waitingspinnerwidget.cpp

SOURCES += \
    base/ddca_utils.cpp \
    base/ddcui_globals.cpp \
    base/debug_utils.cpp \
    base/feature_selector.cpp \
    base/global_state.cpp \
    base/other_options_state.cpp \
    base/vertical_scroll_area.cpp \

SOURCES += \
    nongui/ddc_error.cpp \
    nongui/feature_base_model.cpp \
    nongui/feature_value.cpp \
    nongui/vcprequest.cpp \
    nongui/vcpthread.cpp \
    nongui/simple_feature_value_observer.cpp \
    nongui/simple_feature_value_subject.cpp \
    nongui/simple_feature_value.cpp

#     nongui/feature_change_observer.cpp \

SOURCES += \
    monitor_desc/monitor_desc_ui.cpp \
    monitor_desc/monitor_desc_actions.cpp

SOURCES += \
    feature_value_widgets/value_abstract_widget.cpp \
    feature_value_widgets/value_base_widget.cpp \
    feature_value_widgets/value_cont_widget.cpp \
    feature_value_widgets/value_nc_widget.cpp \
    feature_value_widgets/value_reset_widget.cpp \
    feature_value_widgets/value_2button_widget.cpp \
    feature_value_widgets/value_stacked_widget.cpp \
    feature_value_widgets/value_std_widget.cpp 

SOURCES += \
    alt/table_model_view/table_model_view_ui.cpp \
    alt/table_model_view/feature_table_model.cpp \
    alt/table_model_view/feature_value_tableitem_abstract_editor.cpp \
    alt/table_model_view/feature_value_tableitem_cb_editor.cpp \
    alt/table_model_view/feature_value_tableitem_cont_editor.cpp \
    alt/table_model_view/feature_value_tableitem_delegate.cpp

SOURCES += \
    alt/table_widget/table_widget_ui.cpp

SOURCES += \
    feature_scrollarea/features_scrollarea_ui.cpp \
    feature_scrollarea/feature_widget.cpp \
    feature_scrollarea/features_scrollarea_contents.cpp \
    feature_scrollarea/page_change_observer.cpp \
    feature_scrollarea/features_scrollarea_view.cpp

SOURCES += \
    alt/list_model_view/list_model_view_ui.cpp \
    alt/list_model_view/feature_item_model.cpp

SOURCES += \
    alt/list_widget/list_widget_ui.cpp \
    alt/list_widget/feature_list_widget.cpp \
    alt/list_widget/feature_widget_extended.cpp

SOURCES += \
    option_dialogs/feature_selection_dialog.cpp \
    option_dialogs/other_options_dialog.cpp

SOURCES += \
    main/monitor.cpp \
    main/mainwindow.cpp \
    main/main.cpp



#   vcplineitem.cpp \

# SOURCES += \
#     c_feature_widget.cpp

# Each group of header files depends only on the groups above it.

HEADERS += \
    imported/QtWaitingSpinner/waitingspinnerwidget.h

HEADERS += \
    base/ddca_utils.h \
    base/ddcui_globals.h \
    base/debug_utils.h \
    base/feature_selector.h \
    base/global_state.h \
    base/monitor.h \
    base/other_options_state.h \
    base/vertical_scroll_area.h

HEADERS += \
    nongui/ddc_error.h \
    nongui/feature_base_model.h \
    nongui/feature_change_observer.h \
    nongui/feature_value.h \
    nongui/vcprequest.h \
    nongui/vcpthread.h \
    nongui/simple_feature_value_observer.h \
    nongui/simple_feature_value_subject.h \
    nongui/simple_feature_value.h

HEADERS += \
    monitor_desc/monitor_desc_ui.h \
    monitor_desc/monitor_desc_actions.h

HEADERS += \
    feature_value_widgets/value_abstract_widget.h \
    feature_value_widgets/value_base_widget.h \
    feature_value_widgets/value_std_widget.h \
    feature_value_widgets/value_cont_widget.h \
    feature_value_widgets/value_nc_widget.h \
    feature_value_widgets/value_reset_widget.h \
    feature_value_widgets/value_2button_widget.h \
    feature_value_widgets/value_stacked_widget.h

HEADERS += \
    alt/table_model_view/table_model_view_ui.h \
    alt/table_model_view/feature_table_model.h \
    alt/table_model_view/feature_value_tableitem_abstract_editor.h \
    alt/table_model_view/feature_value_tableitem_cb_editor.h \
    alt/table_model_view/feature_value_tableitem_cont_editor.h \
    alt/table_model_view/feature_value_tableitem_delegate.h

HEADERS += \
    alt/table_widget/table_widget_ui.h

HEADERS += \
    feature_scrollarea/features_scrollarea_ui.h \
    feature_scrollarea/feature_widget.h \
    feature_scrollarea/features_scrollarea_contents.h \
    feature_scrollarea/page_change_observer.h \
    feature_scrollarea/features_scrollarea_view.h

HEADERS += \
   alt/list_model_view/list_model_view_ui.h \
   alt/list_model_view/feature_item_model.h

HEADERS += \
    alt/list_widget/list_widget_ui.h \
    alt/list_widget/feature_list_widget.h \
    alt/list_widget/feature_widget_extended.h

HEADERS += \
    option_dialogs/feature_selection_dialog.h \
    option_dialogs/other_options_dialog.h

HEADERS += \
    main/mainwindow.h \
    main/ui_mainwindow2.h

#     vcplineitem.h \

# HEADERS += \
#     c_feature_widget.h

#   mainwindow.ui \

FORMS += \
    option_dialogs/feature_selection_dialog.ui \
    option_dialogs/other_options_dialog.ui

# FORMS += \
#     c_feature_widget.ui

# FORMS += \
#     featureitem.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ddcutil
