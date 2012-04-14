ADD_RATHAXES_SOURCES(minimal_lkm_src lkm.rtx
                     RTI builtin.rti log.rti lkm.rti
                     BLT             log.blt lkm.blt)

IF (WIN32)
    IF ("_not_a_var_$ENV{DDKBUILDENV}" STREQUAL "_not_a_var_chk")
        ADD_RATHAXES_LKM(minimal_lkm minimal_lkm_src)
    ELSE ("_not_a_var_$ENV{DDKBUILDENV}" STREQUAL "_not_a_var_chk")
        MESSAGE(STATUS "Not in the Checked Windows Driver Development Kit environment, not building the Windows LKM")
    ENDIF ("_not_a_var_$ENV{DDKBUILDENV}" STREQUAL "_not_a_var_chk")
ENDIF (WIN32)
