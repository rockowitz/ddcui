cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ddcui)

# set (coverity_cflags --type_traits_helpers)
# set (coverity_cflags --no_predefined_feature_test_macros)

message(STATUS "Significant environment variables:")
message(STATUS "  INCLUDE:           $ENV{INCLUDE}")
message(STATUS "  LD_LIBRARY_PATH:   $ENV{LD_LIBARARY_PATH}")
message(STATUS "  PKG_CONFIG_PATH:   $ENV{PKG_CONFIG_PATH} ")
message(STATUS "  CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "  CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "  DESTDIR:           $ENV{DESTDIR}" )
message(STATUS "  CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}" )
# message("  NONESUCH:"        $ENV{NONESUCH})
message(STATUS "")


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT5_LIBRARIES Qt5::Widgets )

# was: set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/build_cmake)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build_cmake)

set(EXECUTABLE_OUTPUT_PATH, ${CMAKE_BINARY_DIR})
# set(LIBRARY_OUTPUT_PATH,    ${CMAKE_BINARY_DIR})

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
message( STATUS "Qt5Widgets_INCLUDE_DIRS:    ${Qt5Widgets_INCLUDE_DIRS}" )
message( STATUS "Qt5Core_INCLUDE_DIRS:       ${Qt5Core_INCLUDE_DIRS}" )
message( STATUS "Qt5Gui_INCLUDE_DIRS:        ${Qt5Gui_INCLUDE_DIRS}" )
message( STATUS "Qt5Help_INCLUDE_DIRS:       ${Qt5Help_INCLUDE_DIRS}" )

# find_package(glib-2.0)

find_package (PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)

pkg_check_modules(DDCUTIL REQUIRED ddcutil>=0.9.9)
message("Set by pkg-config:" )
message( STATUS "GLIB2_INCLUDE_DIRS:    ${GLIB2_INCLUDE_DIRS}" )
message( STATUS "GLIB2_LIBRARIES:       ${GLIB2_LIBRARIES}" )
message( STATUS "DDCUTIL_INCLUDE_DIRS:  ${DDCUTIL_INCLUDE_DIRS}" )
message( STATUS "DDCUTIL_LIBRARY_DIRS:  ${DDCUTIL_LIBRARY_DIRS}" )
message( STATUS "DDCUTIL_LIBRARIES:     ${DDCUTIL_LIBRARIES}" )

# hack for testing
# set(DDCUTIL_LIBRARY_DIRS /usr/lib/x86_64-linux-gnu/)
# message( STATUS "DDCUTIL_LIBRARY_DIRS:  ${DDCUTIL_LIBRARY_DIRS}" )

# find_library(DDCUTIL_LIBRARY 
#     NAMES libddcutil ddcutil libddcutil.so libddcutil.so.0
#     PATHS /usr/lib /usr/lib/x86_64-linux-gnu)
# message( STATUS "DDCUTIL_LIBRARY:  ${DDCUTIL_LIBRARY}" )


include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Help_INCLUDE_DIRS} 
                           ${GLIB2_INCLUDE_DIRS} ${DDCUTIL_INCLUDE_DIRS} )

# both work:
# OPTION(APPLY_CANCEL "Use Accept/Cancel buttons" OFF)
set(APPLY_CANCEL OFF)
# set(COVERITY_DUMMIES OFF)


configure_file(cconfig.h.in config.h)


include_directories(src)

set(SOURCES
    src/c_util/data_structures.c
    src/c_util/string_util.c

    src/base/core.cpp 
    src/base/ddca_utils.cpp 
    src/base/ddcui_parms.cpp
    src/base/feature_list.c
    src/base/feature_selector.cpp 
    src/base/global_state.cpp 
    src/base/other_options_state.cpp
    src/base/user_interface_options_state.cpp
    src/base/widget_debug.cpp 

    src/core_widgets/enhanced_slider.cpp
    src/core_widgets/hex_number_validator.cpp
    src/core_widgets/hline_widget.cpp 
    src/core_widgets/number_entry_widget.cpp

    src/cmdline/parsed_cmd.c
    src/cmdline/cmd_parser_aux.c
    src/cmdline/cmd_parser.c

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
    src/feature_value_widgets/value_cnc_widget_x14.cpp
    src/feature_value_widgets/value_ncplus_widget.cpp
    src/feature_value_widgets/value_reset_widget.cpp 
    src/feature_value_widgets/value_2button_widget.cpp 
    src/feature_value_widgets/value_stacked_widget.cpp 
    src/feature_value_widgets/value_std_widget.cpp 
    src/feature_value_widgets/value_bytes_widget.cpp

    src/feature_scrollarea/features_scrollarea_ui.cpp 
    src/feature_scrollarea/feature_widget.cpp 
    src/feature_scrollarea/feature_widget_header.cpp 
    src/feature_scrollarea/features_scrollarea_contents.cpp 
    src/feature_scrollarea/features_scrollarea_view.cpp
    src/feature_scrollarea/features_scrollarea.cpp 

    src/help/help_dialog.cpp
    ddcui.qrc

    src/option_dialogs/feature_selection_dialog.cpp 
    src/option_dialogs/other_options_dialog.cpp
    src/option_dialogs/user_interface_options_dialog.cpp

    src/action_dialogs/debug_actions_dialog.cpp

    src/main/monitor.cpp 
    src/main/mainwindow_ui.cpp
    src/main/mainwindow.cpp 
    src/main/main.cpp
    src/main/msgbox_thread.cpp
)


# file(GLOB dummy_SOURCES src/dummies/*.cpp)
# if (COVERITY_DUMMIES) 
#    set (SOURCES ${SOURCES} ${dummy_SOURCES} )
# endif()

set(UI_SOURCES
    src/option_dialogs/feature_selection_dialog.ui
    src/option_dialogs/other_options_dialog.ui
    src/option_dialogs/user_interface_options_dialog.ui
    src/action_dialogs/debug_actions_dialog.ui
   )

# must come before add_executable(): 
link_directories(${DDCUTIL_LIBRARY_DIRS})

# qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})
# add_executable(ddcui ${SOURCES} ${UI_GENERATED_HEADERS})
add_executable(ddcui ${SOURCES})

target_compile_options(ddcui PUBLIC "-fPIC"  ${coverity_cflags})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

get_property(dirs TARGET ddcui PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
#   message(STATUS "dir='${dir}'")
# endforeach()

message( STATUS "CMAKE_CURRENT_BINARY_DIR:     ${CMAKE_CURRENT_BINARY_DIR}" )
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/ddcui_autogen)
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/ddcui_autogen/include)

get_property(dirs TARGET ddcui PROPERTY INCLUDE_DIRECTORIES)

message( STATUS "INCLUDE_DIRECTORIES:     ${dirs}" )
foreach(d ${dirs})
  message(STATUS "${d}")
endforeach()
message( STATUS "CMAKE_AUTOUIC:           ${CMAKE_AUTOUIC}")
message( STATUS "AUTOGEN_BUILD_DIR:       ${AUTOGEN_BUILD_DIR}")



# set_property(TARGET ddcui PROPERTY AUTOUIC)

# Use the Widgets module from Qt 5.
target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES})
# target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES} ${DDCUTIL_LIBRARY})

install(TARGETS ddcui DESTINATION bin)
install(FILES man/ddcui.1 DESTINATION share/man/man1)
install(FILES AUTHORS ChangeLog COPYING NEWS.md README.md DESTINATION share/doc/ddcui)


# per https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
