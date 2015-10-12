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
  )

render_test_runner(
    TARGET gtest_test
  )

render_component_config(gtest)

install(
    FILES
        teamcity_gtest.cpp
        teamcity_gtest.h
    DESTINATION "${PROJECT_NAME}-gtest-${PROJECT_VERSION}"
    COMPONENT gtest
  )

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-gtest.cmake"
    DESTINATION "${PROJECT_NAME}-gtest-${PROJECT_VERSION}/cmake"
    COMPONENT gtest
  )

cpack_add_component(
    gtest
    DISPLAY_NAME "Google Test support"
  )
