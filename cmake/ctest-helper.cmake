function(ditest_add_integration_test name bench_dir bench_args exe_dir result_dir)
    message(STATUS "Configuring benchmark for testing ${name}")

    set(MUST_OUTPUT  ${result_dir}/${name})
    set(TYPEART_OUTPUT ${result_dir}/${name}.yaml)
    file(MAKE_DIRECTORY ${MUST_OUTPUT})

    add_test(NAME test_build_${name}
        COMMAND "make" MPICC=${typeart_mpicc} MPICXX=${typeart_mpicxx}
        WORKING_DIRECTORY ${bench_dir}
    )

    add_test(NAME test_clean_${name}
        COMMAND "make" clean
        WORKING_DIRECTORY ${bench_dir}
    )

    add_test(
        NAME test_verifier_${name}
        COMMAND ${CMAKE_COMMAND}
                -DINPUT_FILE=${MUST_OUTPUT}/MUST_Output.json
                -P ${PROJECT_SOURCE_DIR}/cmake/verify-must-json-output.cmake
    )

    set_tests_properties(test_build_${name} PROPERTIES DEPENDS test_clean_${name})
    set_tests_properties(test_clean_${name} test_build_${name} PROPERTIES FIXTURES_SETUP ${name}_fixture)
    set_tests_properties(test_verifier_${name} PROPERTIES FIXTURES_CLEANUP ${name}_fixture)

    string(REPLACE " " ";" bench_arg_list ${bench_args})
    set(MUST_ARGS --must:errorcode 0 --must:typeart --must:output json --must:quiet --must:output-dir ${MUST_OUTPUT} --must:temp ${MUST_OUTPUT}/must_temp)
    set(MUST_OUT_ARGS > ${result_dir}/${name}.out)
    add_test(NAME ${name}
        COMMAND "${must_run}" ${MUST_ARGS} ${bench_arg_list} ${MUST_OUT_ARGS}
        WORKING_DIRECTORY "${exe_dir}"
    )

    set_property(TEST test_build_${name} ${name} PROPERTY ENVIRONMENT "TYPEART_TYPES=${TYPEART_OUTPUT}")    
    set_tests_properties(${name} PROPERTIES FIXTURES_REQUIRED ${name}_fixture)
endfunction()
