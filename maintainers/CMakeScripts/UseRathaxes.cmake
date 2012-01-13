FUNCTION(_RTX_PARSE_OPTIONS ARGS RTI_OUT BLT_OUT SYSTEM_OUT)
    SET(OPTIONS "RTI" "BLT" "SYSTEM")
    SET(FILE_LISTS ${RTI_OUT} ${BLT_OUT} ${SYSTEM_OUT})

    FOREACH(I ${ARGS})
        LIST(FIND OPTIONS "${I}" OPTION_IDX)
        IF (NOT ${OPTION_IDX} EQUAL -1)
            LIST(GET FILE_LISTS ${OPTION_IDX} CURRENT_FILE_LIST)
        ELSE (NOT ${OPTION_IDX} EQUAL -1)
            IF (DEFINED CURRENT_FILE_LIST)
                LIST(APPEND ${CURRENT_FILE_LIST} "${I}")
            ELSE (DEFINED CURRENT_FILE_LIST)
                MESSAGE(SEND_ERROR "Unexpected argument: ${I}")
            ENDIF (DEFINED CURRENT_FILE_LIST)
        ENDIF (NOT ${OPTION_IDX} EQUAL -1)
    ENDFOREACH(I ${ARGS})

    IF (NOT DEFINED ${SYSTEM_OUT})
        SET(${SYSTEM_OUT} ${CMAKE_SYSTEM_NAME})
    ENDIF (NOT DEFINED ${SYSTEM_OUT})

    # It's a "return value" variable so we have to "export" it with
    # PARENT_SCOPE, otherwise it will stay local to the function.
    #
    # {RTI,BLT,SYSTEM}_OUT point to a variable name hence the double
    # expand to get the value.
    SET(${RTI_OUT} ${${RTI_OUT}} PARENT_SCOPE)
    SET(${BLT_OUT} ${${BLT_OUT}} PARENT_SCOPE)
    SET(${SYSTEM_OUT} ${${SYSTEM_OUT}} PARENT_SCOPE)
ENDFUNCTION(_RTX_PARSE_OPTIONS ARGS RTI_OUT BLT_OUT)

FUNCTION(_RTX_GENERATE_BUILD_COMMANDS
         OUT_NAME               # [in] Name of the target
         REGISTER_COMMANDS_OUT  # [out] rathaxes [rti,blt]-registers commands
         RTI_FILES BLT_FILES    # [in, in] rti and blt files
         GENERATE_COMMANDS_OUT  # [out] rathaxes generate /o commands
         OUTPUTS                # [out] name of each generate file
         SYSTEMS)               # [in] name of each target system
    FOREACH(RTI ${RTI_FILES})
        LIST(APPEND ${REGISTER_COMMANDS_OUT}
             "COMMAND" ${_RTX_CODEWORKER_COMMAND}
             "rti-register" "${CMAKE_CURRENT_SOURCE_DIR}/${RTI}")
    ENDFOREACH(RTI ${RTI_FILES})

    FOREACH(BLT ${BLT_FILES})
        LIST(APPEND ${REGISTER_COMMANDS_OUT}
            "COMMAND" ${_RTX_CODEWORKER_COMMAND}
            "blt-register" "${CMAKE_CURRENT_SOURCE_DIR}/${BLT}")
    ENDFOREACH(BLT ${BLT_FILES})

    FOREACH(OS ${SYSTEMS})
        LIST(APPEND ${OUTPUTS} "${OUT_NAME}_${OS}.c")
        # At some point I guess we will use different directories instead of
        # different file names.
        LIST(APPEND ${GENERATE_COMMANDS_OUT}
             COMMAND ${_RTX_CODEWORKER_COMMAND}
             "generate" "/o" "${OUT_NAME}_${OS}.c" "${OS}"
             "${CMAKE_CURRENT_SOURCE_DIR}/${RTX_FILE}")
    ENDFOREACH(OS ${SYSTEMS})

    SET(${REGISTER_COMMANDS_OUT} ${${REGISTER_COMMANDS_OUT}} PARENT_SCOPE)
    SET(${GENERATE_COMMANDS_OUT} ${${GENERATE_COMMANDS_OUT}} PARENT_SCOPE)
    SET(${OUTPUTS} ${${OUTPUTS}} PARENT_SCOPE)
ENDFUNCTION(_RTX_GENERATE_BUILD_COMMANDS
            OUT_NAME
            REGISTER_COMMANDS_OUT
            RTI_FILES BLT_FILES
            GENERATE_COMMANDS_OUT
            OUTPUTS
            SYSTEMS)

