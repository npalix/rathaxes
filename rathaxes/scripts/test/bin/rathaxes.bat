@echo off

rem We have to explicitely give the path of the script as the first argument on
rem Windows.

%CODEWORKER% --quiet %INCLUDE_PATHS% -script %COMPILER_PATH%/rathaxes.cws -args %COMPILER_PATH%/rathaxes.cws %LOCAL_CACHE_PATH%/ %*
