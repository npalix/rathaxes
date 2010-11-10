CONFIGURE_FILE(rtxarticle.cls ${CMAKE_CURRENT_BINARY_DIR}/rtxarticle.cls @ONLY)
CONFIGURE_FILE(rtxreport.cls ${CMAKE_CURRENT_BINARY_DIR}/rtxreport.cls @ONLY)
CONFIGURE_FILE(rathaxes.sty ${CMAKE_CURRENT_BINARY_DIR}/rathaxes.sty @ONLY)

ADD_SUBDIRECTORY(tests)
