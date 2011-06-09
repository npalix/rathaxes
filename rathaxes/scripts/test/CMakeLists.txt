ADD_SUBDIRECTORY(compiler)
ADD_SUBDIRECTORY(bin)

# Install rules for the compiler files:

INSTALL(DIRECTORY compiler DESTINATION lib/rathaxes
        PATTERN compiler/tests EXCLUDE)
INSTALL(DIRECTORY samples DESTINATION share/doc/rathaxes)
