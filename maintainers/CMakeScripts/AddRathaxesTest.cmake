# This function call codeworker + cnorm with the given codeworker script.
#
# The codeworker script must return 0 on success.

FUNCTION(ADD_RATHAXES_TEST CW_SCRIPT)
    STRING(REGEX MATCH "(.*)\\.cws$" TEST_NAME "${CW_SCRIPT}")
    ADD_TEST("Rathaxes_${TEST_NAME}" ${CODEWORKER_BINARY_PATH} "-I" "${RATHAXES_SOURCE_DIR}/maintainers/cnorm/src" "-I" "${RATHAXES_SOURCE_DIR}/rathaxes/scripts/test/compiler/" "-script" "${CMAKE_CURRENT_SOURCE_DIR}/${CW_SCRIPT}")
ENDFUNCTION(ADD_RATHAXES_TEST CW_SCRIPT)
