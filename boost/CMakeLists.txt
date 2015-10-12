add_executable(
    boost_test
    boost_test.cpp
    teamcity_boost.cpp
    ../common/teamcity_messages.cpp
    ../common/teamcity_messages.h
  )

target_include_directories(
    boost_test PRIVATE
    ${Boost_INCLUDE_DIRS}
  )

target_compile_definitions(
    boost_test PRIVATE
    -DBOOST_TEST_DYN_LINK
  )

target_link_libraries(
    boost_test PRIVATE
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  )

render_test_runner(
    TARGET boost_test
  )

render_component_config(boost)

install(
    FILES teamcity_boost.cpp
    DESTINATION "${CMAKE_INSTALL_DATADIR}"
    COMPONENT "${TC_BOOST}"
  )

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/teamcity-cpp-boost.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake"
    COMPONENT "${TC_BOOST}"
  )
