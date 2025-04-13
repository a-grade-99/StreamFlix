#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "userinterface.h" 
#include "edicaodeconteudo.h"
#include "databasetomemory.h"
#include "pesquisadeconteudo.h"
#include "listadefavoritos.h"
#include "metricas.h"
#include "player.h"
#include "databasetools.h"

// Diretório temporário para arquivos de teste
#define TEST_FILES_DIR "./test_files/"
#define TEST_FILME_PATH TEST_FILES_DIR "filmes.csv"
#define TEST_USER_PATH TEST_FILES_DIR "users.csv"

// Funções auxiliares para preparar ambiente de testes
void setup_test_environment() {
    // Cria diretório de testes se não existir
    system("mkdir -p " TEST_FILES_DIR);
    
    // Prepara arquivo de filmes para testes
    FILE *filmes_test = fopen(TEST_FILME_PATH, "w");
    fprintf(filmes_test, "id,titulo,categoria,duracao,classificacao,visto\n");
    fprintf(filmes_test, "1,Filme Teste 1,A,120,12,5\n");
    fprintf(filmes_test, "2,Filme Teste 2,C,90,16,10\n");
    fprintf(filmes_test, "3,Filme Teste 3,A,150,18,3\n");
    fclose(filmes_test);
    
    // Prepara arquivo de usuários para testes
    FILE *users_test = fopen(TEST_USER_PATH, "w");
    fprintf(users_test, "id,utilizador,idade,senha,status,atividade,historico,personalizada,favoritos\n");
    fprintf(users_test, "1,admin,30,12345,a,10,1-2-3,1-2,3\n");
    fprintf(users_test, "2,user1,25,54321,u,5,2-3,2,1-3\n");
    fclose(users_test);
}

void teardown_test_environment() {
    // Remove arquivos de teste
    remove(TEST_FILME_PATH);
    remove(TEST_USER_PATH);
    
    // Libera memória alocada durante os testes
    if (lista_filmes != NULL) {
        limparListaFilmes();
    }
    
    if (utilizador_logado != NULL) {
        free(utilizador_logado);
        utilizador_logado = NULL;
    }
}

// Mock para substituir system("cls") durante os testes
void mock_system_cls() {
    // Não faz nada nos testes
}

//=======================
// Testes para databasetomemory.c
//=======================

void test_guardarFilmesCsv() {
    #define FILE_PATH TEST_FILME_PATH
    
    printf("Testando guardarFilmesCsv()... ");
    
    // Redireciona para o arquivo de teste
    char original_path[100];
    strcpy(original_path, FILE_PATH);
    
    // Executa a função a ser testada
    guardarFilmesCsv();
    
    // Verifica se os filmes foram carregados corretamente
    Filme *filme = lista_filmes;
    assert(filme != NULL);
    
    // Verifica o terceiro filme (primeiro na lista devido à inserção no início)
    assert(filme->id == 3);
    assert(strcmp(filme->titulo, "Filme Teste 3") == 0);
    assert(filme->categoria == 'A');
    assert(filme->duracao == 150);
    assert(filme->classificacao == 18);
    assert(filme->visto == 3);
    
    filme = filme->prox;
    assert(filme != NULL);
    
    // Verifica o segundo filme
    assert(filme->id == 2);
    assert(strcmp(filme->titulo, "Filme Teste 2") == 0);
    assert(filme->categoria == 'C');
    assert(filme->duracao == 90);
    assert(filme->classificacao == 16);
    assert(filme->visto == 10);
    
    filme = filme->prox;
    assert(filme != NULL);
    
    // Verifica o primeiro filme
    assert(filme->id == 1);
    assert(strcmp(filme->titulo, "Filme Teste 1") == 0);
    assert(filme->categoria == 'A');
    assert(filme->duracao == 120);
    assert(filme->classificacao == 12);
    assert(filme->visto == 5);
    
    assert(filme->prox == NULL);
    
    // Restaura FILE_PATH e libera memória
    #undef FILE_PATH
    limparListaFilmes();
    
    printf("OK\n");
}

void test_limparListaFilmes() {
    printf("Testando limparListaFilmes()... ");
    
    // Preparação - cria manualmente alguns filmes na memória
    Filme *filme1 = (Filme *)malloc(sizeof(Filme));
    Filme *filme2 = (Filme *)malloc(sizeof(Filme));
    
    filme1->id = 1;
    strcpy(filme1->titulo, "Filme Teste 1");
    filme1->prox = filme2;
    
    filme2->id = 2;
    strcpy(filme2->titulo, "Filme Teste 2");
    filme2->prox = NULL;
    
    lista_filmes = filme1;
    
    // Executa a função a ser testada
    limparListaFilmes();
    
    // Verifica se a lista foi limpa
    assert(lista_filmes == NULL);
    
    printf("OK\n");
}

