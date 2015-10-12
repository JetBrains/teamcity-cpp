cmake_minimum_required(VERSION 3.3)

# TODO `teamcity-cpp` sounds not cleancertain, maybe it is better smth
# like `cpp-tests-teamcity-itegration` (CTTI)? %)
project(teamcity-cpp VERSION 1.5 LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost 1.55 COMPONENTS unit_test_framework)
find_package(GTest)
find_package(CppUnit)

include(GNUInstallDirs)
set(CMAKE_INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}-${PROJECT_VERSION}")
set(CMAKE_INSTALL_FULL_DATADIR "${CMAKE_INSTALL_FULL_DATADIR}/${PROJECT_NAME}-${PROJECT_VERSION}")

include(RenderComponentConfig)
include(RenderTestRunner)
include(CMakePackageConfigHelpers)
include(CTest)
enable_testing()

configure_package_config_file(
    teamcity-cpp-config.cmake.in
    teamcity-cpp-config.cmake
    INSTALL_DESTINATION "${CMAKE_INSTALL_FULL_DATADIR}/cmake"
    PATH_VARS
        CMAKE_INSTALL_PREFIX
        CMAKE_INSTALL_DATADIR
  )

include_directories(common)

if(WIN32)
    set(TC_COMMON "TeamCity C++ common code")
    set(TC_BOOST "Boost Unit Test Framework support")
    set(TC_GTEST "Google Test support")
    set(TC_CPPUNIT "CppUnit support")
else()
    set(TC_COMMON "common")
    set(TC_BOOST "boost")
    set(TC_GTEST "gtest")
    set(TC_CPPUNIT "cppunit")
endif()

add_subdirectory(common)
if(Boost_FOUND)
    add_subdirectory(boost)
endif()
if(GTest_FOUND)
    add_subdirectory(gtest)
endif()
if(CPPUNIT_FOUND)
    add_subdirectory(cppunit)
endif()

install(
    FILES "${PROJECT_BINARY_DIR}/teamcity-cpp-config.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake"
    COMPONENT "${TC_COMMON}"
  )

include(CPack)
