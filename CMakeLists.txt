cmake_minimum_required(VERSION 3.10)

message(STATUS "cmake version: ${CMAKE_VERSION}")
# message(VERBOSE) requires cmake 3.15
if (CMAKE_VERSION VERSION_LESS 15) 
   set(VERBOSE STATUS)
else()
   set(VERBOSE VERBOSE)
endif()

set(ddcui_qt_version         5.5)
set(ddcui_glib_version       2.40)   # g_option_context_parse_strv() new in 2.40
set(ddcui_libddcutil_version 2.0.0)
set(cmake_build_type_default "RelWithDebInfo")

message(${VERBOSE} "Required Minimum Versions:")
message(${VERBOSE} "  ddcui_qt_version:          ${ddcui_qt_version}" )
message(${VERBOSE} "  ddcui_qlib_version:        ${ddcui_glib_version}" )
message(${VERBOSE} "  ddcui_libddcutil_version:  ${ddcui_libddcutil_version}" ) 

option(USE_CCACHE "Use ccache" ON)
message(STATUS "USE_CCACHE:                ${USE_CCACHE}" )

option(DDCUTIL_PROJECT_DIR "Root directory of ddcutil project" OFF)
message(STATUS "DDCUTIL_PROJECT_DIR:       ${DDCUTIL_PROJECT_DIR}"  )

string(APPEND CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR} ) 

include(validate_CMAKE_BUILD_TYPE)

project(ddcui VERSION 0.3.1 LANGUAGES C CXX)

message(STATUS "Build type is: ${CMAKE_BUILD_TYPE} (CMAKE_BUILD_TYPE)")
message(${VERBOSE} "   CMAKE_CXX_FLAGS:                ${CMAKE_CXX_FLAGS}" )
message(${VERBOSE} "   CMAKE_C_FLAGS:                  ${CMAKE_C_FLAGS}" )
message(${VERBOSE} "   CMAKE_CXX_FLAGS_DEBUG:          ${CMAKE_CXX_FLAGS_DEBUG}" )
message(${VERBOSE} "   CMAKE_C_FLAGS_DEBUG:            ${CMAKE_C_FLAGS_DEBUG}" )
message(${VERBOSE} "   CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )
message(${VERBOSE} "   CMAKE_C_FLAGS_RELWITHDEBINFO:   ${CMAKE_C_FLAGS_RELWITHDEBINFO}" )
message(${VERBOSE} "   CMAKE_CXX_FLAGS_RELEASE:        ${CMAKE_CXX_FLAGS_RELEASE}" )
message(${VERBOSE} "   CMAKE_C_FLAGS_RELEASE:          ${CMAKE_C_FLAGS_RELEASE}" )

# message(${VERBOSE} "Project variables:")
# message(${VERBOSE} "  PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}" )
# message(${VERBOSE} "  PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR}" )

message(STATUS "Source directory: ${CMAKE_SOURCE_DIR} (CMAKE_SOURCE_DIR)" )
message(STATUS "Build directory:  ${CMAKE_BINARY_DIR} (CMAKE_BINARY_DIR)" )

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} ) 
   message(WARNING "Attempting to build in source directory.")
   if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
      message(WARNING "Use -B option to set the build directory.")
   endif()
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

message(${VERBOSE} "Significant environment variables:")
message(${VERBOSE} "  INCLUDE:                  $ENV{INCLUDE}")
message(${VERBOSE} "  LD_LIBRARY_PATH:          $ENV{LD_LIBARARY_PATH}")
message(${VERBOSE} "  PKG_CONFIG_PATH:          $ENV{PKG_CONFIG_PATH} ")
message(${VERBOSE} "cmake paths:")
message(${VERBOSE} "  CMAKE_MODULE_PATH:        ${CMAKE_MODULE_PATH}")
message(${VERBOSE} "  CMAKE_PREFIX_PATH:        ${CMAKE_PREFIX_PATH}")
message(${VERBOSE} "  CMAKE_BINARY_DIR:         ${CMAKE_BINARY_DIR}" )
message(${VERBOSE} "  CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}" )
message(${VERBOSE} "  EXCECUTABLE_OUTPUT_PATH:  ${EXECUTABLE_OUTPUT_PATH}" )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find includes in corresponding build directories, needed for config.h
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
message( ${VERBOSE} "Qt settings:")
message( ${VERBOSE} "  CMAKE_AUTOUIC:           ${CMAKE_AUTOUIC}")
message( ${VERBOSE} "  CMAKE_AUTOMOC:           ${CMAKE_AUTOMOC}")
message( ${VERBOSE} "  CMAKE_AUTORCC:           ${CMAKE_AUTORCC}")
# message( ${VERBOSE} "AUTOGEN_BUILD_DIR:       ${AUTOGEN_BUILD_DIR}") overrides default location

