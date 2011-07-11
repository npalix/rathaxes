@echo off
rem Script d'execution du compilateur Rathaxes

SET OS=windows
SET INSTALLPATH=c:\rathaxes\rathaxes\
SET GENERATIONPATH=%INSTALLPATH%\scripts\generation\
SET MISCPATH=%INSTALLPATH%\scripts\misc\
SET PARSINGPATH=%INSTALLPATH%\scripts\parsing\
SET CODEWORKEREXTENSION=%INSTALLPATH%\scripts\CodeWorkerExtension\
SET SEMANTICSPATH=%INSTALLPATH%\scripts\semantics\
SET COREPATH=%INSTALLPATH%\scripts\core\
SET CNORM_PATH=%INSTALLPATH%\scripts\cnorm\
SET CNORM2PATH=%INSTALLPATH%\scripts\cnorm\cnorm2c\
SET BLACKLIBRARYPATH=%INSTALLPATH%\black_library\
SET INCLUDEPATH=-I %GENERATIONPATH% -I %MISCPATH% -I %PARSINGPATH% -I %CODEWORKEREXTENSION% -I %SEMANTICSPATH% -I %COREPATH% -I %CNORM_PATH% -I %CNORM2PATH% -I %BLACKLIBRARYPATH%

IF "%1"=="clean" del *.c del *.rtree goto exit

codeworker -nologo -stack 1000 %INCLUDEPATH% -script %INSTALLPATH%\scripts\rathaxes.cws -args "windows" %INSTALLPATH% "windows" %1 %2 %3 %4 %5 %6 %7 %8 %9

pause
:exit

