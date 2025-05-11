function(ditest_add_integration_test name bench_dir bench_args exe_dir)
    message(STATUS "Configuring benchmark for testing ${name}")

    set(MUST_OUTPUT  ${DITEST_MINIAPP_INTEGRATION_RESULT}/${name})
    set(TYPEART_OUTPUT ${DITEST_MINIAPP_INTEGRATION_RESULT}/${name}.yaml)
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
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/verify-must-output.cmake
    )

    set_tests_properties(test_build_${name} PROPERTIES DEPENDS test_clean_${name})
    set_tests_properties(test_clean_${name} test_build_${name} PROPERTIES FIXTURES_SETUP ${name}_fixture)
    set_tests_properties(test_verifier_${name} PROPERTIES FIXTURES_CLEANUP ${name}_fixture)

    string(REPLACE " " ";" bench_arg_list ${bench_args})
    set(MUST_ARGS --must:errorcode 0 --must:typeart --must:output json --must:quiet --must:output-dir ${MUST_OUTPUT} --must:temp ${MUST_OUTPUT}/must_temp)
    set(MUST_OUT_ARGS > ${DITEST_MINIAPP_INTEGRATION_RESULT}/${name}.out)
    add_test(NAME ${name}
        COMMAND "${must_run}" ${MUST_ARGS} ${bench_arg_list} ${MUST_OUT_ARGS}
        WORKING_DIRECTORY "${exe_dir}"
    )

    set_property(TEST test_build_${name} ${name} PROPERTY ENVIRONMENT "TYPEART_TYPES=${TYPEART_OUTPUT}")    
    set_tests_properties(${name} PROPERTIES FIXTURES_REQUIRED ${name}_fixture)
endfunction()

set(DITEST_MINIAPP_INTEGRATION_RESULT ${PROJECT_BINARY_DIR}/miniapp-exec)
file(MAKE_DIRECTORY ${DITEST_MINIAPP_INTEGRATION_RESULT})

list(APPEND benchmarks "lulesh" "104.milc" "122.tachyon" "amg2013" "tealeaf")
list(APPEND benchmark_work_dir "lulesh" "104.milc" "122.tachyon" "amg2013/test" "tealeaf")
list(APPEND benchmark_args  "-np 8 ./lulesh2.0 -s 12 -i 100" 
                            "-np 4 104.milc < data/mtest_su3imp.in"
                            "-np 4 122.tachyon data/mtest_scene.dat"
                            "-np 8 amg2013 -pooldist 1 -r 12 12 12 -P 1 1 1"
                            "-np 2 ./serial-tealeaf")

foreach(bench bench_args bench_work_dir IN ZIP_LISTS benchmarks benchmark_args benchmark_work_dir)
  ditest_add_integration_test(${bench} 
    ${CMAKE_CURRENT_SOURCE_DIR}/${bench}
    ${bench_args}
    ${CMAKE_CURRENT_SOURCE_DIR}/${bench_work_dir}
  )
endforeach()