set(QT5_LIBRARIES Qt5::Widgets )
# Ensures all Qt componenets have same version: 
find_package(Qt5 ${ddcui_qt_version} COMPONENTS Core Widgets Gui Help REQUIRED)
message( ${VERBOSE} "Qt directories:")
message( ${VERBOSE} "  Qt5Widgets_INCLUDE_DIRS:  ${Qt5Widgets_INCLUDE_DIRS}" )
message( ${VERBOSE} "  Qt5Core_INCLUDE_DIRS:     ${Qt5Core_INCLUDE_DIRS}"    )
message( ${VERBOSE} "  Qt5Gui_INCLUDE_DIRS:      ${Qt5Gui_INCLUDE_DIRS}"     )
message( ${VERBOSE} "  Qt5Help_INCLUDE_DIRS:     ${Qt5Help_INCLUDE_DIRS}"    )

find_package (PkgConfig REQUIRED )
pkg_check_modules(GLIB2 REQUIRED glib-2.0>=${ddcui_glib_version})

if ( (DDCUTIL_PROJECT_DIR STREQUAL "OFF")) 
   message( ${VERBOSE} "Locating ddcutil files using pkg_check_modules")
   pkg_check_modules(DDCUTIL REQUIRED ddcutil>=${ddcui_libddcutil_version})
else()
   message( ${VERBOSE} "Using ddcutil files from ${DDCUTIL_PROJECT_DIR}")
   string(LENGTH ${DDCUTIL_PROJECT_DIR} N)
   math(EXPR lastpos "${N}-1")
   string(SUBSTRING ${DDCUTIL_PROJECT_DIR} ${lastpos} 1 CH)
   if ( ( ${CH} STREQUAL /) )
      string(SUBSTRING ${DDCUTIL_PROJECT_DIR} 0 ${lastpos} DDCUTIL_PROJECT_DIR)
      message( ${VERBOSE} "Adjusted DDCUTIL_PROJECT_DIR:  ${DDCUTIL_PROJECT_DIR}")
   endif()
   set(DDCUTIL_INCLUDE_DIRS ${DDCUTIL_PROJECT_DIR}/src/public)
   set(DDCUTIL_LIBRARY_DIRS ${DDCUTIL_PROJECT_DIR}/src/.libs)
   set(DDCUTIL_LIBRARIES ddcutil)
endif()

message( ${VERBOSE} "Set by pkg-config:" )
message( ${VERBOSE} "  GLIB2_INCLUDE_DIRS:       ${GLIB2_INCLUDE_DIRS}"   )
message( ${VERBOSE} "  GLIB2_LIBRARIES:          ${GLIB2_LIBRARIES}"      )

message( ${VERBOSE} "  DDCUTIL_INCLUDE_DIRS:     ${DDCUTIL_INCLUDE_DIRS}" )
message( ${VERBOSE} "  DDCUTIL_LIBRARY_DIRS:     ${DDCUTIL_LIBRARY_DIRS}" )
message( ${VERBOSE} "  DDCUTIL_LIBRARIES:        ${DDCUTIL_LIBRARIES}"    )

configure_file(cconfig.h.in config.h)

