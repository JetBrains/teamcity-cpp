cmake_minimum_required(VERSION 2.8)

# TODO `teamcity-cpp` sounds not cleancertain, maybe it is better smth
# like `cpp-tests-teamcity-itegration` (CTTI)? %)
project(teamcity-cpp)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Boost COMPONENTS unit_test_framework)
find_package(GTest)
find_package(CppUnit)

include(RenderTestRunner)
include(CTest)
enable_testing()

include_directories(common)

if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TBZ2)
endif()
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
include(CPack)

add_subdirectory(common)
if(Boost_FOUND)
    add_subdirectory(boost)
endif()
if(GTEST_FOUND)
    add_subdirectory(gtest)
endif()
if(CPPUNIT_FOUND)
    add_subdirectory(cppunit)
endif()
