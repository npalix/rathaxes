ADD_RATHAXES_SOURCES(minimal_lkm_src lkm.rtx
                     RTI log.rti lkm.rti
                     BLT log.blt lkm.blt)

# We can't name lkm since it's already used as the target name to generate the
# source (with ADD_RATHAXES_SOURCES).
IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	ADD_RATHAXES_LKM(minimal_lkm minimal_lkm_src)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
