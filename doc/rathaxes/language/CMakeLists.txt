RATHAXES_COPY_LATEX_CLASSES()

ADD_LATEX_DOCUMENT(language_specification_fr.tex
	       DEFAULT_PDF MANGLE_TARGET_NAMES
	      )
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/language_specification_fr.pdf
    DESTINATION share/doc/rathaxes/)
