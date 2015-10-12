add_executable(
    cppunit_test
    cppunit_test.cpp
    teamcity_cppunit.cpp
    $<TARGET_OBJECTS:common>
  )

target_include_directories(
    cppunit_test PRIVATE
    ${CPPUNIT_INCLUDE_DIRS}
  )

target_link_libraries(
    cppunit_test PRIVATE
    ${CPPUNIT_LIBRARIES}
  )

render_test_runner(
    TARGET cppunit_test
  )

render_component_config(cppunit)

install(
    FILES
        teamcity_cppunit.cpp
        teamcity_cppunit.h
    DESTINATION "${PROJECT_NAME}-cppunit-${PROJECT_VERSION}"
    COMPONENT cppunit
  )

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-cppunit.cmake"
    DESTINATION "${PROJECT_NAME}-cppunit-${PROJECT_VERSION}/cmake"
    COMPONENT cppunit
  )

cpack_add_component(
    cppunit
    DISPLAY_NAME "CppUnit support"
  )
