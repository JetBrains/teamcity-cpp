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
