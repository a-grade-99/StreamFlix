#include "edicaodeconteudo.h"
#include "databasetools.h"

// Interface para adicionar ou editar um filme
void interfaceAdicionarOuEditarFilme(){
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
            adicionarOuEditarFilme(titulo, categoria, duracao, classificacao);
            break;
        }
    }
}

// Interface para remover um filme
void interfaceRemoverFilme(){

    char titulo[100];

    system("cls");
        
    printf("\n\n============== REMOVER FILME ==============\n\n");

    printf("Digite o Título do Filme Que Deseja Apagar: ");
    fgets(titulo, sizeof(titulo), stdin);
    titulo[strcspn(titulo, "\n")] = '\0'; // Remove a quebra de linha

    apagarFilme(titulo);

}
