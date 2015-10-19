find_program(DIFF_EXECUTABLE diff)

if(NOT DIFF_EXECUTABLE)
    message(FATAL_ERROR "WARNING: `diff` executable not found")
endif()

set(_rtr_base_dir "${CMAKE_CURRENT_LIST_DIR}")

function(render_test_runner)
    set(_options)
    set(_one_value_args TARGET)
    set(_multi_value_args)
    cmake_parse_arguments(_rtr "${_options}" "${_one_value_args}" "${_multi_value_args}" ${ARGN})

    if(NOT (_rtr_TARGET AND TARGET ${_rtr_TARGET}))
        message(FATAL_ERROR "TARGET option is missed or invalid")
    endif()

    foreach(output "output" "error")
        foreach(kind "" "_flowid")
            if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_rtr_TARGET}${kind}.expected.${output}")
                message(FATAL_ERROR "Missed ${_rtr_TARGET}${kind}.expected.${output} in ${CMAKE_CURRENT_SOURCE_DIR}")
            endif()
            set(_rtr${kind}_expected_${output} "${CMAKE_CURRENT_SOURCE_DIR}/${_rtr_TARGET}${kind}.expected.${output}")
        endforeach()
    endforeach()

    configure_file("${_rtr_base_dir}/run_test.cmake.in" "${_rtr_TARGET}.cmake" @ONLY)

    add_test(
        NAME ${_rtr_TARGET}
        COMMAND ${CMAKE_COMMAND} -P "${_rtr_TARGET}.cmake"
      )

    add_test(
        NAME ${_rtr_TARGET}_flowid
        COMMAND ${CMAKE_COMMAND}  -DPRETEND_TEAMCITY=ON -P "${_rtr_TARGET}.cmake"
      )
endfunction()
