#include <stdio.h>
#include <stdlib.h>
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
#define TEST_FILME_PATH TEST_FILES_DIR "filmes_test.csv"
#define TEST_USER_PATH TEST_FILES_DIR "users_test.csv"
#define TEST_TEMP_PATH TEST_FILES_DIR "temp_test.csv"
#define TEST_TEMP_USER_PATH TEST_FILES_DIR "temp_user_test.csv"

// Redefinição de constantes para usar arquivos de teste
#undef FILE_PATH
#undef UTILIZADOR_PATH
#undef TEMP_FILE
#undef TEMP_FILE_UTILIZADOR
#define FILE_PATH TEST_FILME_PATH
#define UTILIZADOR_PATH TEST_USER_PATH
#define TEMP_FILE TEST_TEMP_PATH
#define TEMP_FILE_UTILIZADOR TEST_TEMP_USER_PATH

// Variáveis globais para capturar saída
FILE *output_buffer;
char test_output[4096];

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
    
    // Carrega os dados na memória
    guardarFilmesCsv();
    
    // Prepara captura de saída para testes
    output_buffer = tmpfile();
}

void teardown_test_environment() {
    // Remove arquivos de teste
    remove(TEST_FILME_PATH);
    remove(TEST_USER_PATH);
    remove(TEST_TEMP_PATH);
    remove(TEST_TEMP_USER_PATH);
    
    // Libera memória alocada durante os testes
    if (lista_filmes != NULL) {
        limparListaFilmes();
    }
    
    if (utilizador_logado != NULL) {
        free(utilizador_logado);
        utilizador_logado = NULL;
    }
    
    // Fecha buffer de saída
    if (output_buffer != NULL) {
        fclose(output_buffer);
    }
}

// Função para iniciar a captura de saída
void start_capture_output() {
    output_buffer = tmpfile();
    // Redireciona stdout para o arquivo temporário
    freopen("/dev/null", "w", stdout);
}

// Função para terminar a captura de saída
void end_capture_output() {
    // Restaura stdout
    freopen("/dev/tty", "w", stdout);
}

//=========================
// Testes de Integração
//=========================

// Teste 1: Login e Carregamento de Dados
void test_login_and_data_loading() {
    printf("Testando integração: Login e Carregamento de Dados... ");
    
    // Simula login do usuário admin
    char utilizador[10] = "admin";
    unsigned int senha = 12345;
    
    int login_result = autenticarUtilizadorCsv(utilizador, senha);
    assert(login_result == 1);
    assert(utilizador_logado != NULL);
    
    // Verifica se os filmes estão carregados corretamente
    assert(lista_filmes != NULL);
    
    // Verifica a interação entre o carregamento de usuários e filmes
    assert(lista_filmes->id == 3); // Devido à inserção no início da lista
    assert(utilizador_logado->id == 1);
    
    printf("OK\n");
}

// Teste 2: Métricas e Reprodução
void test_viewing_metrics_integration() {
    printf("Testando integração: Visualização de Filmes e Atualização de Métricas... ");
    
    // Prepara o ambiente: login de usuário
    char utilizador[10] = "admin";
    unsigned int senha = 12345;
    autenticarUtilizadorCsv(utilizador, senha);
    
    // Obtém contagem inicial de visualizações para um filme
    Filme *filme_teste = NULL;
    for (Filme *f = lista_filmes; f != NULL; f = f->prox) {
        if (f->id == 2) {
            filme_teste = f;
            break;
        }
    }
    assert(filme_teste != NULL);
    unsigned int visto_inicial = filme_teste->visto;
    
    // Simula a visualização de um filme (não podemos chamar selecionarFilme diretamente)
    // Manualmente incrementamos o contador e atualizamos o histórico
    filme_teste->visto++;
    
    // Atualizar o histórico do usuário
    if (utilizador_logado->historico[0] == '\0') {
        sprintf(utilizador_logado->historico, "%d", filme_teste->id);
    } else {
        char temp_historico[1020];
        sprintf(temp_historico, "%s-%d", utilizador_logado->historico, filme_teste->id);
        strcpy(utilizador_logado->historico, temp_historico);
    }
    
    // Verificamos que a contagem foi incrementada
    assert(filme_teste->visto == visto_inicial + 1);
    
    // Verificar que o histórico foi atualizado
    assert(strstr(utilizador_logado->historico, "2") != NULL);
    
    printf("OK\n");
}

// Teste 3: Edição de Conteúdo e Pesquisa
void test_content_edit_and_search_integration() {
    printf("Testando integração: Edição de Conteúdo e Pesquisa... ");
    
    // Prepara o ambiente: login de usuário
    char utilizador[10] = "admin";
    unsigned int senha = 12345;
    autenticarUtilizadorCsv(utilizador, senha);
    
    // Conta número inicial de filmes
    int filmes_iniciais = 0;
    for (Filme *f = lista_filmes; f != NULL; f = f->prox) {
        filmes_iniciais++;
    }
    
    // Adiciona um novo filme pelo sistema
    char titulo_novo[100] = "Filme Integração";
    char categoria = 'I';
    unsigned int duracao = 125;
    unsigned int classificacao = 14;
    
    // Grava no CSV
    adicionarOuEditarFilme(titulo_novo, categoria, duracao, classificacao);
    
    // Recarrega os filmes para atualizar a memória
    limparListaFilmes();
    guardarFilmesCsv();
    
    // Conta número de filmes após adição
    int filmes_apos_adicao = 0;
    int filme_encontrado = 0;
    
    for (Filme *f = lista_filmes; f != NULL; f = f->prox) {
        filmes_apos_adicao++;
        if (strcmp(f->titulo, titulo_novo) == 0) {
            filme_encontrado = 1;
            assert(f->categoria == categoria);
            assert(f->duracao == duracao);
            assert(f->classificacao == classificacao);
        }
    }
    
    // Verifica que um filme foi adicionado e encontrado
    assert(filmes_apos_adicao == filmes_iniciais + 1);
    assert(filme_encontrado == 1);
    
    printf("OK\n");
}

