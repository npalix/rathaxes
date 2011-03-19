RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(2012_CDC5_FR_Rathaxes.tex
                   IMAGES
                   ../../gantt/ganttRathaxes_v2.png
                   ../../../../doc/rathaxes/technical/diagramme_architecture.pdf
                   BIBFILES
                   ../../../../doc/bibliography.bib # Must be relative
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
