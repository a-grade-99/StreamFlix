#include "storageManager.h"

#define FILE_PATH "filmes.csv"
#define UTILIZADOR_PATH "users.csv"


Filme *lista_filmes = NULL;  // Ponteiro global para a lista ligada
Utilizador *utilizador_logado = NULL;

// A função lê o arquivo CSV, ignora o cabeçalho e armazena os dados numa lista ligada
void guardarFilmesCsv() {
    FILE *arquivo = fopen(FILE_PATH, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }

    char linha[200];
    int id, classificacao, duracao, visto;
    char titulo[100], categoria;

    // Ignorar o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    // Ler cada linha e guardar na lista
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%99[^,],%c,%d,%d,%d", &id, titulo, &categoria, &duracao, &classificacao, &visto);

        // Criar um novo nó
        Filme *novo_filme = (Filme *)malloc(sizeof(Filme));
        if (!novo_filme) {
            printf("Erro de alocação de memória!\n");
            fclose(arquivo);
            return;
        }

        // Preencher os dados
        novo_filme->id = id;
        strcpy(novo_filme->titulo, titulo);
        novo_filme->categoria = categoria;
        novo_filme->duracao = duracao;
        novo_filme->classificacao = classificacao;
        novo_filme->visto = visto;
        novo_filme->prox = lista_filmes; 
        lista_filmes = novo_filme;
    }

    fclose(arquivo);
}

// Liberta a memória alocada dinamicamente para a lista de filmes.
void limparListaFilmes() {
    Filme *atual = lista_filmes;
    while (atual != NULL) {
        Filme *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    lista_filmes = NULL;
}

// Autentica um utilizador com base no nome e senha fornecidos, lendo os dados de um arquivo CSV.
int autenticarUtilizadorCsv(char utilizador_digitado[10], unsigned int senha_digitada) {
    FILE *arquivo = fopen(UTILIZADOR_PATH, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return 0;
    }

    int id, idade, atividade;
    char utilizador[10], status;
    char historico[1000], personalizada[1000], favoritos[1000];
    unsigned int senha;
    char linha[1200];  

    // Ignorar o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%9[^,],%d,%u,%c,%d,%999[^,],%999[^,],%999[^,\n]",
            &id, utilizador, &idade, &senha, &status, &atividade, historico, personalizada, favoritos);

        if (strcmp(utilizador, utilizador_digitado) == 0 && senha_digitada == senha) {
            // **ALOCAR MEMÓRIA PARA O UTILIZADOR LOGADO**
            utilizador_logado = (Utilizador *)malloc(sizeof(Utilizador));
            if (!utilizador_logado) {
                printf("Erro de alocação de memória para o utilizador!\n");
                fclose(arquivo);
                return 0;
            }

            // Preencher os dados do utilizador logado
            utilizador_logado->id = id;
            strcpy(utilizador_logado->utilizador, utilizador);
            utilizador_logado->idade = idade;
            utilizador_logado->senha = senha;
            utilizador_logado->status = status;
            utilizador_logado->atividade = atividade;
            strcpy(utilizador_logado->historico, historico);
            strcpy(utilizador_logado->personalizada, personalizada);
            strcpy(utilizador_logado->favoritos, favoritos);

            fclose(arquivo);
            return 1;  // Sucesso
        }
    }

    fclose(arquivo);
    return 0;  // Utilizador não encontrado
}