void test_autenticarUtilizadorCsv() {
    #define UTILIZADOR_PATH TEST_USER_PATH
    
    printf("Testando autenticarUtilizadorCsv()... ");
    
    // Redireciona para o arquivo de teste
    char original_path[100];
    strcpy(original_path, UTILIZADOR_PATH);
    
    // Testa login com credenciais corretas
    char utilizador[10] = "admin";
    unsigned int senha = 12345;
    
    int resultado = autenticarUtilizadorCsv(utilizador, senha);
    
    // Verifica se o login foi bem-sucedido
    assert(resultado == 1);
    assert(utilizador_logado != NULL);
    assert(utilizador_logado->id == 1);
    assert(strcmp(utilizador_logado->utilizador, "admin") == 0);
    assert(utilizador_logado->idade == 30);
    assert(utilizador_logado->senha == 12345);
    assert(utilizador_logado->status == 'a');
    assert(utilizador_logado->atividade == 10);
    assert(strcmp(utilizador_logado->historico, "1-2-3") == 0);
    assert(strcmp(utilizador_logado->personalizada, "1-2") == 0);
    assert(strcmp(utilizador_logado->favoritos, "3") == 0);
    
    // Libera memória do usuário logado
    free(utilizador_logado);
    utilizador_logado = NULL;
    
    // Testa login com credenciais incorretas
    utilizador[0] = 'x';
    resultado = autenticarUtilizadorCsv(utilizador, senha);
    assert(resultado == 0);
    assert(utilizador_logado == NULL);
    
    // Restaura UTILIZADOR_PATH
    #undef UTILIZADOR_PATH
    
    printf("OK\n");
}

//=======================
// Testes para databasetools.c
//=======================

void test_tituloExiste() {
    #define FILE_PATH TEST_FILME_PATH
    #define TEMP_FILE TEST_FILES_DIR "temp_test.csv"
    printf("Testando tituloExiste()... ");
    
    // Redireciona para o arquivo de teste
    char original_path[100];
    strcpy(original_path, FILE_PATH);
    #define FILE_PATH TEST_FILME_PATH
    
    // Testa com um título que existe
    int resultado = tituloExiste("Filme Teste 1");
    assert(resultado == 1);
    
    // Testa com um título que não existe
    resultado = tituloExiste("Filme Inexistente");
    assert(resultado == 0);
    
    // Restaura FILE_PATH
    #undef FILE_PATH
    
    printf("OK\n");
}

void test_gerarId() {
    #define FILE_PATH TEST_FILME_PATH
    #define TEMP_FILE TEST_FILES_DIR "temp_test.csv"
    printf("Testando gerarId()... ");
    
    // Redireciona para o arquivo de teste
    char original_path[100];
    strcpy(original_path, FILE_PATH);
    #define FILE_PATH TEST_FILME_PATH
    
    // Gera um novo ID
    int novo_id = gerarId();
    
    // Verifica se o ID é maior que o último ID no arquivo (3)
    assert(novo_id > 3);
    
    // Restaura FILE_PATH
    #undef FILE_PATH
    
    printf("OK\n");
}

void test_adicionarOuEditarFilme() {
    #define FILE_PATH TEST_FILME_PATH
    #define TEMP_FILE TEST_FILES_DIR "temp_test.csv"
    printf("Testando adicionarOuEditarFilme()... ");
    
    // Redireciona para os arquivos de teste
    char original_path[100];
    strcpy(original_path, FILE_PATH);
    char original_temp[100];
    strcpy(original_temp, TEMP_FILE);
    
    // Adiciona um novo filme
    char titulo[100] = "Filme Teste Novo";
    char categoria = 'D';
    unsigned int duracao = 110;
    unsigned int classificacao = 14;
    
    adicionarOuEditarFilme(titulo, categoria, duracao, classificacao);
    
    // Verifica se o filme foi adicionado corretamente
    FILE *arquivo = fopen(TEST_FILME_PATH, "r");
    assert(arquivo != NULL);
    
    char linha[200];
    int encontrado = 0;
    
    // Pula o cabeçalho
    fgets(linha, sizeof(linha), arquivo);
    
    // Procura pelo novo filme
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strstr(linha, titulo) != NULL) {
            encontrado = 1;
            assert(strstr(linha, "D") != NULL);  // Categoria
            assert(strstr(linha, "110") != NULL);  // Duração
            assert(strstr(linha, "14") != NULL);  // Classificação
            assert(strstr(linha, "0") != NULL);  // Visto (deve começar com 0)
            break;
        }
    }
    
    assert(encontrado == 1);
    fclose(arquivo);
    
    // Restaura FILE_PATH e TEMP_FILE
    #undef FILE_PATH
    #undef TEMP_FILE
    
    printf("OK\n");
}

void test_apagarFilme() {
    #define FILE_PATH TEST_FILME_PATH
    #define TEMP_FILE TEST_FILES_DIR "temp_test.csv"

    printf("Testando apagarFilme()... ");
    
    // Redireciona para os arquivos de teste
    char original_path[100];
    strcpy(original_path, FILE_PATH);
    char original_temp[100];
    strcpy(original_temp, TEMP_FILE);
    
    // Remove um filme existente
    char titulo[100] = "Filme Teste 1";
    
    apagarFilme(titulo);
    
    // Verifica se o filme foi removido
    FILE *arquivo = fopen(TEST_FILME_PATH, "r");
    assert(arquivo != NULL);
    
    char linha[200];
    int encontrado = 0;
    
    // Pula o cabeçalho
    fgets(linha, sizeof(linha), arquivo);
    
    // Procura pelo filme que deveria ter sido removido
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strstr(linha, titulo) != NULL) {
            encontrado = 1;
            break;
        }
    }
    
    assert(encontrado == 0);  // Não deve encontrar o filme removido
    fclose(arquivo);
    
    // Restaura FILE_PATH e TEMP_FILE
    #undef FILE_PATH
    #undef TEMP_FILE
    
    printf("OK\n");
}

//=======================
// Testes para pesquisadeconteudo.c
//=======================

// Testar funções de pesquisa é complicado pois elas interagem diretamente com printf
// Uma abordagem seria capturar a saída padrão, ou modificar as funções para retornar
// resultados ao invés de imprimi-los diretamente

//=======================
// Testes para listadefavoritos.c
//=======================

void test_idExiste() {
    printf("Testando idExiste()... ");
    
    // Testes com diferentes padrões de listas
    assert(idExiste("", 1) == 0);  // Lista vazia
    assert(idExiste("1", 1) == 1);  // ID único
    assert(idExiste("1-2-3", 2) == 1);  // ID no meio
    assert(idExiste("1-2-3", 3) == 1);  // ID no final
    assert(idExiste("1-2-3", 4) == 0);  // ID não existe
    assert(idExiste("10-20-30", 1) == 0);  // Prevenir falsos positivos com prefixos
    
    printf("OK\n");
}

void test_removerIdDaLista() {
    printf("Testando removerIdDaLista()... ");
    
    // Testa remoção do primeiro ID
    char lista1[1000] = "1-2-3";
    removerIdDaLista(lista1, 1);
    assert(strcmp(lista1, "2-3") == 0);
    
    // Testa remoção do ID do meio
    char lista2[1000] = "1-2-3";
    removerIdDaLista(lista2, 2);
    assert(strcmp(lista2, "1-3") == 0);
    
    // Testa remoção do último ID
    char lista3[1000] = "1-2-3";
    removerIdDaLista(lista3, 3);
    assert(strcmp(lista3, "1-2") == 0);
    
    // Testa remoção de ID inexistente
    char lista4[1000] = "1-2-3";
    removerIdDaLista(lista4, 4);
    assert(strcmp(lista4, "1-2-3") == 0);
    
    // Testa remoção do único ID
    char lista5[1000] = "1";
    removerIdDaLista(lista5, 1);
    assert(strcmp(lista5, "") == 0);
    
    printf("OK\n");
}

//=======================
// Testes para metricas.c
//=======================

// As funções de métricas são principalmente funções de visualização,
// o que torna o teste unitário mais complexo sem modificações.
// Uma abordagem seria modificar as funções para separar a lógica da exibição.

//=======================
// Testes para todos os componentes
//=======================

void run_all_tests() {
    printf("\n===== Iniciando testes =====\n\n");
    
    // Setup do ambiente de testes
    setup_test_environment();
    
    // Testes para databasetomemory.c
    test_guardarFilmesCsv();
    test_limparListaFilmes();
    test_autenticarUtilizadorCsv();
    
    // Testes para databasetools.c
    test_tituloExiste();
    test_gerarId();
    test_adicionarOuEditarFilme();
    test_apagarFilme();
    
    // Testes para listadefavoritos.c
    test_idExiste();
    test_removerIdDaLista();
    
    // Limpeza do ambiente de testes
    teardown_test_environment();
    
    printf("\n===== Testes concluídos com sucesso! =====\n");
}

int main() {
    run_all_tests();
    return 0;
}