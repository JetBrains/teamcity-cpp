add_executable(
    gtest_test
    gtest_test.cpp
    teamcity_gtest.cpp
    $<TARGET_OBJECTS:common>
  )

target_include_directories(
    gtest_test PRIVATE
    ${GTEST_INCLUDE_DIRS}
  )

target_link_libraries(
    gtest_test PRIVATE
    ${GTEST_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
  )

render_test_runner(
    TARGET gtest_test
  )

install(
    FILES
        teamcity_gtest.cpp
        teamcity_gtest.h
        ../common/teamcity_messages.cpp
        ../common/teamcity_messages.h
    DESTINATION "${PROJECT_NAME}-gtest-${PROJECT_VERSION}"
    COMPONENT gtest
  )

cpack_add_component(
    gtest
    DISPLAY_NAME "Google Test support"
  )
