set(DITEST_MINIAPP_INTEGRATION_RESULT ${PROJECT_BINARY_DIR}/miniapp-exec)
file(MAKE_DIRECTORY ${DITEST_MINIAPP_INTEGRATION_RESULT})

list(APPEND benchmarks "lulesh" "104.milc" "122.tachyon" "amg2013" "tealeaf" "lulesh-ad")
list(APPEND benchmark_work_dir "lulesh" "104.milc" "122.tachyon" "amg2013/test" "tealeaf" "lulesh-ad/lulesh")
list(APPEND benchmark_args  "-np 8 ./lulesh2.0 -s 12 -i 100" 
                            "-np 4 104.milc < data/mtest_su3imp.in"
                            "-np 4 122.tachyon data/mtest_scene.dat"
                            "-np 8 amg2013 -pooldist 1 -r 12 12 12 -P 1 1 1"
                            "-np 2 ./serial-tealeaf"
                            "-np 8 ./lulesh2.0 -s 12 -i 50")

foreach(bench bench_args bench_work_dir IN ZIP_LISTS benchmarks benchmark_args benchmark_work_dir)
  ditest_add_integration_test(${bench} 
    ${CMAKE_CURRENT_SOURCE_DIR}/${bench}
    ${bench_args}
    ${CMAKE_CURRENT_SOURCE_DIR}/${bench_work_dir}
    ${DITEST_MINIAPP_INTEGRATION_RESULT}
  )
endforeach()