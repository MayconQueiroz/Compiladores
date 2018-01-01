::Compila e executa o compilador
gcc Atri.c -o Atri.exe
if %ERRORLEVEL% == 0 ( GOTO EXEC )
GOTO OUT
:EXEC
::Opcao -d mostra debuger interno
Atri.exe Teste.cnm
:OUT
ECHO %ERRORLEVEL%
