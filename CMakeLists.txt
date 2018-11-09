cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ddcui)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT5_LIBRARIES Qt5::Widgets )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/build_cmake)

set(EXECUTABLE_OUTPUT_PATH, ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH,    ${CMAKE_BINARY_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ADDITIONAL_MAKE_CLEAN_FILES(  ... )

# Find the QtWidgets library
# find_package(Qt5Widgets)
# ensures all Qt componenets have same version: 
find_package(Qt5 COMPONENTS Core Widgets Gui Help REQUIRED)

# find_package(glib-2.0)

find_package (PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(DDCUTIL REQUIRED ddcutil)
message( STATUS "DDCUTIL_INCLUDE_DIRS:  ${DDCUTIL_INCLUDE_DIRS}" )
message( STATUS "DDCUTIL_LIBRARY_DIRS:  ${DDCUTIL_LIBRARY_DIRS}" )
message( STATUS "DDCUTIL_LIBRARIES:  ${DDCUTIL_LIBRARIES}" )

include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS} ${DDCUTIL_INCLUDE_DIRS} )

# both work:
OPTION(APPLY_CANCEL "Use Accept/Cancel buttons" OFF)
set(ALT_FEATURES OFF)
# set(COVERITY_DUMMIES OFF)
configure_file(cconfig.h.in config.h)

# -j is a parm passed to make, not a compiler option
# set( CMAKE_CXX_FLAGS "${$CMAKE_CXX_FLAGS} -j 5")
# add_compile_options("-j 5")


include_directories(src)

set(SOURCES
    src/imported/QtWaitingSpinner/waitingspinnerwidget.cpp

    src/base/ddca_utils.cpp 
    src/base/ddcui_globals.cpp 
    src/base/debug_utils.cpp 
    src/base/feature_selector.cpp 
    src/base/global_state.cpp 
    src/base/hline_widget.cpp 
    src/base/other_options_state.cpp 

    src/nongui/ddc_error.cpp 
    src/nongui/feature_base_model.cpp 
    src/nongui/feature_value.cpp 
    src/nongui/msgbox_queue.cpp
    src/nongui/vcprequest.cpp 
    src/nongui/vcpthread.cpp 

    src/monitor_desc/monitor_desc_ui.cpp 
    src/monitor_desc/monitor_desc_actions.cpp

    src/feature_value_widgets/value_base_widget.cpp 
    src/feature_value_widgets/value_cont_widget.cpp 
    src/feature_value_widgets/value_nc_widget.cpp 
    src/feature_value_widgets/value_reset_widget.cpp 
    src/feature_value_widgets/value_2button_widget.cpp 
    src/feature_value_widgets/value_stacked_widget.cpp 
    src/feature_value_widgets/value_std_widget.cpp 

    src/feature_scrollarea/features_scrollarea_ui.cpp 
    src/feature_scrollarea/feature_widget.cpp 
    src/feature_scrollarea/feature_widget_header.cpp 
    src/feature_scrollarea/features_scrollarea_contents.cpp 
    src/feature_scrollarea/features_scrollarea_view.cpp

    src/help/help_dialog.cpp
    ddcui.qrc

    src/option_dialogs/feature_selection_dialog.cpp 
    src/option_dialogs/other_options_dialog.cpp

    src/main/monitor.cpp 
    src/main/mainwindow_ui.cpp
    src/main/mainwindow.cpp 
    src/main/main.cpp
    src/main/msgbox_thread.cpp
)

if (ALT_FEATURES) 
   set (SOURCES ${SOURCES} 
       src/alt/table_model_view/table_model_view_ui.cpp 
       src/alt/table_model_view/feature_table_model.cpp 
       src/alt/table_model_view/feature_value_tableitem_abstract_editor.cpp 
       src/alt/table_model_view/feature_value_tableitem_cb_editor.cpp 

       src/alt/table_model_view/feature_value_tableitem_cont_editor.cpp 
       src/alt/table_model_view/feature_value_tableitem_delegate.cpp

       src/alt/table_widget/table_widget_ui.cpp

       src/alt/list_model_view/list_model_view_ui.cpp 
       src/alt/list_model_view/feature_item_model.cpp

       src/alt/list_widget/list_widget_ui.cpp 
       src/alt/list_widget/feature_list_widget.cpp 
       src/alt/list_widget/feature_widget_extended.cpp
   )
endif()

# file(GLOB dummy_SOURCES src/dummies/*.cpp)

set(UI_SOURCES
    src/option_dialogs/feature_selection_dialog.ui
    src/option_dialogs/other_options_dialog.ui
   )

# qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

link_directories(${DDCUTIL_LIBRARY_DIRS})

# add_executable(ddcui ${SOURCES} ${UI_GENERATED_HEADERS})
add_executable(ddcui ${SOURCES})

target_compile_options(ddcui PUBLIC "-fPIC")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# set_property(TARGET ddcui PROPERTY AUTOUIC)

# Use the Widgets module from Qt 5.
target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES})

install(TARGETS ddcui DESTINATION bin)
