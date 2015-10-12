set(_rcc_base_dir "${CMAKE_CURRENT_LIST_DIR}")

function(render_component_config COMP_NAME)
    string(TOUPPER "${COMP_NAME}" _rcc_comp_upcase)
    string(TOLOWER "${COMP_NAME}" _rcc_comp_lowcase)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/teamcity_${_rcc_comp_lowcase}.h")
        set(_rcc_more_sources "\"${CMAKE_INSTALL_FULL_DATADIR}/teamcity_${_rcc_comp_lowcase}.h\"")
    endif()

    configure_file(
        "${_rtr_base_dir}/teamcity-cpp-component.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/teamcity-cpp-${_rcc_comp_lowcase}.cmake"
        @ONLY
      )
endfunction()
