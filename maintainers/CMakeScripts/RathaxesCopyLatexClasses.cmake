# Sucks hard, but makes things easier to use:

SET(RATHAXES_LATEX_CLASSES
    maintainers/latex/rtxarticle.cls
    maintainers/latex/rtxreport.cls
    maintainers/latex/rathaxes.sty
    maintainers/latex/rtxslides.sty
   )

# Make sure they are configured before any call to
# RATHAXES_COPY_LATEX_CLASSES() can be made.
FOREACH(I ${RATHAXES_LATEX_CLASSES})
    CONFIGURE_FILE(${RATHAXES_SOURCE_DIR}/${I} ${RATHAXES_BINARY_DIR}/${I} @ONLY)
ENDFOREACH(I ${RATHAXES_LATEX_CLASSES})

MACRO(RATHAXES_COPY_LATEX_CLASSES)
    FOREACH(I ${RATHAXES_LATEX_CLASSES})
        FILE_COPY(${RATHAXES_BINARY_DIR}/${I} ${CMAKE_CURRENT_BINARY_DIR})
    ENDFOREACH(I ${RATHAXES_LATEX_CLASSES})
ENDMACRO(RATHAXES_COPY_LATEX_CLASSES)