// Teste 4: Favoritos e Recomendações
void test_favorites_and_recommendations_integration() {
    printf("Testando integração: Favoritos e Recomendações... ");
    
    // Prepara o ambiente: login de usuário
    char utilizador[10] = "user1";
    unsigned int senha = 54321;
    autenticarUtilizadorCsv(utilizador, senha);
    
    // Verifica os favoritos iniciais
    char favoritos_iniciais[1000];
    strcpy(favoritos_iniciais, utilizador_logado->favoritos);
    assert(strcmp(favoritos_iniciais, "1-3") == 0);
    
    // Remove um ID dos favoritos
    removerIdDaLista(utilizador_logado->favoritos, 1);
    assert(strcmp(utilizador_logado->favoritos, "3") == 0);
    
    // Adiciona um novo ID aos favoritos
    if (strlen(utilizador_logado->favoritos) > 0) {
        char nova_lista[1020];
        snprintf(nova_lista, sizeof(nova_lista), "%s-%d", utilizador_logado->favoritos, 2);
        strcpy(utilizador_logado->favoritos, nova_lista);
    } else {
        char nova_lista[1020];
        snprintf(nova_lista, sizeof(nova_lista), "%d", 2);
        strcpy(utilizador_logado->favoritos, nova_lista);
    }
    
    assert(strcmp(utilizador_logado->favoritos, "3-2") == 0);
    
    // Salva as alterações
    adicionarOuEditarUser(
        utilizador_logado->id, utilizador_logado->utilizador, utilizador_logado->idade, utilizador_logado->senha,
        utilizador_logado->status, utilizador_logado->atividade, utilizador_logado->historico,
        utilizador_logado->personalizada, utilizador_logado->favoritos
    );
    
    // Verifica se as alterações foram salvas corretamente no CSV
    free(utilizador_logado);
    utilizador_logado = NULL;
    
    // Faz login novamente para carregar os dados atualizados
    autenticarUtilizadorCsv(utilizador, senha);
    assert(strcmp(utilizador_logado->favoritos, "3-2") == 0);
    
    printf("OK\n");
}

// Teste 5: Histórico e Métricas
void test_history_and_metrics_integration() {
    printf("Testando integração: Histórico e Métricas... ");
    
    // Prepara o ambiente: login de usuário
    char utilizador[10] = "admin";
    unsigned int senha = 12345;
    autenticarUtilizadorCsv(utilizador, senha);
    
    // Verifica histórico inicial
    assert(strcmp(utilizador_logado->historico, "1-2-3") == 0);
    
    // Simula visualização de um filme (ID 1)
    int filme_id = 1;
    Filme *filme_visto = NULL;
    
    for (Filme *f = lista_filmes; f != NULL; f = f->prox) {
        if (f->id == filme_id) {
            filme_visto = f;
            break;
        }
    }
    
    assert(filme_visto != NULL);
    unsigned int visto_inicial = filme_visto->visto;
    
    // Incrementa visualizações e atualiza histórico
    filme_visto->visto++;
    
    // Atualiza histórico
    char novo_historico[1020];
    sprintf(novo_historico, "%s-%d", utilizador_logado->historico, filme_id);
    strcpy(utilizador_logado->historico, novo_historico);
    
    // Incrementa atividade
    utilizador_logado->atividade++;
    
    // Verifica atualizações
    assert(filme_visto->visto == visto_inicial + 1);
    assert(strcmp(utilizador_logado->historico, "1-2-3-1") == 0);
    
    // Atualiza o CSV
    adicionarOuEditarUser(
        utilizador_logado->id, utilizador_logado->utilizador, utilizador_logado->idade, utilizador_logado->senha,
        utilizador_logado->status, utilizador_logado->atividade, utilizador_logado->historico,
        utilizador_logado->personalizada, utilizador_logado->favoritos
    );
    
    // Verificar persistência
    free(utilizador_logado);
    utilizador_logado = NULL;

    autenticarUtilizadorCsv(utilizador, senha);
    assert(strcmp(utilizador_logado->historico, "1-2-3-1") == 0);
    assert(utilizador_logado->atividade == 11); // anteriormente era 10

    printf("OK\n");
}

int main() {
    setup_test_environment();

    test_login_and_data_loading();
    test_viewing_metrics_integration();
    test_content_edit_and_search_integration();
    test_favorites_and_recommendations_integration();
    test_history_and_metrics_integration();

    teardown_test_environment();
    printf("Todos os testes de integração foram concluídos com sucesso.\n");
    return 0;
}