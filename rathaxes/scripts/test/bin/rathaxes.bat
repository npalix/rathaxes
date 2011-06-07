@echo off
rem Script d'execution du compilateur Rathaxes

SET INSTALL_PATH=c:\rathaxes\
SET CNORM_PATH=%INSTALL_PATH%\maintainers\cnorm\src\
SET RATHAXES_PATH=%INSTALL_PATH%\rathaxes\

SET INCLUDE_PATH=-I %CNORM_PATH% -I %RATHAXES_PATH%\compiler\

IF "%1"=="clean" del *.c del *.rtree goto exit

codeworker --quiet %INCLUDE_PATH% -script %RATHAXES_PATH%\compiler\rathaxes.cws -args %RATHAXES_PATH% %1 %2 %3 %4 %5 %6 %7 %8 %9

pause
:exit


