RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(2012_DEVIS_RMLL_FR_RATHAXES.tex
                   IMAGES
                   # Must be relative for ADD_LATEX_DOCUMENT:
                   BIBFILES ../../../../doc/bibliography.bib
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
