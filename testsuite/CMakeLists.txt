find_package(LLVM CONFIG HINTS "${LLVM_DIR}")
if(NOT LLVM_FOUND)
  message(STATUS "LLVM not found at: ${LLVM_DIR}.")
  find_package(LLVM REQUIRED CONFIG)
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

macro(pythonize_bool truth_var var)
  if(${truth_var})
    set(${var} True)
  else()
    set(${var} False)
  endif()
endmacro()

ditests_find_llvm_progs(DITESTS_FILECHECK_EXEC "FileCheck-${LLVM_VERSION_MAJOR};FileCheck" ABORT_IF_MISSING)
ditests_find_llvm_progs(DITEST_CLANG_EXEC "clang-${LLVM_VERSION_MAJOR};clang" ABORT_IF_MISSING)
ditests_find_llvm_progs(DITEST_CLANGCXX_EXEC "clang++-${LLVM_VERSION_MAJOR};clang++" ABORT_IF_MISSING)

if(LLVM_EXTERNAL_LIT)
  cmake_path(GET LLVM_EXTERNAL_LIT PARENT_PATH LLVM_EXTERNAL_LIT_DIR)
endif()

ditests_find_llvm_progs(DITESTS_LIT_EXEC
  "llvm-lit;lit;lit.py"
  HINTS ${LLVM_EXTERNAL_LIT_DIR} /usr/lib/llvm-${LLVM_VERSION_MAJOR} /usr/lib/llvm /usr/bin /usr/local/bin /opt/local/bin
  ABORT_IF_MISSING
)

set(DITEST_MUST_LAYOUT_FILE ${PROJECT_SOURCE_DIR}/support/must-layout-2.xml)
set(DITEST_MUST_TIMEOUT 10)

set(DITESTS_TEST_DIR ${CMAKE_BINARY_DIR}/testsuite-exec)
file(MAKE_DIRECTORY ${DITESTS_TEST_DIR})

include(ProcessorCount)
ProcessorCount(NUM_CPU)
math(EXPR NUM_CPU "${NUM_CPU} / 4")
if(NUM_CPU EQUAL 0)
  set(NUM_CPU 1)
endif()

configure_ditests_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
)

function(ditests_add_lit_testsuite target comment)
  cmake_parse_arguments(ARG "" "" "PARAMS;DEPENDS;ARGS" ${ARGN})

  foreach(param ${ARG_PARAMS})
    list(APPEND TEST_PARAMS --param ${param})
  endforeach()

  add_custom_target(check-${target}
    COMMAND ${DITESTS_LIT_EXEC} ${ARG_ARGS} ${TEST_PARAMS} ${ARG_UNPARSED_ARGUMENTS}
    COMMENT "${comment}"
    USES_TERMINAL
  )

  add_test(NAME ${target}
    COMMAND ${DITESTS_LIT_EXEC} ${ARG_ARGS} ${TEST_PARAMS} ${ARG_UNPARSED_ARGUMENTS}
  )

  if (ARG_DEPENDS)
    add_dependencies(check-${target} ${ARG_DEPENDS})
    
    add_test(${target}_test_depend_build
      "${CMAKE_COMMAND}"
      --build "${CMAKE_BINARY_DIR}"
      --config "$<CONFIG>"
      --target ${ARG_DEPENDS}
    )
    set_tests_properties(${target}_test_depend_build PROPERTIES FIXTURES_SETUP test_fixture_${target})
    set_tests_properties(${target} PROPERTIES FIXTURES_REQUIRED test_fixture_${target})
  endif()
endfunction()

function(ditests_add_lit_target)
  cmake_parse_arguments(ARG "" "" "SUITES" ${ARGN})

  foreach(suite IN LISTS ARG_SUITES)
    if("${suite}" STREQUAL "all")
      set(SUITE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
      set(TARGET_NAME ditests)
    else()
      set(SUITE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${suite})
      set(TARGET_NAME ditests-${suite})
    endif()

    if(NOT "${suite}" STREQUAL "mpi-bugbench")
      set(depends ditest_pass)
    else()
      set(depends )
    endif()

    if(NOT EXISTS ${SUITE_PATH} AND NOT "${suite}" STREQUAL "staging")
      message(WARNING "Could not find suitable lit test target at ${SUITE_PATH}")
      continue()
    endif()

    ditests_add_lit_testsuite(${TARGET_NAME}
      "Running the lit suite ditests"
      ${SUITE_PATH}
      DEPENDS ${depends}
      ARGS -v -j ${NUM_CPU} --show-pass --show-xfail
      PARAMS ditests_site_config=${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
    )
  endforeach()
endfunction()

set(DITESTS_SUITES
  mpi-bugbench
  dimeta
  minitypes
)

ditests_add_lit_target(SUITES ${DITESTS_SUITES})


# add_test(
#   NAME ditests-lit-suite
#   COMMAND
#     ${DITESTS_LIT_EXEC} -j ${NUM_CPU} --param
#     ditests_site_config=${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )
