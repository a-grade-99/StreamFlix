@echo off
echo A compilar o projeto...

gcc -Wall -Wextra -pedantic -std=c11 -Iheaders ^
source\main.c ^
source\storageManager.c ^
source\dataUtils.c ^
source\edicaoFilmes.c ^
source\historicoFilmes.c ^
source\metricas.c ^
source\managerPesquisa.c ^
source\managerListas.c ^
source\mainInterface.c ^
-o streamflix.exe


if %errorlevel% neq 0 (
    echo Houve erros na compilação.
) else (
    echo Compilado com sucesso!
)
pause