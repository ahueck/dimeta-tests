function(ditest_add_integration_test name bench_dir bench_args exe_dir result_dir)
    message(STATUS "Configuring benchmark for testing ${name}")

    set(MUST_OUTPUT  ${result_dir}/${name})
    set(TYPEART_OUTPUT ${result_dir}/${name}.yaml)
    set(TEST_LOG_OUT_FILE ${result_dir}/${name}.out)
    file(MAKE_DIRECTORY ${MUST_OUTPUT})

    add_test(NAME test_build_${name}
        COMMAND "make" MPICC=${typeart_mpicc} MPICXX=${typeart_mpicxx} ${ARGN}
        WORKING_DIRECTORY ${bench_dir}
    )

    add_test(NAME test_clean_${name}
        COMMAND "make" clean ${ARGN}
        WORKING_DIRECTORY ${bench_dir}
    )

    add_test(
        NAME test_verifier_${name}
        COMMAND ${CMAKE_COMMAND}
                -DINPUT_FILE=${MUST_OUTPUT}/MUST_Output.json
                -P ${PROJECT_SOURCE_DIR}/cmake/verify-must-json-output.cmake
    )

    add_test(
        NAME test_log_parser_${name}
        COMMAND ${Python3_EXECUTABLE}
                ${PROJECT_SOURCE_DIR}/support/parse-log.py
                ${result_dir}/${name}.out
                ${result_dir}/${name}-counter.json
    )

    set_tests_properties(test_build_${name} PROPERTIES DEPENDS test_clean_${name})
    set_tests_properties(test_clean_${name} test_build_${name} PROPERTIES FIXTURES_SETUP ${name}_fixture)
    set_tests_properties(test_verifier_${name} test_log_parser_${name} PROPERTIES FIXTURES_CLEANUP ${name}_fixture)

    string(REPLACE " " ";" bench_arg_list ${bench_args})
    set(MUST_ARGS --must:stacktrace none --must:errorcode 0 --must:typeart --must:output json --must:quiet --must:output-dir ${MUST_OUTPUT} --must:temp ${MUST_OUTPUT}/must_temp)

    string(REPLACE ";" " " MUST_ARGS_JOINED "${MUST_ARGS}")
    string(REPLACE ";" " " BENCH_ARG_LIST_JOINED "${bench_arg_list}")

    add_test(NAME ${name}
        COMMAND /bin/bash -c "mkdir -p \"${MUST_OUTPUT}\" && \"${must_run}\" ${MUST_ARGS_JOINED} ${BENCH_ARG_LIST_JOINED} &> \"${TEST_LOG_OUT_FILE}\""
        WORKING_DIRECTORY "${exe_dir}"
    )

    set_property(TEST ${name} PROPERTY ENVIRONMENT 
        "TYPEART_TYPES=${TYPEART_OUTPUT}"
        "PRTE_ALLOW_RUN_AS_ROOT=1"
        "PRTE_ALLOW_RUN_AS_ROOT_CONFIRM=1"
        "OMPI_ALLOW_RUN_AS_ROOT=1"
        "OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1"
    )
    set_tests_properties(${name} PROPERTIES FIXTURES_REQUIRED ${name}_fixture)
endfunction()
