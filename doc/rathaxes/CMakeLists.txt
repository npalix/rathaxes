ADD_SUBDIRECTORY(technical/)

# Copy the style file to the build directory
FILE_COPY(rathaxesmmix.sty ${CMAKE_CURRENT_BINARY_DIR})
# The style file is also needed into doc/soa
FILE_COPY(rathaxesmmix.sty ${CMAKE_CURRENT_BINARY_DIR}/../soa)
# The style file is also needed in subdirs
FILE_COPY(rathaxesmmix.sty ${CMAKE_CURRENT_BINARY_DIR}/technical)

SET(RATHAXES_TEX_FILE
    introduction_to_Rathaxes.tex
    rathaxes_technical_documentation.tex
   )

FOREACH(I ${RATHAXES_TEX_FILE})
    ADD_LATEX_DOCUMENT(${I}
                       IMAGES
                       ArchitectureGrandPublic.png
                       ComunicationDiagram.png
                       functional_diagram.eps
                       implementation.eps
                       DEFAULT_PDF MANGLE_TARGET_NAMES
                      )
ENDFOREACH(I ${RATHAXES_TEX_FILE})
