find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(DITEST_MINIAPP_INTEGRATION_RESULT ${PROJECT_BINARY_DIR}/miniapp-exec)
file(MAKE_DIRECTORY ${DITEST_MINIAPP_INTEGRATION_RESULT})

set(DITESTS_ROCM_ARCH_DEFAULT "gfx1031")
if(DITESTS_HIP_ARCH)
  set(DITESTS_ROCM_ARCH_DEFAULT "${DITESTS_HIP_ARCH}")
endif()
set(ROCM_ARCH "${DITESTS_ROCM_ARCH_DEFAULT}" CACHE STRING "ROCm architecture for HIP tests")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/tealeaf/build.sh.in
  ${CMAKE_CURRENT_SOURCE_DIR}/tealeaf/build.sh
  @ONLY
)

list(APPEND benchmark_names "lulesh" "104.milc" "122.tachyon" "amg2013" "tealeaf" "tealeaf-cuda" "tealeaf-hip" "lulesh-ad")
list(APPEND benchmark_dirs "lulesh" "104.milc" "122.tachyon" "amg2013" "tealeaf" "tealeaf" "tealeaf" "lulesh-ad")
list(APPEND benchmark_work_dirs "lulesh" "104.milc" "122.tachyon" "amg2013/test" "tealeaf" "tealeaf" "tealeaf" "lulesh-ad/lulesh")
list(APPEND benchmark_args  "-np 8 ./lulesh2.0 -s 12 -i 100" 
                            "-np 4 104.milc < data/mtest_su3imp.in"
                            "-np 4 122.tachyon data/mtest_scene.dat"
                            "-np 8 amg2013 -pooldist 1 -r 12 12 12 -P 1 1 1"
                            "-np 2 ./serial-tealeaf"
                            "-np 2 ./cuda-tealeaf"
                            "-np 2 ./hip-tealeaf"
                            "-np 8 ./lulesh2.0 -s 12 -i 50")
list(APPEND benchmark_build_vars "" "" "" "" "MODEL=serial" "MODEL=cuda" "MODEL=hip" "")

foreach(name bench_dir bench_work_dir bench_args build_vars IN ZIP_LISTS benchmark_names benchmark_dirs benchmark_work_dirs benchmark_args benchmark_build_vars)
  
  if(name STREQUAL "tealeaf-cuda" AND NOT DITESTS_CUDA_RUNNABLE)
    message(STATUS "Skipping test ${name} (CUDA device not found)")
    continue()
  endif()

  if(name STREQUAL "tealeaf-hip" AND NOT DITESTS_HIP_RUNNABLE)
    message(STATUS "Skipping test ${name} (HIP device not found)")
    continue()
  endif()

  separate_arguments(build_var_list NATIVE_COMMAND ${build_vars})
  ditest_add_integration_test(${name} 
    ${CMAKE_CURRENT_SOURCE_DIR}/${bench_dir}
    ${bench_args}
    ${CMAKE_CURRENT_SOURCE_DIR}/${bench_work_dir}
    ${DITEST_MINIAPP_INTEGRATION_RESULT}
    ${build_var_list}
  )
endforeach()

set(CLEAN_SCRIPT "${PROJECT_SOURCE_DIR}/cmake/clean-folder.cmake")

add_custom_target(ditest-clean-miniapp-exec-folder
    COMMAND ${CMAKE_COMMAND} -DFOLDER=${DITEST_MINIAPP_INTEGRATION_RESULT} -P ${CLEAN_SCRIPT}
    COMMENT "Cleaning contents of ${DITEST_MINIAPP_INTEGRATION_RESULT}"
)
