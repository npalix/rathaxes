# This is needed to use our latex classes:
RATHAXES_COPY_LATEX_CLASSES()


ADD_LATEX_DOCUMENT(2012_UD2_FR_RATHAXES.tex
                   BIBFILES
                   ../../doc/bibliography.bib # Must be relative
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
