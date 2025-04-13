
//player.c
#include "historicoFilmes.h"

#define FILE_PATH "filmes.csv"
#define TEMP_FILE "temp.csv"

// Função para selecionar um filme para assistir
void selecionarFilme() {

    system("cls");

    if (lista_filmes == NULL) {
        printf("Nenhum filme disponível.\n");
        return;
    }

    int id_filme;
    printf("\nDigite o ID do filme que deseja assistir: ");
    scanf("%d", &id_filme);

    Filme *filme_atual = lista_filmes;
    while (filme_atual && filme_atual->id != id_filme) {
        filme_atual = filme_atual->prox;
    }

    if (filme_atual == NULL) {
        printf("\nFilme não encontrado.\n");
        return;
    }

    int opcao;
    int assistindo = 1;

    while (assistindo) {
        system("cls");
        printf("\nFilme Selecionado: %s\n", filme_atual->titulo);
        printf("\n\n========== MENU DE REPRODUÇÃO ==========\n\n");
        printf("1. Reproduzir Filme\n");
        printf("2. Pausar Filme\n");
        printf("3. Concluir Visualização\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("\nA Reproduzir: %s\n", filme_atual->titulo);
                utilizador_logado->atividade++;
                printf("\nClique no Enter para continuar...");
                getchar();
                break;
            case 2:
                printf("\nFilme pausado: %s\n", filme_atual->titulo);
                utilizador_logado->atividade++;
                printf("\nPressione Enter para continuar...");
                getchar();
                break;
            case 3:
                printf("\nFilme concluído: %s\n", filme_atual->titulo);
                utilizador_logado->atividade++;
                // Incrementar visualizações do filme - atualiza em memória
                filme_atual->visto++;
                
                // Atualizar visualizações no arquivo CSV
                FILE *arquivo = fopen(FILE_PATH, "r");
                FILE *temp = fopen(TEMP_FILE, "w");
                
                if (arquivo == NULL || temp == NULL) {
                    printf("Erro ao abrir o ficheiro para atualizar visualizações!\n");
                } else {
                    char linha[200];
                    int id, duracao, classificacao, visto;
                    char titulo[100], categoria;
                    
                    // Copia o cabeçalho
                    if (fgets(linha, sizeof(linha), arquivo)) {
                        fprintf(temp, "%s", linha);
                    }
                    
                    // Lê cada linha e atualiza o filme correspondente
                    while (fgets(linha, sizeof(linha), arquivo)) {
                        sscanf(linha, "%d,%99[^,],%c,%d,%d,%d", &id, titulo, &categoria, &duracao, &classificacao, &visto);
                        
                        if (id == filme_atual->id) {
                            // Incrementa o contador de visualizações
                            fprintf(temp, "%d,%s,%c,%d,%d,%d\n", id, titulo, categoria, duracao, classificacao, filme_atual->visto);
                        } else {
                            // Mantém os outros filmes sem alterações
                            fprintf(temp, "%s", linha);
                        }
                    }
                    
                    fclose(arquivo);
                    fclose(temp);
                    
                    // Substitui o CSV original pelo atualizado
                    remove(FILE_PATH);
                    rename(TEMP_FILE, FILE_PATH);
                }
                
                // Atualizar histórico do usuário
                if (utilizador_logado != NULL) {
                    // Verifica se o histórico está vazio
                    if (utilizador_logado->historico[0] == '\0') {
                        // Se vazio, apenas adiciona o ID do filme
                        sprintf(utilizador_logado->historico, "%d", filme_atual->id);
                    } else {
                        // Caso contrário, adiciona um separador e o ID do filme
                        char temp_historico[1020];
                        sprintf(temp_historico, "%s-%d", utilizador_logado->historico, filme_atual->id);
                        strcpy(utilizador_logado->historico, temp_historico);
                    }
                }
                assistindo = 0;
                break;
            case 4:
                utilizador_logado->atividade++;
                assistindo = 0;
                break;
            default:
                printf("\nOpção inválida!\n");
                printf("Pressione Enter para continuar...");
                getchar();
        }
    }

    printf("\n===================================================\n\n");
    printf("Clique em qualquer tecla para voltar ao menu: ");
    getchar();
}

// Função para listar o histórico de visualizações
void listarHistorico() {

    system("cls");

    if (utilizador_logado == NULL || strlen(utilizador_logado->historico) == 0) {
        printf("\nO histórico de visualizações está vazio.\n");
    } else {
        printf("\n========== HISTÓRICO DE VISUALIZAÇÕES ==========\n\n");

        char historico_temp[1000];
        strcpy(historico_temp, utilizador_logado->historico);

        char *token = strtok(historico_temp, "-");
        while (token != NULL) {
            int id_filme = atoi(token);
            
            Filme *filme_atual = lista_filmes;
            while (filme_atual != NULL) {
                if (filme_atual->id == id_filme) {
                    printf("ID: %d | Título: %s\n", filme_atual->id, filme_atual->titulo);
                    break;
                }
                filme_atual = filme_atual->prox;
            }
            token = strtok(NULL, "-");
        }
    }

    printf("\n===================================================\n\n");
    printf("Aperte qualquer tecla para voltar ao menu: ");
    getchar();
}

// Função para recomendar filmes com base no histórico
void filmesRecomendados() {

    system("cls");

    if (utilizador_logado == NULL || lista_filmes == NULL || strlen(utilizador_logado->historico) == 0) {
        printf("\nNenhum utilizador logado, filmes disponíveis ou histórico registrado.\n");
        printf("\n===================================================\n\n");
        printf("Aperte qualquer tecla para voltar ao menu: ");
        getchar();
        return;
    }

    // Contador de categorias
    int contador_categorias[256] = {0}; // Índice é o código ASCII da categoria

    // Contabilizar categorias no histórico
    char historico_temp[1000];
    strcpy(historico_temp, utilizador_logado->historico);

    char *token = strtok(historico_temp, "-");
    while (token != NULL) {
        int id_filme = atoi(token);

        Filme *filme_atual = lista_filmes;
        while (filme_atual != NULL) {
            if (filme_atual->id == id_filme) {
                contador_categorias[(unsigned char)filme_atual->categoria]++;
                break;
            }
            filme_atual = filme_atual->prox;
        }
        token = strtok(NULL, "-");
    }

    // Identificar a categoria mais assistida
    char categoria_mais_assistida = '\0';
    int max_contagem = 0;

    for (int i = 0; i < 256; i++) {
        if (contador_categorias[i] > max_contagem) {
            max_contagem = contador_categorias[i];
            categoria_mais_assistida = (char)i;
        }
    }

    // Listar recomendações
    printf("\n============= FILMES RECOMENDADOS =============\n\n");

    int recomendados = 0;
    Filme *filme_atual = lista_filmes;
    while (filme_atual != NULL && recomendados < 2) {
        if (filme_atual->categoria == categoria_mais_assistida) {
            printf("ID: %d | Título: %s | Categoria: %c\n", filme_atual->id, filme_atual->titulo, filme_atual->categoria);
            recomendados++;
        }
        filme_atual = filme_atual->prox;
    }

    if (recomendados == 0) {
        printf("Nenhuma recomendação disponível no momento.\n");
    }

    printf("\n===================================================\n\n");
    printf("Aperte qualquer tecla para voltar ao menu: ");
    getchar();
}


