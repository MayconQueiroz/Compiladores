gcc Compilador.c -o Compilador0.exe
if %ERRORLEVEL% == 0 ( GOTO EXEC )
GOTO OUT
:EXEC
Compilador0.exe -d CE.cnm
:OUT
ECHO %ERRORLEVEL%
