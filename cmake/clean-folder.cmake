if(NOT DEFINED FOLDER)
    message(FATAL_ERROR "FOLDER variable not defined.")
endif()

if("${FOLDER}" STREQUAL "" OR "${FOLDER}" STREQUAL "/" OR NOT IS_DIRECTORY "${FOLDER}")
    message(FATAL_ERROR "Invalid or dangerous FOLDER path")
endif()

file(GLOB contents "${FOLDER}/*" "${FOLDER}/.*")

foreach(item IN LISTS contents)
    if(NOT item MATCHES "^${FOLDER}/\\.\\.?$")  # skip . and ..
        file(REMOVE_RECURSE "${item}")
    endif()
endforeach()
