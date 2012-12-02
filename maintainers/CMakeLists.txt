IF (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)
    ADD_SUBDIRECTORY(latex)
ENDIF (PDFLATEX_COMPILER AND PS2PDF_CONVERTER)

# Embed CNorm and CodeWorker:

INSTALL(DIRECTORY cnorm/src/
        DESTINATION lib/rathaxes/cnorm
        PATTERN ".svn" EXCLUDE)
INSTALL(PROGRAMS ${CODEWORKER_BINARY_PATH}
        DESTINATION lib/rathaxes/)

# Generate the install rules for the rathaxes binaries

ADD_SUBDIRECTORY(bin)

# Install the Rathaxes CMakeScripts library

ADD_SUBDIRECTORY(CMakeScripts)
