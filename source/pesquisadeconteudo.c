
//pesquisadeconteudo.c
#include "pesquisadeconteudo.h"

void pesquisar_por_titulo() {
    char titulo[100];

    while (1) {
        system("cls");

        printf("\n\n========== PESQUISA POR TÍTULO ==========\n\n");

        printf("Digite o Título do Filme: ");
        fgets(titulo, sizeof(titulo), stdin);
        titulo[strcspn(titulo, "\n")] = '\0'; // Remove a quebra de linha

        Filme *atual = lista_filmes;
        int encontrado = 0;

        printf("\n========= RESULTADOS DA PESQUISA =========\n");

        while (atual != NULL) {
            if (strcmp(atual->titulo, titulo) == 0) {
                printf("\nID: %d\n", atual->id);
                printf("Título: %s\n", atual->titulo);
                printf("Categoria: %c\n", atual->categoria);
                printf("Duração: %d minutos\n", atual->duracao);
                printf("Classificação Etária: %d+\n", atual->classificacao);
                printf("------------------------------------------\n");
                encontrado = 1;
            }
            atual = atual->prox;
        }

        if (!encontrado) {
            printf("\nNenhum filme encontrado com esse título.\n");
        }

        printf("\n==========================================\n\n");
        printf("Tecle Qualquer Tecla Para Sair: ");
        getchar();

        break;
    }
}

void pesquisar_por_categoria() {
    char categoria;

    while (1) {
        system("cls");

        printf("\n\n========== PESQUISA POR CATEGORIA ==========\n\n");

        printf("Digite a Categoria do Filme (ex: A para Ação, C para Comédia): ");
        scanf("%c", &categoria);
        while (getchar() != '\n'); // Limpa o buffer antes de voltar

        Filme *atual = lista_filmes;  
        int encontrado = 0;

        printf("\n========= RESULTADOS DA PESQUISA =========\n");

        while (atual != NULL) {
            if (atual->categoria == categoria) {
                printf("\nID: %d\n", atual->id);
                printf("Título: %s\n", atual->titulo);
                printf("Categoria: %c\n", atual->categoria);
                printf("Duração: %d minutos\n", atual->duracao);
                printf("Classificação Etária: %d+\n", atual->classificacao);
                printf("------------------------------------------\n");
                encontrado = 1;
            }
            atual = atual->prox;
        }

        if (!encontrado) {
            printf("\nNenhum filme encontrado na categoria '%c'.\n", categoria);
        }

        printf("\n==========================================\n\n");
        printf("Tecle Qualquer Tecla Para Sair: ");
        getchar();

        break;
    }
}

void pesquisar_por_faixa_etaria() {
    int idade;

    while (1) {
        system("cls");

        printf("\n\n========== PESQUISA POR FAIXA ETÁRIA ==========\n\n");

        printf("Digite a idade para ver os filmes recomendados: ");
        scanf("%d", &idade);
        while (getchar() != '\n'); // Limpa o buffer antes de voltar
        
        Filme *atual = lista_filmes;  
        int encontrado = 0;

        printf("\n========= RESULTADOS DA PESQUISA =========\n");

        while (atual != NULL) {
            if (atual->classificacao <= idade) { // Exibe filmes com classificação menor ou igual à idade informada
                printf("\nID: %d\n", atual->id);
                printf("Título: %s\n", atual->titulo);
                printf("Categoria: %c\n", atual->categoria);
                printf("Duração: %d minutos\n", atual->duracao);
                printf("Classificação Etária: %d+\n", atual->classificacao);
                printf("------------------------------------------\n");
                encontrado = 1;
            }
            atual = atual->prox;
        }

        if (!encontrado) {
            printf("\nNenhum filme encontrado para a idade %d.\n", idade);
        }

        printf("\n==========================================\n\n");
        printf("Tecle Qualquer Tecla Para Sair: ");
        getchar();

        break;

    }
}