# This function will build a Rathaxes target. Usage:
#
# ADD_RATHAXES_SOURCES(hello.rtx
#                         [RTI hello.rti [file.rti]]
#                         [BLT hello.blt [file.blt]]
#                         [SYSTEM windows [linux]])
#
# Files are built in the same order as they are listed, except for the rtx file
# which is built last.
#
# If no system are specified with SYSTEM then the source will be generated for
# the current system (as in CMAKE_SYSTEM_NAME).
#
# At some point we will certainly have our own language definition for CMake
# but let's start with simpler things.
FUNCTION(ADD_RATHAXES_SOURCES OUT_NAME RTX_FILE)
    SET(_RTX_CODEWORKER_COMMAND
        ${CODEWORKER_BINARY_PATH} "-nologo"
        "-I" "${RATHAXES_SOURCE_DIR}/maintainers/cnorm/src"
        "-I" "${RATHAXES_SOURCE_DIR}/rathaxes/compiler/"
        "-script" "${CMAKE_SOURCE_DIR}/rathaxes/compiler/rathaxes.cws"
        "-args" "${CMAKE_SOURCE_DIR}/rathaxes/" "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/RathaxesCache/")

    LIST(REMOVE_ITEM ARGV ${OUT_NAME} ${RTX_FILE})

    # We have to expand ARGV inside quotes, so it expand as a single LIST
    # argument (otherwise it would expand as different arguments).
    _RTX_PARSE_OPTIONS("${ARGV}" RTI_FILES BLT_FILES SYSTEMS)

    _RTX_GENERATE_BUILD_COMMANDS(${OUT_NAME}
                                 REGISTER_COMMANDS
                                 "${RTI_FILES}"
                                 "${BLT_FILES}"
                                 GENERATE_COMMANDS
                                 OUTPUTS
                                 "${SYSTEMS}")

    # Create a list of the compiler files, we will use them as dependencies of
    # the target we are going to add. This way, when we modify the compiler the
    # target goes out of date and will be rebuilt at the next "make".
    FILE(GLOB_RECURSE COMPILER_FILES
         "${RATHAXES_SOURCE_DIR}/rathaxes/compiler/misc/*.cw[sp]"
         "${RATHAXES_SOURCE_DIR}/rathaxes/compiler/rtx*.cw[sp]"
         "${RATHAXES_SOURCE_DIR}/rathaxes/compiler/rathaxes.cws")

    # ADD_CUSTOM_COMMAND + ADD_CUSTOM_TARGET is a CMake idiom to add a target
    # that should be rebuilt automatically when its sources change.
    STRING(REPLACE ";" ", " SYSTEMS "${SYSTEMS}")
    ADD_CUSTOM_COMMAND(OUTPUT ${OUTPUTS}
                       COMMAND ${_RTX_CODEWORKER_COMMAND} "cache" "clear"
                       ${REGISTER_COMMANDS}
                       ${GENERATE_COMMANDS}
                       COMMENT "Building Rathaxes target ${OUT_NAME} for ${SYSTEMS}"
                       VERBATIM
                       DEPENDS ${RTX_FILE} ${RTI_FILES} ${BLT_FILES} ${COMPILER_FILES})

    ADD_CUSTOM_TARGET(${OUT_NAME} ALL DEPENDS ${OUTPUTS})
ENDFUNCTION(ADD_RATHAXES_SOURCES OUT_NAME RTX_FILE)

# This function call codeworker + cnorm with the given codeworker script. The
# path to the rathaxes source directory is given as the first argument (arg[0]).
#
# The codeworker script must return 0 on success.
FUNCTION(ADD_RATHAXES_CWS_TEST CW_SCRIPT)
    STRING(REGEX MATCH "(.*)\\.cws$" TEST_NAME "${CW_SCRIPT}")
    ADD_TEST("Rathaxes_${TEST_NAME}"
             ${CODEWORKER_BINARY_PATH}
             "-I" "${RATHAXES_SOURCE_DIR}/maintainers/cnorm/src"
             "-I" "${RATHAXES_SOURCE_DIR}/rathaxes/compiler/"
             "-script" "${CMAKE_CURRENT_SOURCE_DIR}/${CW_SCRIPT}"
             "-args" "${RATHAXES_SOURCE_DIR}/rathaxes")
ENDFUNCTION(ADD_RATHAXES_CWS_TEST CW_SCRIPT)

