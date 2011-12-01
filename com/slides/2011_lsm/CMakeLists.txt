# This is needed to use our latex classes:
RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(rmll.tex
                   DEFAULT_PDF MANGLE_TARGET_NAMES
		   IMAGE_DIRS .
                  )
