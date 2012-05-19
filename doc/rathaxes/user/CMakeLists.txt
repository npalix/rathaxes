RATHAXES_COPY_LATEX_CLASSES()

SET(USERS_DOC
    firststeps_en
    firststeps_fr
    dsl_middleend_en
    dsl_frontend_en
    dsl_frontend_fr
    dsl_backend_en
    dsl_backend_fr
   )

FOREACH(I ${USERS_DOC})
    ADD_LATEX_DOCUMENT(${I}.tex DEFAULT_PDF MANGLE_TARGET_NAMES)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${I}.pdf
            DESTINATION share/doc/rathaxes/)
ENDFOREACH(I ${USERS_DOC})