set(SOURCES
    src/c_util/data_structures.c
    src/c_util/ddcutil_config_file.c
    src/c_util/error_info.c
    src/c_util/file_util_base.c
    src/c_util/glib_util.c
    src/c_util/debug_util.c
    src/c_util/report_util.c
    src/c_util/simple_ini_file.c
    src/c_util/string_util.c
    src/c_util/xdg_util.c

    src/base/ddcui_core_aux.c
	src/base/ddcui_core.cpp 
    src/base/ddca_utils.cpp 
    src/base/ddcui_parms.cpp
    src/base/feature_list.c
    src/base/feature_selector.cpp 
    src/base/global_state.cpp
    src/base/monitor.h
    src/base/nc_values_state.cpp
    src/base/user_interface_options_state.cpp
    src/base/widget_debug.cpp 

    src/core_widgets/hex_number_validator.cpp
    src/core_widgets/hline_widget.cpp 
    src/core_widgets/number_entry_widget.cpp
    src/core_widgets/spin_slider.cpp

    src/cmdline/ddcui_cmd_parser.c
    src/cmdline/ddcui_cmd_parser_aux.c
    src/cmdline/ddcui_parsed_cmd.c

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
    src/option_dialogs/nc_values_dialog.cpp
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
    src/option_dialogs/nc_values_dialog.ui
    src/option_dialogs/user_interface_options_dialog.ui
    src/action_dialogs/debug_actions_dialog.ui
   )

# must come before add_executable(): 
link_directories(${DDCUTIL_LIBRARY_DIRS})

# qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})
# add_executable(ddcui ${SOURCES} ${UI_GENERATED_HEADERS})
add_executable(ddcui ${SOURCES})

# -Wno-format-pedantic -Wno-sign-compare -std=c++20 -Wno-format -pedantic -Wextra -Wall
# set(more_options -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-parameter -Wno-non-pod-varargs  )
target_compile_options(ddcui PUBLIC "-fPIC" ${coverity_cflags})
# add_compile_options(-Og -H V=1)

# redundant: ${Qt5Gui_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS}. 
#            ${Qt5Help_INCLUDE_DIRS} is a superset of them all 
target_include_directories(ddcui SYSTEM PRIVATE
                                 ${Qt5Help_INCLUDE_DIRS}
                                 ${GLIB2_INCLUDE_DIRS} 
                                 ${DDCUTIL_INCLUDE_DIRS})

target_include_directories(ddcui PRIVATE src)

# Use the Widgets module from Qt 5.
target_link_libraries(ddcui Qt5::Widgets ${GLIB2_LIBRARIES} ${DDCUTIL_LIBRARIES})

get_property(dirs TARGET ddcui PROPERTY INCLUDE_DIRECTORIES)
message( ${VERBOSE} "INCLUDE_DIRECTORIES:     ${dirs}" )
message( ${VERBOSE} "Expanded:" )
foreach(d ${dirs})
  message(${VERBOSE} "  ${d}")
endforeach()

get_property(dirs TARGET ddcui PROPERTY LINK_DIRECTORIES)
message( ${VERBOSE} "LINK_DIRECTORIES:     ${dirs}" )
message( ${VERBOSE} "Expanded:" )
foreach(d ${dirs})
  message(${VERBOSE} "  ${d}")
endforeach()

get_property(dirs TARGET ddcui PROPERTY LINK_LIBRARIES)
message( ${VERBOSE} "LINK_LIBRARIES:     ${dirs}" )
message( ${VERBOSE} "Expanded:" )
foreach(d ${dirs})
  message(${VERBOSE} "  ${d}")
endforeach()



### make clean

# ADDITIONAL_MAKE_CLEAN_FILES(  ... )


### make install

