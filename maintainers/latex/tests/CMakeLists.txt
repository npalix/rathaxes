# This is needed to use our latex classes:
RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(article.tex
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
ADD_LATEX_DOCUMENT(report.tex
                   BIBFILES
                   ../../../doc/bibliography.bib # Must be relative
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
ADD_LATEX_DOCUMENT(slides.tex
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
