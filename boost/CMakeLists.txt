option(TEAMCITY_BOOST_TEST_LINK_DYNAMIC "Link boost test dynamically [DEFAULT]" True)

add_executable(
    boost_test
    boost_test.cpp
    teamcity_boost.cpp
    $<TARGET_OBJECTS:common>
  )

target_include_directories(
    boost_test PRIVATE
    ${Boost_INCLUDE_DIRS}
  )

if(TEAMCITY_BOOST_TEST_LINK_DYNAMIC)
    target_compile_definitions(
        boost_test PRIVATE
        -DBOOST_TEST_DYN_LINK
      )
endif(TEAMCITY_BOOST_TEST_LINK_DYNAMIC)

target_link_libraries(
    boost_test PRIVATE
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

render_test_runner(
    TARGET boost_test
  )

install(
    FILES
        teamcity_boost.cpp
        ../common/teamcity_messages.cpp
        ../common/teamcity_messages.h
    DESTINATION "${PROJECT_NAME}-boost-${PROJECT_VERSION}"
    COMPONENT boost
  )

cpack_add_component(
    boost
    DISPLAY_NAME "Boost Unit Test Framework support"
  )
