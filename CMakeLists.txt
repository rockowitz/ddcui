cmake_minimum_required(VERSION 3.15)
# message(VERBOSE) requires cmake 3.15

set(ddcui_qt_version         5.5)
set(ddcui_glib_version       2.40)   # g_option_context_parse_strv() new in 2.40
set(ddcui_libddcutil_version 1.2.0)
set(cmake_build_type_default "RelWithDebInfo")

message(VERBOSE "Required Minimum Versions:")
message(VERBOSE "  ddcui_qt_version:          ${ddcui_qt_version}" )
message(VERBOSE "  ddcui_qlib_version:        ${ddcui_glib_version}" )
message(VERBOSE "  ddcui_libddcutil_version:  ${ddcui_libddcutil_version}" ) 

option(USE_CCACHE "Use ccache" ON)
# message(STATUS "USE_CCACHE: ${USE_CCACHE}" )

set(valid_build_types Release RelWithDebInfo Debug MinSizeRel)
string(TOUPPER "${valid_build_types}" uc_valid_build_types)
string(REPLACE ";" ", " pretty_valid_build_types "${valid_build_types}")
STRING(TOUPPER "${CMAKE_BUILD_TYPE}" UC_CMAKE_BUILD_TYPE )
message(STATUS "Initial CMAKE_BUILD_TYPE:  ${CMAKE_BUILD_TYPE}")

# https://cmake.org/pipermail/cmake/2008-September/023808.html
# If the user specifies -DCMAKE_BUILD_TYPE on the command line, take their
# definition and dump it in the cache along with proper documentation, 
# otherwise set CMAKE_BUILD_TYPE to the default prior to calling project()
set(i0 0)
if (DEFINED CMAKE_BUILD_TYPE)
  string(LENGTH "${CMAKE_BUILD_TYPE}" i0)
endif(DEFINED CMAKE_BUILD_TYPE)
IF( (DEFINED CMAKE_BUILD_TYPE) AND (i0 GREATER 0) ) 
   list(FIND uc_valid_build_types "${UC_CMAKE_BUILD_TYPE}" index) 
   if (index EQUAL -1)
      message(FATAL_ERROR "CMAKE_BUILD_TYPE must be one of ${pretty_valid_build_types}" )
   endif()
   # message(STATUS "Setting CMakeCache.txt value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING 
                   "Choose the type of build, options are: ${pretty_valid_build_types}." FORCE)
 else()
    set(CMAKE_BUILD_TYPE ${cmake_build_type_default})
 endif()
 
# Set the possible values of build type for cmake-gui
# set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "RelWithDebInfo" "MinSizeRel")

# see also: https://stackoverflow.com/questions/48754619/what-are-cmake-build-type-debug-release-relwithdebinfo-and-minsizerel
#           https://blog.feabhas.com/2021/07/cmake-part-2-release-and-debug-builds/

project(ddcui LANGUAGES C CXX)

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE} (CMAKE_BUILD_TYPE)")
# message(STATUS "   CMAKE_CXX_FLAGS:                ${CMAKE_CXX_FLAGS}" )
# message(STATUS "   CMAKE_C_FLAGS:                  ${CMAKE_C_FLAGS}" )
# message(STATUS "   CMAKE_CXX_FLAGS_DEBUG:          ${CMAKE_CXX_FLAGS_DEBUG}" )
# message(STATUS "   CMAKE_C_FLAGS_DEBUG:            ${CMAKE_C_FLAGS_DEBUG}" )
# message(STATUS "   CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )
# message(STATUS "   CMAKE_C_FLAGS_RELWITHDEBINFO:   ${CMAKE_C_FLAGS_RELWITHDEBINFO}" )
# message(STATUS "   CMAKE_CXX_FLAGS_RELEASE:        ${CMAKE_CXX_FLAGS_RELEASE}" )
# message(STATUS "   CMAKE_C_FLAGS_RELEASE:          ${CMAKE_C_FLAGS_RELEASE}" )

# message(VERBOSE "Project variables:")
# message(VERBOSE "  PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}" )
# message(VERBOSE "  PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR}" )

message(STATUS "Source directory: ${CMAKE_SOURCE_DIR} (CMAKE_SOURCE_DIR)" )
message(STATUS "Build directory:  ${CMAKE_BINARY_DIR} (CMAKE_BINARY_DIR)" )

if (  ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} ) 
   message(FATAL_ERROR " Attempting to build in source directory. Use -B option to set the build directory.")
endif()
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# add_compile_definitions(QT_NO_DEBUG)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND AND USE_CCACHE)
     message( STATUS "Using ccache to improve build performance" )
     set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# set (coverity_cflags --type_traits_helpers)
