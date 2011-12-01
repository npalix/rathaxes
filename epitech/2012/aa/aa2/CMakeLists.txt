RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(2012_AA2_FR_Rathaxes.tex
                   IMAGES
                   diagramme_architecture.pdf
                   diagramme_fonctionel.png
                   diagramme_general.png
                   logigramme.pdf
                   # Must be relative for ADD_LATEX_DOCUMENT:
                   BIBFILES ../../../../doc/bibliography.bib
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
