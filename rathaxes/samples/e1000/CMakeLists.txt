ADD_RATHAXES_SOURCES(e1000_src
                     lkm.rtx
                     RTI
                     builtin.rti
                     log.rti
                     lkm.rti
                     device.rti
                     pci.rti
                     socket.rti
                     ethernet.rti
                     e1000.rti
                     BLT
                     log.blt
                     lkm.blt
                     pci.blt
                     device.blt
                     socket.blt
                     e1000.blt
                     ethernet.blt)

IF (LINUX_KBUILD_DIR)
    ADD_RATHAXES_LKM(e1000 e1000_src)
ENDIF (LINUX_KBUILD_DIR)
