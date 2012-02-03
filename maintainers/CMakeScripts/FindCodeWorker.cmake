# Package Finder for CodeWorker and CMake.
#
# This finder will seek for a pre-existing codeworker installation in the
# standard system paths or in the directory pointed by the CODEWORKER_DIR
# environment variable.
#
# If codeworker cannot be found, the finder will try to download a prebuilt
# version for the current system.
#
# Usage: FIND_PACKAGE(CodeWorker [REQUIRED])
#
# This will set the variable CODEWORKER_BINARY_PATH with the path to the
# CodeWorker executable, if CodeWorker is found.

IF (NOT CODEWORKER_BINARY_PATH)
    # First try to find it on the system:
    FIND_PROGRAM(CODEWORKER_BINARY_PATH
                 NAMES
                 codeworker
                 PATHS
                 $ENV{CODEWORKER_DIR})

    # Else try to download it:
    IF (NOT CODEWORKER_BINARY_PATH)
        IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            SET(CODEWORKER_BINARY_NAME codeworker)
        ELSEIF (WIN32)
            SET(CODEWORKER_BINARY_NAME codeworker.exe)
        ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            SET(CODEWORKER_BINARY_NAME CodeWorker)
        ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

        IF (CODEWORKER_BINARY_NAME)
            SET(CODEWORKER_BINARY_PATH "${RATHAXES_BINARY_DIR}/maintainers/cnorm/${CODEWORKER_BINARY_NAME}")

            MESSAGE(STATUS "Downloading http://rathaxes.googlecode.com/files/${CODEWORKER_BINARY_NAME}")

            FILE(DOWNLOAD
                 "http://rathaxes.googlecode.com/files/${CODEWORKER_BINARY_NAME}"
                 ${CODEWORKER_BINARY_PATH}
                 STATUS CODEWORKER_DL_STATUS
                 TIMEOUT 20)

            LIST(GET CODEWORKER_DL_STATUS 0 CODEWORKER_DL_RETURN)
            LIST(GET CODEWORKER_DL_STATUS 1 CODEWORKER_DL_MESSAGE)

            IF (${CODEWORKER_DL_RETURN} EQUAL 0)
                # Use FORCE to overwrite the cache written by FIND_PROGRAM:
                SET(CODEWORKER_BINARY_PATH ${CODEWORKER_BINARY_PATH} CACHE FILEPATH "Path to the downloaded pre-built CodeWorker binary" FORCE)
                IF (UNIX)
                    EXECUTE_PROCESS(COMMAND "chmod" "+x" "${CODEWORKER_BINARY_PATH}")
                    IF (CMAKE_SYSTEM_NAME MATCHES "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
                        MESSAGE(STATUS "CodeWorker needs the 32bit compatibility libraries (ia32-libs), make sure you have them!")
                    ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
                ENDIF (UNIX)
            ELSE (${CODEWORKER_DL_RETURN} EQUAL 0)
                SET(CODEWORKER_BINARY_PATH "")
                MESSAGE(STATUS "Can't download the pre-built binary CodeWorker: ${CODEWORKER_DL_MESSAGE}")
            ENDIF (${CODEWORKER_DL_RETURN} EQUAL 0)
        ELSE (CODEWORKER_BINARY_NAME)
            MESSAGE(STATUS "No pre-built CodeWorker binary available for ${CMAKE_SYSTEM_NAME}")
        ENDIF (CODEWORKER_BINARY_NAME)
    ENDIF (NOT CODEWORKER_BINARY_PATH)
ELSE (NOT CODEWORKER_BINARY_PATH)
    SET(CODEWORKER_FIND_QUIETLY TRUE)
ENDIF (NOT CODEWORKER_BINARY_PATH)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CodeWorker DEFAULT_MSG CODEWORKER_BINARY_PATH)
