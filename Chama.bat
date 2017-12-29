::Compila e executa o compilador
gcc Compilador.c -o Compilador.exe
if %ERRORLEVEL% == 0 ( GOTO EXEC )
GOTO OUT
:EXEC
::Opcao -d mostra debuger interno
Compilador.exe -d CE.cnm
:OUT
ECHO %ERRORLEVEL%
