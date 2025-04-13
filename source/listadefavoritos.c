
//listadefavoritos.c
#include "listadefavoritos.h"

// Função para remover um ID específico de uma lista de filmes
void remover_id_da_lista(char *lista, int id_remover) {
    char nova_lista[1000] = "";
    char *token = strtok(lista, "-");

    while (token != NULL) {
        int id_atual = atoi(token);
        if (id_atual != id_remover) {
            if (strlen(nova_lista) > 0) {
                strcat(nova_lista, "-");
            }
            strcat(nova_lista, token);
        }
        token = strtok(NULL, "-");
    }

    strcpy(lista, nova_lista); // Atualiza a lista com os IDs restantes
}

// Função para verificar se um ID já está na lista
int id_ja_existe(const char *lista, int id) {
    if (strlen(lista) == 0) return 0; // Lista vazia, ID não existe

    char temp_lista[1000];
    snprintf(temp_lista, sizeof(temp_lista), "-%s-", lista); // Adiciona "-" no início e fim para evitar falsos positivos

    char id_str[15]; 
    snprintf(id_str, sizeof(id_str), "-%d-", id); // Formata o ID com "-"

    return strstr(temp_lista, id_str) != NULL; // Retorna 1 se o ID já existir, 0 caso contrário
}


void criar_lista_personalizada_favoritos() {
    int escolha, id_filme;
    char nova_lista[1020];

    if (utilizador_logado == NULL) {
        return;
    }

    system("cls");
    printf("\n========== CRIAR LISTA ==========\n\n");
    printf("1 ==> Adicionar à lista personalizada\n");
    printf("2 ==> Adicionar aos favoritos\n\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);
    while (getchar() != '\n'); // Limpa o buffer

    if (escolha != 1 && escolha != 2) {
        return;
    }

    system("cls");
    printf("\n==========================================\n\n");
    printf("Digite o ID do filme que deseja adicionar: ");
    scanf("%d", &id_filme);
    while (getchar() != '\n'); // Limpa o buffer

    char *lista_selecionada = (escolha == 1) ? utilizador_logado->personalizada : utilizador_logado->favoritos;

    if (id_ja_existe(lista_selecionada, id_filme)) {
        printf("\n\nErro: O filme com ID %d já está na lista!\n", id_filme);
        printf("\n==========================================\n\n");
        printf("Aperte qualquer tecla para voltar ao menu: ");
        getchar();
        return;
    }

    // Adiciona o ID corretamente à lista sem corromper o formato
    if (strlen(lista_selecionada) > 0) {
        snprintf(nova_lista, sizeof(nova_lista), "%s-%d", lista_selecionada, id_filme);
    } else {
        snprintf(nova_lista, sizeof(nova_lista), "%d", id_filme);
    }
    strcpy(lista_selecionada, nova_lista);

    // Atualiza o CSV
    gravar_dados_utilizador_CSV(
        utilizador_logado->id, utilizador_logado->utilizador, utilizador_logado->idade, utilizador_logado->senha,
        utilizador_logado->status, utilizador_logado->atividade, utilizador_logado->historico,
        utilizador_logado->personalizada, utilizador_logado->favoritos
    );

    printf("\n\nFilme adicionado com sucesso!\n");
    printf("\n==========================================\n\n");
    printf("Aperte qualquer tecla para voltar ao menu: ");
    getchar();
}


void consultar_lista_personalizada_ou_favoritos() {
    int escolha;

    if (utilizador_logado == NULL) {
        return;
    }

    // Pergunta ao utilizador qual lista deseja consultar
    system("cls");
    printf("\n========== CONSULTAR LISTA ==========\n\n");
    printf("1 ==> Consultar Lista Personalizada\n");
    printf("2 ==> Consultar Lista de Favoritos\n\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);
    while (getchar() != '\n'); // Limpa o buffer de entrada
    system("cls");

    char *lista_selecionada;
    if (escolha == 1) {
        lista_selecionada = utilizador_logado->personalizada;
        printf("\n========== Lista Personalizada ==========\n\n");
    } else if (escolha == 2) {
        lista_selecionada = utilizador_logado->favoritos;
        printf("\n========== Lista de Favoritos ==========\n\n");
    } else {
        return;
    }

    // Se a lista estiver vazia
    if (strlen(lista_selecionada) == 0) {
        printf("Nenhum filme encontrado nesta lista!\n");
        printf("\n==========================================\n\n");
        printf("Aperte Qualquer Tecla para voltar ao menu: ");
        getchar();
        return;
    }

    // Percorre os IDs na string (ex: "1-3-5")
    char lista_copia[1000]; // Cria uma cópia da lista
    strcpy(lista_copia, lista_selecionada); // Copia a lista original

    char *token = strtok(lista_copia, "-"); // Usa a cópia em strtok
    int encontrou = 0;

    while (token != NULL) {
        int id_filme = atoi(token); // Converte o ID para inteiro
        Filme *atual = lista_filmes; // Percorre a lista ligada de filmes

        while (atual != NULL) {
            if (atual->id == id_filme) {
                printf("\nID: %d\n", atual->id);
                printf("Título: %s\n", atual->titulo);
                printf("Categoria: %c\n", atual->categoria);
                printf("Duração: %d minutos\n", atual->duracao);
                printf("Classificação Etária: %d+\n", atual->classificacao);
                printf("------------------------------------------\n");
                encontrou = 1;
                break;
            }
            atual = atual->prox;
        }

        token = strtok(NULL, "-"); // Pega o próximo ID da lista
    }

    if (!encontrou) {
        printf("Nenhum filme correspondente encontrado!\n");
    }

    printf("\n==========================================\n\n");
    printf("Aperte Qualquer Tecla para voltar ao menu: ");
    getchar();
}

void editar_ou_apagar_lista_personalizada_ou_favorito() {
    int escolha, id_remover;

    if (utilizador_logado == NULL) {
        return;
    }

    // Pergunta ao utilizador qual lista deseja modificar
    system("cls");
    printf("\n========== EDITAR / APAGAR LISTA ==========\n\n");
    printf("1 ==> Editar Lista Personalizada\n");
    printf("2 ==> Editar Lista de Favoritos\n\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);
    while (getchar() != '\n'); // Limpa o buffer de entrada
    system("cls");

    char *lista_selecionada;
    if (escolha == 1) {
        lista_selecionada = utilizador_logado->personalizada;
        printf("\n========== Editando Lista Personalizada ==========\n\n");
    } else if (escolha == 2) {
        lista_selecionada = utilizador_logado->favoritos;
        printf("\n========== Editando Lista de Favoritos ==========\n\n");
    } else {
        return;
    }

    // Se a lista estiver vazia
    if (strlen(lista_selecionada) == 0) {
        printf("A lista está vazia! Nada para editar ou remover.\n");
        printf("\n==========================================\n\n");
        printf("Aperte Qualquer Tecla para voltar ao menu: ");
        getchar();
        return;
    }

    // Mostra a lista atual para o utilizador
    printf("Lista atual: %s\n\n", lista_selecionada);

    // Pergunta ao utilizador o que deseja fazer
    printf("1 ==> Remover um filme específico\n");
    printf("2 ==> Apagar toda a lista\n\n");
    printf("Escolha uma opção: ");
    scanf("%d", &id_remover);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    if (id_remover == 1) {
        // Remover um filme específico
        printf("\nDigite o ID do filme que deseja remover: ");
        scanf("%d", &id_remover);
        while (getchar() != '\n'); // Limpa o buffer de entrada

        remover_id_da_lista(lista_selecionada, id_remover);
        printf("\nFilme removido da lista!\n");
    } else if (id_remover == 2) {
        // Apagar toda a lista
        strcpy(lista_selecionada, "");
        printf("\nLista apagada com sucesso!\n");
    } else {
        return;
    }

    // Atualiza os dados no CSV
    gravar_dados_utilizador_CSV(
        utilizador_logado->id, utilizador_logado->utilizador, utilizador_logado->idade, utilizador_logado->senha,
        utilizador_logado->status, utilizador_logado->atividade, utilizador_logado->historico,
        utilizador_logado->personalizada, utilizador_logado->favoritos
    );

    printf("\n==========================================\n\n");
    printf("Aperte Qualquer Tecla para voltar ao menu: ");
    getchar();

}
