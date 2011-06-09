# Generate the scripts to install

SET(CNORM_PATH "${CMAKE_INSTALL_PREFIX}/lib/rathaxes/cnorm/")
SET(COMPILER_PATH "${CMAKE_INSTALL_PREFIX}/lib/rathaxes/compiler/")
SET(CODEWORKER_PATH "${CMAKE_INSTALL_PREFIX}/lib/rathaxes/codeworker${CMAKE_EXECUTABLE_SUFFIX}")

IF (WIN32)
    SET(RATHAXES_SCRIPT_NAME "rathaxes.bat")
    CONFIGURE_FILE("rathaxes.bat.in" "rathaxes.bat" @ONLY)
ELSE (WIN32)
    SET(RATHAXES_SCRIPT_NAME "rathaxes")
    CONFIGURE_FILE("rathaxes.in" "rathaxes" @ONLY)
ENDIF (WIN32)

INSTALL(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/${RATHAXES_SCRIPT_NAME}" DESTINATION bin)
