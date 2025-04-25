get_filename_component(MUST_BASE_PATH "${PROJECT_SOURCE_DIR}" ABSOLUTE)

add_custom_target(must-bootstrap
    COMMAND "./must-bootstrap.sh" ${MUST_BASE_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(dimeta-bootstrap
    COMMAND "./dimeta-bootstrap.sh" ${MUST_BASE_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
