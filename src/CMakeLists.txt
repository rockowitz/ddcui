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

if (COVERITY_DUMMIES) 
   set (SOURCES ${SOURCES} 

    dummies/dummy1.cpp
    dummies/dummy2.cpp
    dummies/dummy3.cpp
    dummies/dummy4.cpp
    dummies/dummy5.cpp
    dummies/dummy6.cpp
    dummies/dummy7.cpp
    dummies/dummy8.cpp
    dummies/dummy9.cpp
    dummies/dummy10.cpp
    dummies/dummy11.cpp
    dummies/dummy12.cpp
    dummies/dummy13.cpp
    dummies/dummy14.cpp
    dummies/dummy15.cpp
    dummies/dummy16.cpp
    dummies/dummy17.cpp
    dummies/dummy18.cpp
    dummies/dummy19.cpp
    dummies/dummy20.cpp
    dummies/dummy21.cpp
    dummies/dummy22.cpp
    dummies/dummy23.cpp
    dummies/dummy24.cpp
    dummies/dummy25.cpp
    dummies/dummy26.cpp
    dummies/dummy27.cpp
    dummies/dummy28.cpp
    dummies/dummy29.cpp
    dummies/dummy30.cpp
    dummies/dummy31.cpp
    dummies/dummy32.cpp
    dummies/dummy33.cpp
    dummies/dummy34.cpp
    dummies/dummy35.cpp
    dummies/dummy36.cpp
    dummies/dummy37.cpp
    dummies/dummy38.cpp
    dummies/dummy39.cpp
    dummies/dummy40.cpp
    dummies/dummy41.cpp
    dummies/dummy42.cpp
    dummies/dummy43.cpp
    dummies/dummy44.cpp
    dummies/dummy45.cpp
    dummies/dummy46.cpp
    dummies/dummy47.cpp
    dummies/dummy48.cpp
    dummies/dummy49.cpp

    dummies/dummy50.cpp
    dummies/dummy51.cpp
    dummies/dummy52.cpp
    dummies/dummy53.cpp
    dummies/dummy54.cpp
    dummies/dummy55.cpp
    dummies/dummy56.cpp
    dummies/dummy57.cpp
    dummies/dummy58.cpp
    dummies/dummy59.cpp

    
    dummies/dummy60.cpp
    dummies/dummy61.cpp
    dummies/dummy62.cpp
    dummies/dummy63.cpp
    dummies/dummy64.cpp
    dummies/dummy65.cpp
    dummies/dummy66.cpp
    dummies/dummy67.cpp
    dummies/dummy68.cpp
    dummies/dummy69.cpp

    
    dummies/dummy70.cpp
    dummies/dummy71.cpp
    dummies/dummy72.cpp
    dummies/dummy73.cpp
    dummies/dummy74.cpp
    dummies/dummy75.cpp
    dummies/dummy76.cpp
    dummies/dummy77.cpp
    dummies/dummy78.cpp
    dummies/dummy79.cpp

    dummies/dummy80.cpp
    dummies/dummy81.cpp
    dummies/dummy82.cpp
    dummies/dummy83.cpp
    dummies/dummy84.cpp
    dummies/dummy85.cpp
    dummies/dummy86.cpp
    dummies/dummy87.cpp
    dummies/dummy88.cpp
    dummies/dummy89.cpp
    
    dummies/dummy90.cpp
    dummies/dummy91.cpp
    dummies/dummy92.cpp
    dummies/dummy93.cpp
    dummies/dummy94.cpp
    dummies/dummy95.cpp
    dummies/dummy96.cpp
    dummies/dummy97.cpp
    dummies/dummy98.cpp
    dummies/dummy99.cpp

    dummies/dummy100.cpp
    dummies/dummy101.cpp
    dummies/dummy102.cpp
    dummies/dummy103.cpp
    dummies/dummy104.cpp
    dummies/dummy105.cpp
    dummies/dummy106.cpp
    dummies/dummy107.cpp
    dummies/dummy108.cpp
    dummies/dummy109.cpp


    dummies/dummy110.cpp
    dummies/dummy111.cpp
    dummies/dummy112.cpp
    dummies/dummy113.cpp
    dummies/dummy114.cpp
    dummies/dummy115.cpp
    dummies/dummy116.cpp
    dummies/dummy117.cpp
    dummies/dummy118.cpp
    dummies/dummy119.cpp

    dummies/dummy120.cpp
    dummies/dummy121.cpp
    dummies/dummy122.cpp
    dummies/dummy123.cpp
    dummies/dummy124.cpp
    dummies/dummy125.cpp
    dummies/dummy126.cpp
    dummies/dummy127.cpp
    dummies/dummy128.cpp
    dummies/dummy129.cpp

    dummies/dummy130.cpp
    dummies/dummy131.cpp
    dummies/dummy132.cpp
    dummies/dummy133.cpp
    dummies/dummy134.cpp
    dummies/dummy135.cpp
    dummies/dummy136.cpp
    dummies/dummy137.cpp
    dummies/dummy138.cpp
    dummies/dummy139.cpp

    dummies/glib_util.c
   )
endif()

set(UI_SOURCES
    option_dialogs/feature_selection_dialog.ui
    option_dialogs/other_options_dialog.ui
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