# define GNU standard installation directories
include(GNUInstallDirs)
message( ${VERBOSE} "GNU installation directories:")
message( ${VERBOSE} "  CMAKE_INSTALL_BINDIR:        ${CMAKE_INSTALL_BINDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_PREFIX:        ${CMAKE_INSTALL_PREFIX}" )
message( ${VERBOSE} "  CMAKE_INSTALL_DATATROOTDIR:  ${CMAKE_INSTALL_DATAROOTDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_SYSCONFDIR:    ${CMAKE_INSTALL_SYSCONFDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_LIBDIR:        ${CMAKE_INSTALL_LIBDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_LIBEXECDIR:    ${CMAKE_INSTALL_LIBEXECDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_INCLUDEDIR:    ${CMAKE_INSTALL_INCLUDEDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_INFODIR:       ${CMAKE_INSTALL_INFODIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_MANDIR:        ${CMAKE_INSTALL_MANDIR}" )
message( ${VERBOSE} "  CMAKE_INSTALL_DOCDIR:        ${CMAKE_INSTALL_DOCDIR}" )

# Special handling for openSuse and SLE. 
# Normally, documentation files are stored in directory /usr/share/doc/ddcui. 
# The value of CMAKE_INSTALL_DOCDIR is "share/doc/ddcui".  However, on SuSE
# systems, doc files are instead stored in directory /usr/share/doc/packages/ddcui.
# However, CMAKE_INSTALL_DOCDIR is still set to "share/doc/ddcui". 
# The following code tries to determine if we're running on a system where
# the value of CMAKE_INSTALL_DOCDIR needs to be changed to "share/doc/packages/ddcui". 
# For SuSe (and Fedora) file /etc/os-release is automatically installed
# from the default packages.  However, for SLE it is not, so the os-release check
# is not possible. Instead, a heuristic test is performed.

set(suse_special_case FALSE)
if(EXISTS "/etc/os-release")
   message(STATUS "File /etc/os-release found")
   file(STRINGS "/etc/os-release" id_line REGEX "^ID=(.*)$") 
   string(STRIP ${id_line} id_line)
   
   # Testcases:
   # set(id_line "ID=\"opensuse-leap\"")
   # set(id_line "ID=opensuse-leap")
   # message(STATUS "id_line: |${id_line}|")
   
   string(SUBSTRING ${id_line} 3 -1 distro_name0)
   string(SUBSTRING ${distro_name0} 0 1 ch)
   if (ch STREQUAL "\"") 
      string(LENGTH ${distro_name0} len)
      math(EXPR l2 "${len}-2")
      string(SUBSTRING ${distro_name0} 1 ${l2} distro_name0)
   endif()
   string(SUBSTRING ${distro_name0} 0 8 distro_name)
   message(STATUS "Distribution: distro_name = |${distro_name}|")
   if( distro_name STREQUAL "opensuse" )
      # message(STATUS "distro_name matches opensuse") 
      set(suse_special_case TRUE)
   endif() 
else() 
   message(STATUS "File /etc/os-release NOT found. Performing heuristic test for doc directory.")
   # Heuristic test for suse
   set(dir_to_test "/usr/share/doc/packages")
   file(GLOB  globbed  "${dir_to_test}/*")
   string(LENGTH "${globbed}" len_globbed)
   if (${len_globbed} GREATER 0) 
      set(suse_special_case TRUE)
      message(STATUS "Found non-empty /usr/share/doc/packages")
   endif() 
endif()
if (${suse_special_case}) 
   message(STATUS "suse special handling, setting CMAKE_INSTALL_DOCDIR")
   set(CMAKE_INSTALL_DOCDIR "share/doc/packages/ddcui")
endif()
message(STATUS "After possible fixup CMAKE_INSTALL_DOCDIR: ${CMAKE_INSTALL_DOCDIR}")


install(TARGETS ddcui                       DESTINATION ${CMAKE_INSTALL_BINDIR} )
install(FILES man/ddcui.1                   DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# message(STATUS "suse_special_case = ${suse_special_case}")
# if ( suse_special_case )
#    message(STATUS "Installing to opensuse location")
#    install(FILES AUTHORS CHANGELOG.md NEWS.md README.md 
#                                             DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/packages/ddcui)
# else()
install(FILES AUTHORS CHANGELOG.md NEWS.md README.md 
                                              DESTINATION ${CMAKE_INSTALL_DOCDIR})
# endif()

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