# set (coverity_cflags --no_predefined_feature_test_macros)
# set (COVERITY_DUMMIES OFF)

message(VERBOSE "Significant environment variables:")
message(VERBOSE "  INCLUDE:                  $ENV{INCLUDE}")
message(VERBOSE "  LD_LIBRARY_PATH:          $ENV{LD_LIBARARY_PATH}")
message(VERBOSE "  PKG_CONFIG_PATH:          $ENV{PKG_CONFIG_PATH} ")
message(VERBOSE "cmake paths:")
message(VERBOSE "  CMAKE_MODULE_PATH:        ${CMAKE_MODULE_PATH}")
message(VERBOSE "  CMAKE_PREFIX_PATH:        ${CMAKE_PREFIX_PATH}")
message(VERBOSE "  CMAKE_BINARY_DIR:         ${CMAKE_BINARY_DIR}" )
message(VERBOSE "  CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}" )
message(VERBOSE "  EXCECUTABLE_OUTPUT_PATH:  ${EXECUTABLE_OUTPUT_PATH}" )
# message(STATUS "")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find includes in corresponding build directories, needed for config.h
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
message( VERBOSE "Qt settings:")
message( VERBOSE "  CMAKE_AUTOUIC:           ${CMAKE_AUTOUIC}")
message( VERBOSE "  CMAKE_AUTOMOC:           ${CMAKE_AUTOMOC}")
message( VERBOSE "  CMAKE_AUTORCC:           ${CMAKE_AUTORCC}")
# message( VERBOSE "AUTOGEN_BUILD_DIR:       ${AUTOGEN_BUILD_DIR}") overrides default location

set(QT5_LIBRARIES Qt5::Widgets )
# Ensures all Qt componenets have same version: 
find_package(Qt5 ${ddcui_qt_version} COMPONENTS Core Widgets Gui Help REQUIRED)
message( VERBOSE "Qt directories:")
message( VERBOSE "  Qt5Widgets_INCLUDE_DIRS:  ${Qt5Widgets_INCLUDE_DIRS}" )
message( VERBOSE "  Qt5Core_INCLUDE_DIRS:     ${Qt5Core_INCLUDE_DIRS}"    )
message( VERBOSE "  Qt5Gui_INCLUDE_DIRS:      ${Qt5Gui_INCLUDE_DIRS}"     )
message( VERBOSE "  Qt5Help_INCLUDE_DIRS:     ${Qt5Help_INCLUDE_DIRS}"    )

find_package (PkgConfig REQUIRED )
pkg_check_modules(GLIB2 REQUIRED glib-2.0>=${ddcui_glib_version})
pkg_check_modules(DDCUTIL REQUIRED ddcutil>=${ddcui_libddcutil_version}})
# set(DDCUTIL_LIBRARY_DIRS /usr/lib/x86_64-linux-gnu/)   # hack for testing
message( VERBOSE "Set by pkg-config:" )
message( VERBOSE "  GLIB2_INCLUDE_DIRS:       ${GLIB2_INCLUDE_DIRS}"   )
message( VERBOSE "  GLIB2_LIBRARIES:          ${GLIB2_LIBRARIES}"      )
message( VERBOSE "  DDCUTIL_INCLUDE_DIRS:     ${DDCUTIL_INCLUDE_DIRS}" )
message( VERBOSE "  DDCUTIL_LIBRARY_DIRS:     ${DDCUTIL_LIBRARY_DIRS}" )
message( VERBOSE "  DDCUTIL_LIBRARIES:        ${DDCUTIL_LIBRARIES}"    )

configure_file(cconfig.h.in config.h)

