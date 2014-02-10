PROJECT(RATHAXES C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Setting vars #################################################################

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(RATHAXES_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

MESSAGE(STATUS "CMake Version: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
MESSAGE(STATUS "Rathaxes Version: ${RATHAXES_VERSION}")
MESSAGE(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "Source Directory: ${RATHAXES_SOURCE_DIR}")
MESSAGE(STATUS "Binary Directory: ${RATHAXES_BINARY_DIR}")

OPTION(WITH_DOCUMENTATION "Build the Rathaxes documentation and slides")

# Platform checks/includes #####################################################

IF (${RATHAXES_SOURCE_DIR} STREQUAL ${RATHAXES_BINARY_DIR})
    MESSAGE(FATAL_ERROR "Rathaxes must be built from a build directory, e.g.:
$ rm -f CMakeCache.txt ; mkdir build ; cd build ; cmake ..")
    # FILE(REMOVE "CMakeCache.txt") doesn't work so include it into the snippet.
ENDIF (${RATHAXES_SOURCE_DIR} STREQUAL ${RATHAXES_BINARY_DIR})

LIST(APPEND CMAKE_MODULE_PATH ${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts)

INCLUDE(UseRathaxes)

IF (WITH_DOCUMENTATION)
    FIND_PACKAGE(LATEX)
ENDIF (WITH_DOCUMENTATION)

INCLUDE(AddFileDependencies)
INCLUDE(FileCopy)
INCLUDE(RathaxesCopyLatexClasses)

# Global definitions ###########################################################

ENABLE_TESTING()

# Subdirectories ###############################################################

IF (WITH_DOCUMENTATION)
    IF (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)
        INCLUDE(UseLATEX)

        ADD_SUBDIRECTORY(doc)
        ADD_SUBDIRECTORY(com)
    ELSE (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)
        MESSAGE(STATUS "ps2pdf (shipped with ghostscript) and pdflatex needed to generate documentation.")
    ENDIF (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)
ELSE (WITH_DOCUMENTATION)
    MESSAGE(STATUS "Documentation building disabled")
ENDIF (WITH_DOCUMENTATION)
ADD_SUBDIRECTORY(rathaxes)

# Install rules ################################################################

# We install codeworker in lib to not conflict with an already installed
# codeworker.
INSTALL(PROGRAMS ${CODEWORKER_BINARY_PATH} DESTINATION lib/rathaxes
        # make sure it's named like expected:
        RENAME "codeworker${CMAKE_EXECUTABLE_SUFFIX}")

# Uninstall rules ##############################################################

CONFIGURE_FILE("${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/cmake_uninstall.cmake.in"
               "${RATHAXES_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY
              )

ADD_CUSTOM_TARGET(uninstall
                  "${CMAKE_COMMAND}" -P "${RATHAXES_BINARY_DIR}/cmake_uninstall.cmake"
                 )

# CPack stuff ##################################################################

SET(PACKAGE_NAME "rathaxes")
SET(IGNORE_PATTERNS
    "/\\\\.hg.*"
    ".*\\\\.sw.$"
    "/CMakeFiles/"
    "CMakeCache.txt"
    ".*install.*\\\\.(cmake|txt)$"
    ".*(Makefile|CTest).*"
    ".*CPack.*([^t][^x][^t])$"
    "/Test_.*/"
    "/build/"
    ".*\\\\.r?tree$"
    ".*\\\\.pp$"
    # cnorm repo workarounds:
    "/cnorm/testing/"
   )
SET(PACKAGE_FILE_NAME "${PACKAGE_NAME}-${RATHAXES_VERSION}")
SET(PACKAGE_SOURCE_FILE_NAME "${PACKAGE_NAME}-src-${RATHAXES_VERSION}")

SET(CPACK_PACKAGE_NAME ${PACKAGE_FILE_NAME})
SET(CPACK_PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/CPackDescription.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rathaxes, the cross-platform driver generator.")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${PACKAGE_FILE_NAME})
SET(CPACK_PACKAGE_VENDOR "Rathaxes")

SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_SOURCE_FILE_NAME})
SET(CPACK_SOURCE_IGNORE_FILES ${IGNORE_PATTERNS})

SET(CPACK_RESOURCE_FILE_LICENSE "${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/CPackLicense.txt")
SET(CPACK_RESOURCE_FILE_README "${CPACK_PACKAGE_DESCRIPTION_FILE}")
SET(CPACK_RESOURCE_FILE_WELCOME "${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/CPackWelcome.txt")

IF (UNIX)
    SET(CPACK_GENERATOR "TGZ")
    SET(CPACK_SOURCE_GENERATOR "TGZ")
ENDIF (UNIX)

IF (WIN32)
    SET(CPACK_GENERATOR "NSIS")
    SET(CPACK_SOURCE_GENERATOR "ZIP")
    # Add a page in the installer wizard that ask to put rathaxes in the PATH:
    SET(CPACK_NSIS_MODIFY_PATH ON)
ENDIF (WIN32)

ADD_SUBDIRECTORY(maintainers)

INCLUDE(CPack)
