//metricas.c
#include "metricas.h"

#define UTILIZADOR_PATH "users.csv"

void conteudos_mais_assistidos() {

    system("cls");

    if (lista_filmes == NULL) {
        printf("\nNenhum filme disponível.\n");
        return;
    }

    Filme *top1 = NULL, *top2 = NULL, *top3 = NULL;
    Filme *atual = lista_filmes;

    // Percorre a lista de filmes
    while (atual != NULL) {
        if (top1 == NULL || atual->visto > top1->visto) {
            // Desloca os filmes no ranking
            top3 = top2;
            top2 = top1;
            top1 = atual;
        } else if (top2 == NULL || atual->visto > top2->visto) {
            top3 = top2;
            top2 = atual;
        } else if (top3 == NULL || atual->visto > top3->visto) {
            top3 = atual;
        }
        atual = atual->prox;
    }

    // Exibe os resultados
    printf("\n========== TOP 3 FILMES MAIS ASSISTIDOS ==========\n\n");

    if (top1 != NULL) printf("1º: %s (ID: %d) - Assistido %u vezes\n", top1->titulo, top1->id, top1->visto);
    if (top2 != NULL) printf("2º: %s (ID: %d) - Assistido %u vezes\n", top2->titulo, top2->id, top2->visto);
    if (top3 != NULL) printf("3º: %s (ID: %d) - Assistido %u vezes\n", top3->titulo, top3->id, top3->visto);

    printf("\n===================================================\n\n");

    printf("Aperte Qualquer Tecla para voltar ao menu: ");
    getchar();
}


void categorias_mais_populares() {

    system("cls");

    if (lista_filmes == NULL) {
        printf("\nNenhum filme disponível.\n");
        return;
    }

    // Array para contar visualizações por categoria (26 letras do alfabeto)
    unsigned int contagem[26] = {0};

    // Percorre os filmes e soma os valores de 'visto' por categoria
    Filme *atual = lista_filmes;
    while (atual != NULL) {
        char cat = toupper(atual->categoria);  // Converte a categoria para maiúscula
        if (cat >= 'A' && cat <= 'Z') {
            contagem[cat - 'A'] += atual->visto;
        }
        atual = atual->prox;
    }

    // Identificar as duas categorias mais populares
    char cat1 = '\0', cat2 = '\0';
    unsigned int max1 = 0, max2 = 0;

    for (int i = 0; i < 26; i++) {
        if (contagem[i] > max1) {
            // Desloca a categoria mais assistida para a segunda posição
            max2 = max1;
            cat2 = cat1;

            // Atualiza a nova mais assistida
            max1 = contagem[i];
            cat1 = 'A' + i;
        } else if (contagem[i] > max2) {
            // Atualiza a segunda mais assistida
            max2 = contagem[i];
            cat2 = 'A' + i;
        }
    }

    // Exibir os resultados
    printf("\n========== CATEGORIAS MAIS POPULARES ==========\n\n");

    if (cat1 != '\0') printf("1º: Categoria %c - Assistida %u vezes\n", cat1, max1);
    if (cat2 != '\0') printf("2º: Categoria %c - Assistida %u vezes\n", cat2, max2);
    
    if (cat1 == '\0' && cat2 == '\0') {
        printf("Nenhuma categoria encontrada.\n");
    }

    printf("\n===================================================\n\n");
    printf("Aperte Qualquer Tecla para voltar ao menu: ");
    getchar();  // Aguarda entrada do usuário
}

void utilizadores_mais_ativos() {

    system("cls");

    if (utilizador_logado == NULL) {
        printf("\nNenhum utilizador disponível.\n");
        return;
    }

    Utilizador *top3[3] = {NULL, NULL, NULL};  // Array para armazenar os 3 mais ativos

    // Percorrer a lista de utilizadores e encontrar os mais ativos
    FILE *arquivo = fopen(UTILIZADOR_PATH, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o ficheiro de utilizadores!\n");
        return;
    }

    char linha[1200];  // Para armazenar cada linha lida do arquivo
    fgets(linha, sizeof(linha), arquivo);  // Ignorar o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Criar um utilizador temporário para armazenar os dados lidos
        Utilizador temp;
        sscanf(linha, "%d,%9[^,],%d,%u,%c,%d,%999[^,],%999[^,],%999[^,\n]",
               &temp.id, temp.utilizador, &temp.idade, &temp.senha, &temp.status, 
               &temp.atividade, temp.historico, temp.personalizada, temp.favoritos);

        // Comparar com os atuais top 3 mais ativos
        for (int i = 0; i < 3; i++) {
            if (top3[i] == NULL || temp.atividade > top3[i]->atividade) {
                // Desloca os menos ativos para baixo
                for (int j = 2; j > i; j--) {
                    top3[j] = top3[j - 1];
                }
                // Insere o novo utilizador mais ativo
                top3[i] = malloc(sizeof(Utilizador));
                if (top3[i] != NULL) {
                    *top3[i] = temp;
                }
                break;
            }
        }
    }
    fclose(arquivo);

    // Exibir os resultados
    printf("\n========== UTILIZADORES MAIS ATIVOS ==========\n\n");
    for (int i = 0; i < 3; i++) {
        if (top3[i] != NULL) {
            printf("%dº: %s - Atividade %d\n", i + 1, top3[i]->utilizador, top3[i]->atividade);
            free(top3[i]);  // Liberar memória
        }
    }

    if (top3[0] == NULL) {
        printf("Nenhum utilizador encontrado.\n");
    }

    printf("\n===================================================\n\n");
    printf("Aperte Qualquer Tecla para voltar ao menu: ");
    getchar();
}
