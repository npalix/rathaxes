# Package Finder for Rathaxes and CMake.
#
# This finder simply seek for the rathaxes compiler and stores its path in the
# variable RATHAXES_BINARY_PATH, if rathaxes is found.
#
# This finder is used internally by UseRathaxes.

IF (NOT RATHAXES_BINARY_PATH)
    FIND_PROGRAM(RATHAXES_BINARY_PATH
                 NAMES
                 rathaxes
                 rathaxes.bat)
ELSE (NOT RATHAXES_BINARY_PATH)
    SET(RATHAXES_FIND_QUIETLY TRUE)
ENDIF (NOT RATHAXES_BINARY_PATH)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Rathaxes DEFAULT_MSG RATHAXES_BINARY_PATH)
