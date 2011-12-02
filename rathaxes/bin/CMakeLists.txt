# Generate a rathaxes binary script that set up all the path automatically
# thanks to CMake.
#
# These script are meant to be used for development and use path in the
# build/source directories. Other and equivalent scripts are generated from
# maintainers/launchers and are meant to be installed (i.e: with make install),
# these launchers refers to the path as they will once installed (i.e: after
# make package + installation).

IF (WIN32)
    SET(RATHAXES_SCRIPT_NAME "rathaxesvars.bat")
ELSE (WIN32)
    SET(RATHAXES_SCRIPT_NAME "rathaxes")
ENDIF (WIN32)

SET(CNORM_PATH "${CMAKE_SOURCE_DIR}/maintainers/cnorm/src/")
SET(COMPILER_PATH "${CMAKE_SOURCE_DIR}/rathaxes/compiler/")
SET(CODEWORKER_PATH "${CODEWORKER_BINARY_PATH}")

CONFIGURE_FILE("${RATHAXES_SCRIPT_NAME}.in" "${RATHAXES_SCRIPT_NAME}" @ONLY)
