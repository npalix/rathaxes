RATHAXES_COPY_LATEX_CLASSES()

SET(RATHAXES_TEX_FILE
    fr_specification.tex
   )

FOREACH(I ${RATHAXES_TEX_FILE})
    ADD_LATEX_DOCUMENT(${I}
                       DEFAULT_PDF MANGLE_TARGET_NAMES
                      )
ENDFOREACH(I ${RATHAXES_TEX_FILE})

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/fr_specification.pdf
        DESTINATION share/doc/rathaxes/
        RENAME language_specification_fr.pdf)
