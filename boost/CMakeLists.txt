add_executable(
    boost_test
    boost_test.cpp
    teamcity_boost.cpp
    ../common/teamcity_messages.cpp
    ../common/teamcity_messages.h
  )

target_compile_definitions(
    boost_test
    PRIVATE
    -DBOOST_TEST_DYN_LINK
  )

target_link_libraries(
    boost_test
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

render_test_runner(
    TARGET boost_test
  )
