# This is needed to use our latex classes:
RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(lsm_2012.tex
                   DEFAULT_PDF MANGLE_TARGET_NAMES
		           IMAGE_DIRS .
                  )
