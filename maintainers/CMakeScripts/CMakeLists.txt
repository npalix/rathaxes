# CMakeScripts
INSTALL(FILES
        FindCodeWorker.cmake
        FindRathaxes.cmake
        FindLinuxKBuildDir.cmake
        DESTINATION share/rathaxes/CMakeScripts/)

# Templates used by the CMakeScripts
INSTALL(DIRECTORY Templates DESTINATION share/rathaxes/CMakeScripts/)
