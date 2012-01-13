ADD_RATHAXES_SOURCES(e1000_src lkm.rtx
                     RTI log.rti lkm.rti pci.rti socket.rti ethernet.rti e1000.rti
                     BLT log.blt lkm.blt pci.blt socket.blt ethernet.blt e1000.blt)

# We can't name lkm since it's already used as the target name to generate the
# source (with ADD_RATHAXES_SOURCES).

# does not work atm
IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	ADD_RATHAXES_LKM(e1000 e1000_src)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
