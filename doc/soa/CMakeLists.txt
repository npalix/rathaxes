SET(SOA_TEX_FILE
    algorithm.tex
    callback.tex
    ioaccess.tex
    libbus.tex
    lkm.tex
    lkm_linux.tex
    lkm_openbsd.tex
    lkm_windows.tex
    register.tex
    userland_interface.tex
   )

FOREACH (I ${SOA_TEX_FILE})
    ADD_LATEX_DOCUMENT(${I} DEFAULT_PDF MANGLE_TARGET_NAMES)
ENDFOREACH (I ${SOA_TEX_FILE})
