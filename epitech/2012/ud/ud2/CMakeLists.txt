RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(2012_UD2_EN_Rathaxes.tex
#                  BIBFILES
#                  ../../bibliography.bib # Must be relative
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/2012_UD2_EN_Rathaxes.pdf
        DESTINATION share/doc/rathaxes/
        RENAME rathaxes_en.pdf)

ADD_LATEX_DOCUMENT(2012_UD2_FR_Rathaxes.tex
#                  BIBFILES
#                  ../../bibliography.bib # Must be relative
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )

INSTALL(FILES ${CMAKE_CURRFRT_BINARY_DIR}/2012_UD2_FR_Rathaxes.pdf
        DESTINATION share/doc/rathaxes/
        RENAME rathaxes_en.pdf)
