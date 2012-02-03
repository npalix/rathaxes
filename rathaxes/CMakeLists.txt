ADD_SUBDIRECTORY(compiler)
ADD_SUBDIRECTORY(samples)

# Generate a rathaxes binary script that set up all the path automatically
# thanks to CMake.
#
# These script are meant to be used for development and use path in the
# build/source directories. Other and equivalent scripts are generated from
# maintainers/launchers and are meant to be installed (i.e: with make install),
# these launchers refers to the path as they will once installed (i.e: after
# make package + installation).

IF (WIN32)
    SET(RATHAXES_SCRIPT_NAME "rathaxes.bat")
ELSE (WIN32)
    SET(RATHAXES_SCRIPT_NAME "rathaxes")
ENDIF (WIN32)

SET(CNORM_PATH "${CMAKE_SOURCE_DIR}/maintainers/cnorm/src/")
SET(RATHAXES_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
SET(CODEWORKER_PATH "${CODEWORKER_BINARY_PATH}")

CONFIGURE_FILE("${RATHAXES_SCRIPT_NAME}.in" "${RATHAXES_SCRIPT_NAME}" @ONLY)

# Install rules for the compiler files:

INSTALL(DIRECTORY compiler DESTINATION lib/rathaxes
        PATTERN compiler/tests EXCLUDE)
INSTALL(DIRECTORY samples DESTINATION share/doc/rathaxes
        PATTERN "SamplesCMakeLists.txt" EXCLUDE)

# Install rules for the "standalone" build system to generate the examples:

# Top-level CMakeLists.txt
INSTALL(FILES samples/SamplesCMakeLists.txt
        DESTINATION share/doc/rathaxes/samples
        RENAME CMakeLists.txt)

# UseRathaxes.cmake
INSTALL(FILES
        ${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/UseRathaxes.cmake
        DESTINATION share/doc/rathaxes/samples/CMakeScripts/)