# This function build a native binary executable from a previously generated
# Rathaxes sources.
#
# Usage: ADD_RATHAXES_EXECUTABLE(NAME RATHAXES_SOURCE [SYSTEM])
#
# RATHAXES_SOURCE must corresponds to the first argument of a call to
# ADD_RATHAXES_SOURCES.
#
# The third argument SYSTEM may be used if the target system doesn't correspond
# to CMAKE_SYSTEM_NAME.
FUNCTION(ADD_RATHAXES_EXECUTABLE NAME RATHAXES_SOURCE)
    IF (${ARGC} EQUAL 2)
        SET(SYSTEM ${CMAKE_SYSTEM_NAME})
    ELSE (${ARGC} EQUAL 2)
        SET(SYSTEM ${ARGV2})
    ENDIF (${ARGC} EQUAL 2)

    ADD_EXECUTABLE(${NAME} "${RATHAXES_SOURCE}_${SYSTEM}.c")
    ADD_DEPENDENCIES(${NAME} ${RATHAXES_SOURCE})
ENDFUNCTION(ADD_RATHAXES_EXECUTABLE NAME RATHAXES_SOURCE)

# This function build a native kernel module from a previously generated
# Rathaxes sources. It works by setting up a build directory and calling the
# native build tools from there.
#
# Usage: ADD_RATHAXES_LKM(NAME RATHAXES_SOURCE [SYSTEM])
#
# RATHAXES_SOURCE must corresponds to the first argument of a call to
# ADD_RATHAXES_SOURCES.
#
# The third argument SYSTEM may be used if the target system doesn't correspond
# to CMAKE_SYSTEM_NAME.
FUNCTION(ADD_RATHAXES_LKM NAME RATHAXES_SOURCE)
    IF (${ARGC} EQUAL 2)
        SET(SYSTEM ${CMAKE_SYSTEM_NAME})
    ELSE (${ARGC} EQUAL 2)
        SET(SYSTEM ${ARGV2})
    ENDIF (${ARGC} EQUAL 2)

    IF (${SYSTEM} MATCHES "Linux")
        # Create a little build space for the native Linux build-chain.
        SET(MODULE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${NAME}.dir/${SYSTEM}Build/")

        # Generate the Linux Makefile to build a Linux kernel module
        SET(MODULE_MAKEFILE "${MODULE_BUILD_DIR}/Makefile")
        SET(LKM_OBJECTS "${RATHAXES_SOURCE}_${SYSTEM}.o")
        CONFIGURE_FILE("${RATHAXES_SOURCE_DIR}/maintainers/CMakeScripts/Templates/MakefileLKM.in"
                       "${MODULE_MAKEFILE}")

        SET(KERNEL_OBJECT_NAME "${RATHAXES_SOURCE}_${SYSTEM}.ko")
        ADD_CUSTOM_COMMAND(OUTPUT "${KERNEL_OBJECT_NAME}"
                           # …
                           COMMAND "sed" "-i" "/TARTE/ d" "${RATHAXES_SOURCE}_${SYSTEM}.c"
                           # The linux Makefile to build kernel module is quite
                           # picky about file location and its own name. Let's
                           # copy our source side by side with the Makefile:
                           COMMAND "${CMAKE_COMMAND}" "-E" "copy" "${RATHAXES_SOURCE}_${SYSTEM}.c" "${MODULE_BUILD_DIR}"
                           # Then call make in the little build space we created
                           COMMAND "${CMAKE_COMMAND}" "-E" "chdir" "${MODULE_BUILD_DIR}" "${CMAKE_BUILD_TOOL}"
                           # Finally copy the generated .ko back into the current binary dir
                           COMMAND "${CMAKE_COMMAND}" "-E" "copy" "${MODULE_BUILD_DIR}/${KERNEL_OBJECT_NAME}" "${CMAKE_CURRENT_BINARY_DIR}"
                           COMMENT "Building Rathaxes Linux LKM for ${NAME}"
                           VERBATIM
                           DEPENDS "${RATHAXES_SOURCE}_${SYSTEM}.c")

        ADD_CUSTOM_TARGET("${NAME}" ALL DEPENDS "${KERNEL_OBJECT_NAME}")
    ELSE (${SYSTEM} MATCHES "Linux")
        MESSAGE(STATUS "Don't know how to build kernel modules for ${SYSTEM} (yet)")
    ENDIF (${SYSTEM} MATCHES "Linux")
ENDFUNCTION(ADD_RATHAXES_LKM NAME RATHAXES_SOURCE)
