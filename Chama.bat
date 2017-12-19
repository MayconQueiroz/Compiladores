gcc Compilador.c -o Compilador.exe
if %ERRORLEVEL% == 0 ( GOTO EXEC )
GOTO OUT
:EXEC
Compilador.exe -d CE.cnm
:OUT
ECHO %ERRORLEVEL%
