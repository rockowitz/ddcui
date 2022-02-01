# Validates CMAKE_BUILD_TYPE

message(STATUS "Initial CMAKE_BUILD_TYPE:  ${CMAKE_BUILD_TYPE}")

set(valid_build_types Release RelWithDebInfo Debug MinSizeRel)
string(TOUPPER "${valid_build_types}" uc_valid_build_types)
string(REPLACE ";" ", " pretty_valid_build_types "${valid_build_types}")
STRING(TOUPPER "${CMAKE_BUILD_TYPE}" UC_CMAKE_BUILD_TYPE )

# message(STATUS "valid_build_types:    |${valid_build_types}|")
# message(STATUS "uc_valid_build_types: |${uc_valid_build_types}|")

# For some reason, cmake gets called with -DCMAKE_BUILD_TYPE=None by dh_autoconfigure
# This hack handles that.
if ( "${UC_CMAKE_BUILD_TYPE}" STREQUAL "NONE" )
   message(STATUS "Treating CMAKE_BUILD_TYPE == None as CMAKE_BUILD_TYPE == ''")
   set(CMAKE_BUILD_TYPE "")
endif()

# https://cmake.org/pipermail/cmake/2008-September/023808.html
# If the user specifies -DCMAKE_BUILD_TYPE on the command line, take their
# definition and dump it in the cache along with proper documentation, 
# otherwise set CMAKE_BUILD_TYPE to the default prior to calling project()
set(i0 0)
if (DEFINED CMAKE_BUILD_TYPE)
  string(LENGTH "${CMAKE_BUILD_TYPE}" i0)
endif()

IF( i0 GREATER 0 ) 
   list(FIND uc_valid_build_types "${UC_CMAKE_BUILD_TYPE}" index) 
   if (index EQUAL -1)
      message(FATAL_ERROR "CMAKE_BUILD_TYPE must be one of ${pretty_valid_build_types}" )
   endif()
   # message(STATUS "index=${index}")
   list(GET valid_build_types ${index} CMAKE_BUILD_TYPE)
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

message(STATUS "Final CMAKE_BUILD_TYPE:  ${CMAKE_BUILD_TYPE}")
