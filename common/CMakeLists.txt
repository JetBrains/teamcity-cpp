add_library(
    common OBJECT
    teamcity_messages.cpp
    teamcity_messages.h
  )

install(
    FILES
        teamcity_messages.cpp
        teamcity_messages.h
    DESTINATION "${CMAKE_INSTALL_DATADIR}"
    COMPONENT "${TC_COMMON}"
  )
