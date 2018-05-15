cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ddcui)

set(QT5_LIBRARIES Qt5::Widgets )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/build_cmake)

set(EXECUTABLE_OUTPUT_PATH, ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH,    ${CMAKE_BINARY_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

# find_package(glib-2.0)

find_package (PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(DDCUTIL REQUIRED ddcutil)

include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS})

# both work:
OPTION(APPLY_CANCEL "Use Accept/Cancel buttons" OFF)
set(ALT_FEATURES ON)
configure_file(cconfig.h.in config.h)

set(SOURCES

    imported/QtWaitingSpinner/waitingspinnerwidget.cpp

    base/ddca_utils.cpp 
    base/ddcui_globals.cpp 
    base/debug_utils.cpp 
    base/feature_selector.cpp 
    base/global_state.cpp 
    base/hline_widget.cpp 
    base/other_options_state.cpp 

    nongui/ddc_error.cpp 
    nongui/feature_base_model.cpp 
    nongui/feature_value.cpp 
    nongui/vcprequest.cpp 
    nongui/vcpthread.cpp 

    monitor_desc/monitor_desc_ui.cpp 
    monitor_desc/monitor_desc_actions.cpp

    feature_value_widgets/value_abstract_widget.cpp 
    feature_value_widgets/value_base_widget.cpp 
    feature_value_widgets/value_cont_widget.cpp 
    feature_value_widgets/value_nc_widget.cpp 
    feature_value_widgets/value_reset_widget.cpp 
    feature_value_widgets/value_2button_widget.cpp 
    feature_value_widgets/value_stacked_widget.cpp 
    feature_value_widgets/value_std_widget.cpp 

    feature_scrollarea/features_scrollarea_ui.cpp 
    feature_scrollarea/feature_widget.cpp 
    feature_scrollarea/feature_widget_header.cpp 
    feature_scrollarea/features_scrollarea_contents.cpp 
    feature_scrollarea/features_scrollarea_view.cpp

    option_dialogs/feature_selection_dialog.cpp 
    option_dialogs/other_options_dialog.cpp

    main/monitor.cpp 
    main/mainwindow.cpp 
    main/main.cpp
)

if (ALT_FEATURES) 
   set (SOURCES ${SOURCES} 
       alt/table_model_view/table_model_view_ui.cpp 
       alt/table_model_view/feature_table_model.cpp 
       alt/table_model_view/feature_value_tableitem_abstract_editor.cpp 
       alt/table_model_view/feature_value_tableitem_cb_editor.cpp 

       alt/table_model_view/feature_value_tableitem_cont_editor.cpp 
       alt/table_model_view/feature_value_tableitem_delegate.cpp

       alt/table_widget/table_widget_ui.cpp

       alt/list_model_view/list_model_view_ui.cpp 
       alt/list_model_view/feature_item_model.cpp

       alt/list_widget/list_widget_ui.cpp 
       alt/list_widget/feature_list_widget.cpp 
       alt/list_widget/feature_widget_extended.cpp
   )
endif()

set(UI_SOURCES
    option_dialogs/feature_selection_dialog.ui
    option_dialogs/other_options_dialog.ui
   )

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_executable(ddcui ${SOURCES} ${UI_GENERATED_HEADERS})

target_compile_options(ddcui PUBLIC "-fPIC")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

set_property(TARGET ddcui PROPERTY AUTOUIC)

# Use the Widgets module from Qt 5.
target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES})



install(TARGETS ddcui DESTINATION bin)