add_executable(
    gtest_test
    gtest_test.cpp
    teamcity_gtest.cpp
    $<TARGET_OBJECTS:common>
  )

target_link_libraries(
    gtest_test
    ${GTEST_LIBRARY}
  )
