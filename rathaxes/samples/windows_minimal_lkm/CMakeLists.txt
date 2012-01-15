ADD_RATHAXES_SOURCES(minimal_lkm_src lkm.rtx
                     RTI log.rti lkm.rti
                     BLT log.blt lkm.blt)

IF (WIN32)
    ADD_RATHAXES_LKM(minimal_lkm minimal_lkm_src)
ENDIF (WIN32)
