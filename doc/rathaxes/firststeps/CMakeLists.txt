RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(firststeps.tex
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/firststeps.pdf
        DESTINATION share/doc/rathaxes/)

ADD_LATEX_DOCUMENT(firststeps_fr.tex
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/firststeps_fr.pdf
        DESTINATION share/doc/rathaxes/)
