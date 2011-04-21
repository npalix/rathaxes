RATHAXES_COPY_LATEX_CLASSES()

SET(RATHAXES_TEX_FILE
    user_documentation_fr
    user_documentation_en
   )

FOREACH(I ${RATHAXES_TEX_FILE})
    ADD_LATEX_DOCUMENT(${I}.tex
                       DEFAULT_PDF MANGLE_TARGET_NAMES
                      )
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${I}.pdf
            DESTINATION share/doc/rathaxes/)
ENDFOREACH(I ${RATHAXES_TEX_FILE})
