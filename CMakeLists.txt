cmake_minimum_required(VERSION 3.20)

project(dimeta-tests)

list(
  APPEND
  CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

include(config-util)
include(llvm-util)

add_subdirectory(support)

find_program(must_run
    NAME mustrun
    PATHS ${MUST_PATH} ${MUST_PATH}/bin ${PROJECT_SOURCE_DIR}/must-dimeta/install/bin
    NO_DEFAULT_PATH
)

find_program(typeart_mpicc
    NAME typeart-mpicc
    PATHS ${MUST_PATH} ${MUST_PATH}/bin ${PROJECT_SOURCE_DIR}/must-dimeta/install/bin
    NO_DEFAULT_PATH
)

find_program(typeart_mpicxx
    NAME typeart-mpic++
    PATHS ${MUST_PATH} ${MUST_PATH}/bin ${PROJECT_SOURCE_DIR}/must-dimeta/install/bin
    NO_DEFAULT_PATH
)

message(STATUS "TypeART mpicc wrapper is set to ${typeart_mpicc}")
message(STATUS "TypeART mpicc wrapper is set to ${typeart_mpicxx}")
message(STATUS "MUST mustrun is set to ${must_run}")

add_subdirectory(dimeta-verifier-pass)

enable_testing()
add_subdirectory(testsuite)
