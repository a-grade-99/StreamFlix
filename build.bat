@echo off
echo A compilar o projeto...

gcc -Wall -Wextra -pedantic -std=c11 -Iheaders ^
source\main.c ^
source\databasetomemory.c ^
source\databasetools.c ^
source\edicaodeconteudo.c ^
source\listadefavoritos.c ^
source\metricas.c ^
source\pesquisadeconteudo.c ^
source\player.c ^
source\userinterface.c ^
-o streamflix.exe


if %errorlevel% neq 0 (
    echo Houve erros na compilação.
) else (
    echo Compilado com sucesso!
)
pause