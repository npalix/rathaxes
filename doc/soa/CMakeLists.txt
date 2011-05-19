RATHAXES_COPY_LATEX_CLASSES()

SET(SOA_TEX_FILES
    algorithm
    callback
    ioaccess
    libbus
    lkm
    lkm_linux
    lkm_openbsd
    lkm_windows
    register
    userland_interface
   )

FOREACH (I ${SOA_TEX_FILES})
    ADD_LATEX_DOCUMENT("${I}.tex" DEFAULT_PDF MANGLE_TARGET_NAMES)
    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/${I}.pdf"
            DESTINATION share/doc/rathaxes/soa/)
ENDFOREACH (I ${SOA_TEX_FILES})

ADD_LATEX_DOCUMENT(audio_ac97.tex
                   BIBFILES ../bibliography.bib
                   DEFAULT_PDF MANGLE_TARGET_NAMES
                  )
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/audio_ac97.pdf
        DESTINATION share/doc/rathaxes/soa/)
