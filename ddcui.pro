#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T08:57:02
#
#-------------------------------------------------

# message($$QT_MAJOR_VERSION)
lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5.5 or later")
equals(QT_MAJOR_VERSION,5) {
    # QString.asprintf() introduced in 5.5
    lessThan(QT_MINOR_VERSION,5): error("requires Qt 5.5 or later")
}

CONFIG += c++11

QT += core gui widgets help

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

# CONFIG += apply_cancel

apply_cancel {
   APPLY_CANCEL_FLAG = 1
   message("setting APPLY_CANCEL_FLAG");
}
else {
   message("not setting APPLY_CANCEL_FLAG")
   APPLY_CANCEL_FLAG = 0
}
  

versionconfig.input = qconfig.h.in
versionconfig.output = config.h
QMAKE_SUBSTITUTES += versionconfig

INCLUDEPATH += src


SOURCES += \
    src/imported/QtWaitingSpinner/waitingspinnerwidget.cpp

SOURCES += \
    src/base/ddca_utils.cpp \
    src/base/ddcui_globals.cpp \
    src/base/debug_utils.cpp \
    src/base/feature_selector.cpp \
    src/base/global_state.cpp \
    src/base/hline_widget.cpp \
    src/base/other_options_state.cpp 
    
#   src/base/vertical_scroll_area.cpp \

SOURCES += \
    src/cmdline/parsed_cmd.c \
    src/cmdline/cmd_parser_aux.c \
    src/cmdline/cmd_parser.c

SOURCES += \
    src/nongui/ddc_error.cpp \
    src/nongui/feature_base_model.cpp \
    src/nongui/feature_value.cpp \
    src/nongui/msgbox_queue.cpp \
    src/nongui/vcprequest.cpp \
    src/nongui/vcpthread.cpp 

#   src/nongui/simple_feature_value_observer.cpp \
#   src/nongui/simple_feature_value_subject.cpp \
#   src/nongui/simple_feature_value.cpp

#     src/nongui/feature_change_observer.cpp \

SOURCES += \
    src/monitor_desc/monitor_desc_ui.cpp \
    src/monitor_desc/monitor_desc_actions.cpp

SOURCES += \
    src/feature_value_widgets/value_base_widget.cpp \
    src/feature_value_widgets/value_cont_widget.cpp \
    src/feature_value_widgets/value_nc_widget.cpp \
    src/feature_value_widgets/value_reset_widget.cpp \
    src/feature_value_widgets/value_2button_widget.cpp \
    src/feature_value_widgets/value_stacked_widget.cpp \
    src/feature_value_widgets/value_std_widget.cpp 


SOURCES += \
    src/feature_scrollarea/features_scrollarea_ui.cpp \
    src/feature_scrollarea/feature_widget.cpp \
    src/feature_scrollarea/feature_widget_header.cpp \
    src/feature_scrollarea/features_scrollarea_contents.cpp \
    src/feature_scrollarea/features_scrollarea_view.cpp

SOURCES += \
    src/help/help_dialog.cpp

 #  src/help/helpbrowser.cpp \
 #  src/help/help_dialog0.cpp \

SOURCES += \
    src/option_dialogs/feature_selection_dialog.cpp \
    src/option_dialogs/other_options_dialog.cpp

SOURCES += \
    src/main/monitor.cpp \
    src/main/mainwindow_ui.cpp \
    src/main/mainwindow.cpp \
    src/main/msgbox_thread.cpp \
    src/main/main.cpp

#   vcplineitem.cpp \

# SOURCES += \
#     c_feature_widget.cpp

# Each group of header files depends only on the groups above it.

HEADERS += \
    src/imported/QtWaitingSpinner/waitingspinnerwidget.h

HEADERS += \
    src/base/ddcui_parms.h \
    src/base/ddca_utils.h \
    src/base/ddcui_globals.h \
    src/base/debug_utils.h \
    src/base/feature_selector.h \
    src/base/global_state.h \
    src/base/hline_widget.h \
    src/base/monitor.h \
    src/base/other_options_state.h
    
#   src/base/vertical_scroll_area.h

HEADERS += \
    src/cmdline/parsed_cmd.h \
    src/cmdline/cmd_parser_aux.h \
    src/cmdline/cmd_parser.h


HEADERS += \
    src/nongui/ddc_error.h \
    src/nongui/feature_base_model.h \
    src/nongui/feature_value.h \
    src/nongui/msgbox_queue.h \
    src/nongui/vcprequest.h \
    src/nongui/vcpthread.h 

#   src/nongui/feature_change_observer.h \
#   src/nongui/simple_feature_value_observer.h \
#   src/nongui/simple_feature_value_subject.h \
#   src/nongui/simple_feature_value.h

HEADERS += \
    src/monitor_desc/monitor_desc_ui.h \
    src/monitor_desc/monitor_desc_actions.h

HEADERS += \
    src/feature_value_widgets/value_base_widget.h \
    src/feature_value_widgets/value_std_widget.h \
    src/feature_value_widgets/value_cont_widget.h \
    src/feature_value_widgets/value_nc_widget.h \
    src/feature_value_widgets/value_reset_widget.h \
    src/feature_value_widgets/value_2button_widget.h \
    src/feature_value_widgets/value_stacked_widget.h


HEADERS += \
    src/feature_scrollarea/features_scrollarea_ui.h \
    src/feature_scrollarea/feature_widget.h \
    src/feature_scrollarea/feature_widget_header.h \
    src/feature_scrollarea/features_scrollarea_contents.h \
    src/feature_scrollarea/features_scrollarea_view.h

HEADERS += \
    src/help/help_dialog.h

 #  src/help/helpbrowser.h \
 #  src/help/help_dialog0.h \

HEADERS += \
    src/option_dialogs/feature_selection_dialog.h \
    src/option_dialogs/other_options_dialog.h

HEADERS += \
    src/main/msgbox_thread.h \
    src/main/mainwindow.h \
    src/main/mainwindow_ui.h

#     vcplineitem.h \

# HEADERS += \
#     c_feature_widget.h

#   mainwindow.ui \

FORMS += \
    src/option_dialogs/feature_selection_dialog.ui \
    src/option_dialogs/other_options_dialog.ui 

    # src/help/help_dialog0.ui

# FORMS += \
#     c_feature_widget.ui

# FORMS += \
#     featureitem.ui

RESOURCES = ddcui.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ddcutil
