RATHAXES_COPY_LATEX_CLASSES()

SET(DOCS
    technical_documentation_en
    technical_documentation_fr
   )

FOREACH(I ${DOCS})
    ADD_LATEX_DOCUMENT(${I}.tex
                       IMAGES
                       diagramme_architecture.pdf
                       logigramme.pdf
                       DEFAULT_PDF MANGLE_TARGET_NAMES
                      )
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/technical_documentation_en.pdf
            DESTINATION share/doc/rathaxes/)
ENDFOREACH(I ${DOCS})

ADD_LATEX_DOCUMENT(rtx_2k12_wrap_up.tex DEFAULT_PDF MANGLE_TARGET_NAMES)
