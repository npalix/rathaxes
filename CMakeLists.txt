PROJECT(RATHAXES NONE)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# Setting vars #################################################################

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(RATHAXES_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

MESSAGE(STATUS "CMake Version : ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
MESSAGE(STATUS "Rathaxes Version : ${RATHAXES_VERSION}")
MESSAGE(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "System : ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
MESSAGE(STATUS "Install Prefix : ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "Source Directory : ${RATHAXES_SOURCE_DIR}")
MESSAGE(STATUS "Binary Directory : ${RATHAXES_BINARY_DIR}")

# Platform checks/includes #####################################################

IF (${RATHAXES_SOURCE_DIR} STREQUAL ${RATHAXES_BINARY_DIR})
    MESSAGE(FATAL_ERROR "Rathaxes must be built from a build directory, e.g.:
$ rm -f CMakeCache.txt ; mkdir build ; cd build ; cmake ..")
    # FILE(REMOVE "CMakeCache.txt") doesn't work so include it into the snippet.
ENDIF (${RATHAXES_SOURCE_DIR} STREQUAL ${RATHAXES_BINARY_DIR})

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts)

FIND_PACKAGE(LATEX)

INCLUDE(AddFileDependencies)
INCLUDE(FileCopy)
INCLUDE(RathaxesCopyLatexClasses)

# Global definitions ###########################################################

ENABLE_TESTING()

# Subdirectories ###############################################################

IF (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)
    INCLUDE(UseLATEX)

    ADD_SUBDIRECTORY(doc)
    ADD_SUBDIRECTORY(com)
    ADD_SUBDIRECTORY(epitech)
    ADD_SUBDIRECTORY(maintainers)
ELSE (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)
    MESSAGE(SEND_ERROR "ps2pdf (shipped with ghostscript) and pdflatex needed to generate documentation.")
ENDIF (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)

# CPack stuff ##################################################################

SET(PACKAGE_NAME "rathaxes")
SET(IGNORE_PATTERNS
    "/\\\\.hg.*"
    ".*\\\\.sw.?$"
    "/CMakeFiles/"
    "CMakeCache.txt"
    ".*install.*\\\\.(cmake|txt)$"
    ".*(Makefile|CTest).*"
    ".*CPack.*([^t][^x][^t])$"
    "/Test_.*/"
    # delete some legacy stuff and save some space
    "/management/"
    "/epitech/2009/"
   )
SET(PACKAGE_FILE_NAME "${PACKAGE_NAME}-${RATHAXES_VERSION}")
SET(PACKAGE_SOURCE_FILE_NAME "${PACKAGE_NAME}-src-${RATHAXES_VERSION}")

SET(CPACK_PACKAGE_NAME ${PACKAGE_FILE_NAME})
SET(CPACK_PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/CPackDescription.txt")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_SOURCE_FILE_NAME})
SET(CPACK_SOURCE_IGNORE_FILES ${IGNORE_PATTERNS})
SET(CPACK_RESOURCE_FILE_LICENSE "${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/CPackLicense.txt")
SET(CPACK_RESOURCE_FILE_README "${CPACK_PACKAGE_DESCRIPTION_FILE}")
SET(CPACK_RESOURCE_FILE_WELCOME "${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/CPackWelcome.txt")
SET(CPACK_PACKAGE_VENDOR "Rathaxes Team")

IF (UNIX)
    SET(CPACK_GENERATOR "TGZ")
    SET(CPACK_SOURCE_GENERATOR "TGZ")
ENDIF (UNIX)

IF (WIN32)
    SET(CPACK_GENERATOR "NSIS")
    SET(CPACK_SOURCE_GENERATOR "ZIP")
ENDIF (WIN32)

INCLUDE(CPack)
