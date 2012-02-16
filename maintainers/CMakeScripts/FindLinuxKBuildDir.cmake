# Check if the kernel build directory exists
#
# This finder will seek for the usual /lib/modules/`uname -r`/build/
# directory. Obviously, this is Linux specific.
#
# You can specify an alternate build directory by using the environment
# variable LINUX_KBUILD_DIR.
#
# This is necessary to build Linux kernel modules!
#
# Usage: FIND_PACKAGE(LinuxKBuildDir [REQUIRED])
#
# This will set the variable LINUX_KBUILD_DIR with the path to the KBuild
# directory, if it is found.

IF (NOT LINUX_KBUILD_DIR)
    IF ($ENV{LINUX_KBUILD_DIR})
        SET(KDIR $ENV{LINUX_KBUILD_DIR})
    ELSE ($ENV{LINUX_KBUILD_DIR})
        SET(KDIR "/lib/modules/${CMAKE_SYSTEM_VERSION}/build/")
    ENDIF ($ENV{LINUX_KBUILD_DIR})

    IF (IS_DIRECTORY "${KDIR}")
        FIND_FILE(LINUX_KBUILD_DIR_MAKEFILE "Makefile"
                  PATHS "${KDIR}"
                  # Restrict the search to this directory only:
                  NO_DEFAULT_PATH
                  NO_CMAKE_ENVIRONMENT_PATH
                  NO_CMAKE_PATH
                  NO_SYSTEM_ENVIRONMENT_PATH
                  NO_CMAKE_SYSTEM_PATH)
        # Check if it looks legit:
        IF (LINUX_KBUILD_DIR_MAKEFILE)
            SET(LINUX_KBUILD_DIR "${KDIR}" CACHE PATH "Path to the Linux KBuild directory (usually /lib/modules/`uname -r`/build/)")
        ENDIF (LINUX_KBUILD_DIR_MAKEFILE)
    ENDIF (IS_DIRECTORY "${KDIR}")
ELSE (NOT LINUX_KBUILD_DIR)
    SET(LINUXKBUILDDIR_FIND_QUIETLY TRUE)
ENDIF (NOT LINUX_KBUILD_DIR)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LinuxKBuildDir DEFAULT_MSG LINUX_KBUILD_DIR)
