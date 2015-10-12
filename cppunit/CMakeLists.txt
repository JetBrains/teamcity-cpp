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

install(
    FILES
        teamcity_cppunit.cpp
        teamcity_cppunit.h
        ../common/teamcity_messages.cpp
        ../common/teamcity_messages.h
    DESTINATION "${PROJECT_NAME}-cppunit-${PROJECT_VERSION}"
    COMPONENT cppunit
  )

cpack_add_component(
    cppunit
    DISPLAY_NAME "CppUnit support"
  )