set(SOURCES
    src/c_util/data_structures.c
    src/c_util/ddcutil_config_file.c
    src/c_util/file_util_base.c
    src/c_util/glib_util.c
    src/c_util/debug_util.c
    src/c_util/report_util.c
    src/c_util/simple_ini_file.c
    src/c_util/string_util.c
    src/c_util/xdg_util.c

    src/base/core.cpp 
    src/base/ddca_utils.cpp 
    src/base/ddcui_parms.cpp
    src/base/feature_list.c
    src/base/feature_selector.cpp 
    src/base/global_state.cpp
    src/base/monitor.h
    src/base/other_options_state.cpp
    src/base/user_interface_options_state.cpp
    src/base/widget_debug.cpp 

    src/core_widgets/enhanced_slider.cpp
    src/core_widgets/hex_number_validator.cpp
    src/core_widgets/hline_widget.cpp 
    src/core_widgets/number_entry_widget.cpp
    src/core_widgets/spin_slider.cpp

    src/cmdline/ddcui_cmd_parser.c
    src/cmdline/ddcui_cmd_parser_aux.c
    src/cmdline/ddcui_parsed_cmd.c

    src/nongui/ddc_error.cpp 
    src/nongui/ddca_simulator.cpp
    src/nongui/feature_base_model.cpp 
    src/nongui/feature_value.cpp 
    src/nongui/msgbox_queue.cpp
    src/nongui/vcprequest.cpp 
    src/nongui/vcpthread.cpp 

    src/monitor_desc/monitor_desc_actions.cpp
    src/monitor_desc/monitor_desc_ui.cpp 

    src/feature_value_widgets/value_2button_widget.cpp 
    src/feature_value_widgets/value_base_widget.cpp
    src/feature_value_widgets/value_bytes_widget.cpp
    src/feature_value_widgets/value_cnc_widget_x14.cpp
    src/feature_value_widgets/value_nc_widget.cpp 
    src/feature_value_widgets/value_ncplus_widget.cpp
    src/feature_value_widgets/value_new_cont_widget.cpp 
    src/feature_value_widgets/value_reset_widget.cpp 
    src/feature_value_widgets/value_simple_cont_widget.cpp
    src/feature_value_widgets/value_special_widget_x62.cpp
    src/feature_value_widgets/value_stacked_widget.cpp 
    src/feature_value_widgets/value_std_widget.cpp 

    src/feature_scrollarea/feature_widget.cpp 
    src/feature_scrollarea/feature_widget_header.cpp 
    src/feature_scrollarea/features_scrollarea.cpp 
    src/feature_scrollarea/features_scrollarea_contents.cpp 
    src/feature_scrollarea/features_scrollarea_ui.cpp 
    src/feature_scrollarea/features_scrollarea_view.cpp

    src/help/help_dialog.cpp
    src/help/help_browser.cpp
    ddcui.qrc

    src/action_dialogs/debug_actions_dialog.cpp
    src/option_dialogs/feature_selection_dialog.cpp 
    src/option_dialogs/other_options_dialog.cpp
    src/option_dialogs/user_interface_options_dialog.cpp

    src/main/main.cpp
    src/main/mainwindow.cpp 
    src/main/mainwindow_ui.cpp
    src/main/monitor.cpp 
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
# add_compile_options(-Og -H V=1)

# redundant: ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS}. 
#            ${Qt5Help_INCLUDE_DIRS} is a superset of them all 
target_include_directories(ddcui SYSTEM PRIVATE 
                                 ${Qt5Help_INCLUDE_DIRS}
                                 ${GLIB2_INCLUDE_DIRS} 
                                 ${DDCUTIL_INCLUDE_DIRS})

target_include_directories(ddcui PRIVATE src)

get_property(dirs TARGET ddcui PROPERTY INCLUDE_DIRECTORIES)
message( VERBOSE "INCLUDE_DIRECTORIES:     ${dirs}" )
message( VERBOSE "Expanded:" )
foreach(d ${dirs})
  message(VERBOSE "  ${d}")
endforeach()

# Use the Widgets module from Qt 5.
target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES})


### make clean

# ADDITIONAL_MAKE_CLEAN_FILES(  ... )


### make install

# define GNU standard installation directories
include(GNUInstallDirs)
message( VERBOSE "GNU installation directories:")
message( VERBOSE "  CMAKE_INSTALL_BINDIR:        ${CMAKE_INSTALL_BINDIR}" )
message( VERBOSE "  CMAKE_INSTALL_PREFIX:        ${CMAKE_INSTALL_PREFIX}" )
message( VERBOSE "  CMAKE_INSTALL_DATATROOTDIR:  ${CMAKE_INSTALL_DATAROOTDIR}" )

install(TARGETS ddcui                       DESTINATION ${CMAKE_INSTALL_BINDIR} )
install(FILES man/ddcui.1                   DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/man/man1)
install(FILES AUTHORS CHANGELOG.md NEWS.md README.md 
                                            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/ddcui)
install(FILES ddcui.desktop                 DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES ddcui.appdata.xml             DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/metainfo)
install(FILES icons/video-display-128px.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/128x128/apps RENAME ddcui.png)
install(FILES icons/video-display-64px.png  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/64x64/apps   RENAME ddcui.png)
install(FILES icons/video-display-48px.png  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps   RENAME ddcui.png)
install(FILES icons/video-display-32px.png  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps   RENAME ddcui.png)
install(FILES icons/video-display-16px.png  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/16x16/apps   RENAME ddcui.png)
# install(FILES icons/ddcui.svg       DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
# GNOME classic only looks in pixmaps for icons:
# install(FILES icons/ddcui-48px.png  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pixmaps RENAME ddcui.png)


### make uninstall

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
