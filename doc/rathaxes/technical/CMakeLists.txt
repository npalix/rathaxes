RATHAXES_COPY_LATEX_CLASSES()

SET(RATHAXES_TEX_FILE
    technical_documentation_en.tex
    technical_documentation_fr.tex
   )

FOREACH(I ${RATHAXES_TEX_FILE})
    ADD_LATEX_DOCUMENT(${I}
                       IMAGES
                       diagramme_architecture.pdf
                       logigramme.pdf
                       DEFAULT_PDF MANGLE_TARGET_NAMES
                      )
ENDFOREACH(I ${RATHAXES_TEX_FILE})

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/technical_documentation.pdf
        DESTINATION share/doc/rathaxes/
        RENAME technical_documentation_en.pdf)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/technical_documentation.pdf
        DESTINATION share/doc/rathaxes/
        RENAME technical_documentation_fr.pdf)
