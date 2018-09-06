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
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ADDITIONAL_MAKE_CLEAN_FILES(  ... )

# Find the QtWidgets library
find_package(Qt5Widgets)

# find_package(glib-2.0)

find_package (PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(DDCUTIL REQUIRED ddcutil)

include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS})

# both work:
OPTION(APPLY_CANCEL "Use Accept/Cancel buttons" OFF)
set(ALT_FEATURES OFF)
set(COVERITY_DUMMIES OFF)
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

file(GLOB dummy_SOURCES src/dummies/*.cpp)

if (COVERITY_DUMMIES) 
   set (SOURCES ${SOURCES} ${dummy_SOURCES}

    # src/dummies/dummy1.cpp
    # src/dummies/dummy2.cpp
    # src/dummies/dummy3.cpp
    # src/dummies/dummy4.cpp
    # src/dummies/dummy5.cpp
    # src/dummies/dummy6.cpp
    # src/dummies/dummy7.cpp
    # src/dummies/dummy8.cpp
    # src/dummies/dummy9.cpp
    # src/dummies/dummy10.cpp
    # src/dummies/dummy11.cpp
    # src/dummies/dummy12.cpp
    # src/dummies/dummy13.cpp
    # src/dummies/dummy14.cpp
    # src/dummies/dummy15.cpp
    # src/dummies/dummy16.cpp
    # src/dummies/dummy17.cpp
    # src/dummies/dummy18.cpp
    # src/dummies/dummy19.cpp
    # src/dummies/dummy20.cpp
    # src/dummies/dummy21.cpp
    # src/dummies/dummy22.cpp
    # src/dummies/dummy23.cpp
    # src/dummies/dummy24.cpp
    # src/dummies/dummy25.cpp
    # src/dummies/dummy26.cpp
    # src/dummies/dummy27.cpp
    # src/dummies/dummy28.cpp
    # src/dummies/dummy29.cpp
    # src/dummies/dummy30.cpp
    # src/dummies/dummy31.cpp
    # src/dummies/dummy32.cpp
    # src/dummies/dummy33.cpp
    # src/dummies/dummy34.cpp
    # src/dummies/dummy35.cpp
    # src/dummies/dummy36.cpp
    # src/dummies/dummy37.cpp
    # src/dummies/dummy38.cpp
    # src/dummies/dummy39.cpp
    # src/dummies/dummy40.cpp
    # src/dummies/dummy41.cpp
    # src/dummies/dummy42.cpp
    # src/dummies/dummy43.cpp
    # src/dummies/dummy44.cpp
    # src/dummies/dummy45.cpp
    # src/dummies/dummy46.cpp
    # src/dummies/dummy47.cpp
    # src/dummies/dummy48.cpp
    # src/dummies/dummy49.cpp

    # src/dummies/dummy50.cpp
    # src/dummies/dummy51.cpp
    # src/dummies/dummy52.cpp
    # src/dummies/dummy53.cpp
    # src/dummies/dummy54.cpp
    # src/dummies/dummy55.cpp
    # src/dummies/dummy56.cpp
    # src/dummies/dummy57.cpp
    # src/dummies/dummy58.cpp
    # src/dummies/dummy59.cpp

    
    # src/dummies/dummy60.cpp
    # src/dummies/dummy61.cpp
    # src/dummies/dummy62.cpp
    # src/dummies/dummy63.cpp
    # src/dummies/dummy64.cpp
    # src/dummies/dummy65.cpp
    # src/dummies/dummy66.cpp
    # src/dummies/dummy67.cpp
    # src/dummies/dummy68.cpp
    # src/dummies/dummy69.cpp

    
    # src/dummies/dummy70.cpp
    # src/dummies/dummy71.cpp
    # src/dummies/dummy72.cpp
    # src/dummies/dummy73.cpp
    # src/dummies/dummy74.cpp
    # src/dummies/dummy75.cpp
    # src/dummies/dummy76.cpp
    # src/dummies/dummy77.cpp
    # src/dummies/dummy78.cpp
    # src/dummies/dummy79.cpp

    # src/dummies/dummy80.cpp
    # src/dummies/dummy81.cpp
    # src/dummies/dummy82.cpp
    # src/dummies/dummy83.cpp
    # src/dummies/dummy84.cpp
    # src/dummies/dummy85.cpp
    # src/dummies/dummy86.cpp
    # src/dummies/dummy87.cpp
    # src/dummies/dummy88.cpp
    # src/dummies/dummy89.cpp
    
    # src/dummies/dummy90.cpp
    # src/dummies/dummy91.cpp
    # src/dummies/dummy92.cpp
    # src/dummies/dummy93.cpp
    # src/dummies/dummy94.cpp
    # src/dummies/dummy95.cpp
    # src/dummies/dummy96.cpp
    # src/dummies/dummy97.cpp
    # src/dummies/dummy98.cpp
    # src/dummies/dummy99.cpp

    # src/dummies/dummy100.cpp
    # src/dummies/dummy101.cpp
    # src/dummies/dummy102.cpp
    # src/dummies/dummy103.cpp
    # src/dummies/dummy104.cpp
    # src/dummies/dummy105.cpp
    # src/dummies/dummy106.cpp
    # src/dummies/dummy107.cpp
    # src/dummies/dummy108.cpp
    # src/dummies/dummy109.cpp


    # src/dummies/dummy110.cpp
    # src/dummies/dummy111.cpp
    # src/dummies/dummy112.cpp
    # src/dummies/dummy113.cpp
    # src/dummies/dummy114.cpp
    # src/dummies/dummy115.cpp
    # src/dummies/dummy116.cpp
    # src/dummies/dummy117.cpp
    # src/dummies/dummy118.cpp
    # src/dummies/dummy119.cpp

    # src/dummies/dummy120.cpp
    # src/dummies/dummy121.cpp
    # src/dummies/dummy122.cpp
    # src/dummies/dummy123.cpp
    # src/dummies/dummy124.cpp
    # src/dummies/dummy125.cpp
    # src/dummies/dummy126.cpp
    # src/dummies/dummy127.cpp
    # src/dummies/dummy128.cpp
    # src/dummies/dummy129.cpp

    # src/dummies/dummy130.cpp
    # src/dummies/dummy131.cpp
    # src/dummies/dummy132.cpp
    # src/dummies/dummy133.cpp
    # src/dummies/dummy134.cpp
    # src/dummies/dummy135.cpp
    # src/dummies/dummy136.cpp
    # src/dummies/dummy137.cpp
    # src/dummies/dummy138.cpp
    # src/dummies/dummy139.cpp

    # src/dummies/glib_util.c
   )
endif()

set(UI_SOURCES
    src/option_dialogs/feature_selection_dialog.ui
    src/option_dialogs/other_options_dialog.ui
   )

# qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

# add_executable(ddcui ${SOURCES} ${UI_GENERATED_HEADERS})
add_executable(ddcui ${SOURCES})

target_compile_options(ddcui PUBLIC "-fPIC")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# set_property(TARGET ddcui PROPERTY AUTOUIC)

# Use the Widgets module from Qt 5.
target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES})



install(TARGETS ddcui DESTINATION bin)
