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

if(LLVM_EXTERNAL_LIT)
  cmake_path(GET LLVM_EXTERNAL_LIT PARENT_PATH LLVM_EXTERNAL_LIT_DIR)
endif()

ditests_find_llvm_progs(DITESTS_LIT_EXEC
  "llvm-lit;lit;lit.py"
  HINTS ${LLVM_EXTERNAL_LIT_DIR} /usr/lib/llvm-${LLVM_VERSION_MAJOR} /usr/lib/llvm /usr/bin /usr/local/bin /opt/local/bin
  ABORT_IF_MISSING
)

set(DITEST_MUST_LAYOUT_FILE ${PROJECT_SOURCE_DIR}/support/must-layout-2.xml)

configure_ditests_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
)

function(ditests_add_lit_testsuite target comment)
  cmake_parse_arguments(ARG "" "" "PARAMS;DEPENDS;ARGS" ${ARGN})

  foreach(param ${ARG_PARAMS})
    list(APPEND TEST_PARAMS --param ${param})
  endforeach()

  add_custom_target(${target}
    COMMAND ${DITESTS_LIT_EXEC} ${ARG_ARGS} ${TEST_PARAMS} ${ARG_UNPARSED_ARGUMENTS}
    COMMENT "${comment}"
    USES_TERMINAL
  )

  if (ARG_DEPENDS)
    add_dependencies(${target} ${ARG_DEPENDS})
  endif()
endfunction()

function(ditests_add_lit_target)
  cmake_parse_arguments(ARG "" "" "SUITES" ${ARGN})

  foreach(suite IN LISTS ARG_SUITES)
    if("${suite}" STREQUAL "all")
      set(SUITE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
      set(TARGET_NAME check-ditests)
    else()
      set(SUITE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${suite})
      set(TARGET_NAME check-ditests-${suite})
    endif()

    if(NOT EXISTS ${SUITE_PATH} AND NOT "${suite}" STREQUAL "staging")
      message(WARNING "Could not find suitable lit test target at ${SUITE_PATH}")
      continue()
    endif()

    ditests_add_lit_testsuite(${TARGET_NAME}
      "Running the lit suite ditests"
      ${SUITE_PATH}
      ARGS -v -j 1
      PARAMS ditests_site_config=${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
    )
  endforeach()
endfunction()

set(DITESTS_SUITES
  all
)

ditests_add_lit_target(SUITES ${DITESTS_SUITES})


add_test(
  NAME ditests-lit-suite
  COMMAND
    ${DITESTS_LIT_EXEC} -j 1 --param
    ditests_site_config=${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
    ${CMAKE_CURRENT_SOURCE_DIR}
)
