# Generate the binary scripts to install, these are different from the ones in
# rathaxes/bin/. The scripts in rathaxes/bin are generated to be used from the
# build directory, while here, the scripts are generated to be installed and
# thus refer to the hierarchy (e.g: ${CMAKE_INSTALL_PREFIX}/...)

IF (WIN32)
    # On windows you have to use relative paths and install the proper
    # shortcut using nsis.
    SET(PATH_PREFIX "")
    SET(RATHAXES_SCRIPT_NAME "rathaxesvars.bat")

    INSTALL(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/rathaxes.bat" DESTINATION bin)
ELSE (WIN32)
    SET(PATH_PREFIX "${CMAKE_INSTALL_PREFIX}")
    SET(RATHAXES_SCRIPT_NAME "rathaxes")
ENDIF (WIN32)

SET(CNORM_PATH "${PATH_PREFIX}/lib/rathaxes/cnorm/")
SET(COMPILER_PATH "${PATH_PREFIX}/lib/rathaxes/compiler/")
SET(CODEWORKER_PATH "${PATH_PREFIX}/lib/rathaxes/codeworker${CMAKE_EXECUTABLE_SUFFIX}")

CONFIGURE_FILE("${RATHAXES_SCRIPT_NAME}.in" "${RATHAXES_SCRIPT_NAME}" @ONLY)

INSTALL(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/${RATHAXES_SCRIPT_NAME}" DESTINATION bin)
