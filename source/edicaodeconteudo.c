
//edicaodeconteudo.c
#include "edicaodeconteudo.h"
#include "databasetools.h"

void adicionar_editar_conteudo(){
    char titulo[100];
    char categoria;
    unsigned int duracao;
    unsigned int classificacao;
    char validacao;

    while(1){
        system("cls");
        
        printf("\n\n=============== ADICIONAR FILME ===============\n\n");

        printf("Digite o Título do Filme: ");
        fgets(titulo, sizeof(titulo), stdin);
        titulo[strcspn(titulo, "\n")] = '\0'; // Remove a quebra de linha

        printf("Digite a Categoria do Filme (ex: A, B, C): ");
        scanf("%c", &categoria);
        while (getchar() != '\n'); // Limpa o buffer antes de voltar

        printf("Digite a Duração em Minutos: ");
        scanf("%u", &duracao);
        while (getchar() != '\n'); // Limpa o buffer antes de voltar

        printf("Digite a Classificação Etária: ");
        scanf("%u", &classificacao);
        while (getchar() != '\n'); // Limpa o buffer antes de voltar

        printf("\n=========================================\n\n");
        printf("Os Dados Foram Digitados Correctamente? (Y/N): ");
        scanf("%c", &validacao);
        while (getchar() != '\n'); // Limpa o buffer antes de voltar
        

        if (validacao == 'Y' || validacao == 'y'){
            gravar_dados_filmes_CSV(titulo, categoria, duracao, classificacao);
            break;
        }
    }
}

void remover_conteudo(){

    char titulo[100];

    system("cls");
        
    printf("\n\n============== REMOVER FILME ==============\n\n");

    printf("Digite o Título do Filme Que Deseja Apagar: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0'; // Remove a quebra de linha

    apagar_dados_filmes_CSV(titulo);

}
