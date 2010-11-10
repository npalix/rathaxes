# Sucks hard, but makes things easier to use:
MACRO(RATHAXES_COPY_LATEX_CLASSES)
    FILE_COPY(${RATHAXES_BINARY_DIR}/maintainers/latex/rtxarticle.cls ${CMAKE_CURRENT_BINARY_DIR})
    FILE_COPY(${RATHAXES_BINARY_DIR}/maintainers/latex/rtxreport.cls ${CMAKE_CURRENT_BINARY_DIR})
    FILE_COPY(${RATHAXES_BINARY_DIR}/maintainers/latex/rathaxes.sty ${CMAKE_CURRENT_BINARY_DIR})
ENDMACRO(RATHAXES_COPY_LATEX_CLASSES)
