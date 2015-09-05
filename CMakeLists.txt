cmake_minimum_required(VERSION 3.3)

project(teamcity-cpp VERSION 1.3 LANGUAGES CXX)

find_package(Boost 1.55 COMPONENTS unit_test_framework)
find_package(GTest)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(MacroRenderTestRunner)

include(CTest)
enable_testing()

include_directories(common)

add_subdirectory(common)
if(Boost_FOUND)
    add_subdirectory(boost)
endif()
if(GTest_FOUND)
    add_subdirectory(gtest)
endif()
