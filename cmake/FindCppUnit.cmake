# - Find CppUnit using `cppunit-config`
# Search for CppUnit and set the following variables:
#  CPPUNIT_FOUND        - is package found
#  CPPUNIT_VERSION      - found package version
#  CPPUNIT_INCLUDE_DIRS - dir w/ header files
#  CPPUNIT_LIBRARIES    - libs for dynamic linkage
#
# Caller may give a hint w/ absolute path to `cppunit-config` utility
# by setting the `CPPUNIT_CONFIG_EXECUTABLE` variable.
#

#
# Copyright (C) 2015, Alex Turbov <i.zaufi@gmail.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE for details.
#


# Check if already in cache
# NOTE Feel free to check/change/add any other vars
if(NOT CPPUNIT_LIBRARIES)

    # Try to find `cppunit-config` if no hit given by user
    if(NOT CPPUNIT_CONFIG_EXECUTABLE)
        find_program(
            CPPUNIT_CONFIG_EXECUTABLE
            cppunit-config
          )
    endif()

    if(CPPUNIT_CONFIG_EXECUTABLE)
        execute_process(
            COMMAND "${CPPUNIT_CONFIG_EXECUTABLE}" --version
            OUTPUT_VARIABLE CPPUNIT_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
          )
        execute_process(
            COMMAND "${CPPUNIT_CONFIG_EXECUTABLE}" --libs
            OUTPUT_VARIABLE CPPUNIT_LIBRARIES
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
          )
        execute_process(
            COMMAND "${CPPUNIT_CONFIG_EXECUTABLE}" --include
            OUTPUT_VARIABLE CPPUNIT_INCLUDE_DIRS
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
          )
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(
        CPPUNIT
        FOUND_VAR CPPUNIT_FOUND
        REQUIRED_VARS CPPUNIT_LIBRARIES
        VERSION_VAR CPPUNIT_VERSION
      )

endif()

# X-Chewy-RepoBase: https://raw.githubusercontent.com/mutanabbi/chewy-cmake-rep/master/
# X-Chewy-Path: FindCppUnit.cmake
# X-Chewy-Version: 1.0
# X-Chewy-Description: CppUnit finder
